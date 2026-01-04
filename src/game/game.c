#include "game/game.h"
#include "game/event_handler.h"
#include "game/game_context.h"
#include "game/game_state.h"
#include "game/main_menu.h"
#include "game/physics.h"
#include "game/score.h"
#include "map/map_manager.h"
#include "map/trap.h"
#include "map/flag.h"
#include "entities/grenade_pool.h"
#include "utils/consts.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Spawn parachuting enemies if timer has elapsed.
 */
static void updateParachuteSpawns(GameContext *ctx, float dt) {
  ctx->parachuteSpawnTimer -= dt;
  
  if (ctx->parachuteSpawnTimer <= 0) {
    ctx->parachuteSpawnTimer = ctx->parachuteSpawnInterval;
    
    /* Spawn parachuting enemies at random X positions above the map */
    for (int i = 0; i < ctx->parachuteWaveCount; i++) {
      float spawnX = (float)(rand() % ctx->map->rect.w);
      float spawnY = -100.0f; /* Above the visible area */
      
      /* Pick a random platform as landing target */
      if (ctx->map->platformCount > 0) {
        size_t platformIndex = (size_t)(rand() % (int)ctx->map->platformCount);
        float targetY =
            (float)(ctx->map->platforms[platformIndex].y -
                    HUMANOID_FRAME_HEIGHT);
        
        Enemy *parachuteEnemy = enemyCreateParachuting(spawnX, spawnY, targetY);
        if (parachuteEnemy) {
          enemyArrayAdd(ctx->enemies, parachuteEnemy);
        }
      }
    }
  }
}

/**
 * @brief Update environmental traps and check for damage.
 */
static void updateTraps(GameContext *ctx, float dt) {
  if (!ctx->map->traps)
    return;
    
  TrapArray *traps = (TrapArray *)ctx->map->traps;
  trapArrayUpdate(traps, dt);
  
  /* Check for trap damage to player */
  SDL_Rect playerRect = {(int)ctx->player->base.pos.x, 
                          (int)ctx->player->base.pos.y,
                          SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
  int damage = trapArrayCheckDamage(traps, &playerRect);
  if (damage > 0) {
    ctx->player->base.health -= damage;
    scoreRegisterDamage(ctx->scoreTracker, damage);
  }
  
  /* Check for trap damage to enemies */
  for (int i = 0; i < ctx->enemies->count; i++) {
    Enemy *enemy = ctx->enemies->data[i];
    if (!isEnemyAlive(enemy))
      continue;
      
    SDL_Rect enemyRect = {(int)enemy->base.pos.x, 
                          (int)enemy->base.pos.y,
                          SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
    damage = trapArrayCheckDamage(traps, &enemyRect);
    if (damage > 0) {
      enemy->base.health -= damage;
    }
  }
}

/**
 * @brief Update flag capture objectives.
 */
static void updateFlags(GameContext *ctx, float dt) {
  if (!ctx->map->flags)
    return;
    
  FlagArray *flags = (FlagArray *)ctx->map->flags;
  
  SDL_Rect playerRect = {(int)ctx->player->base.pos.x, 
                          (int)ctx->player->base.pos.y,
                          SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
  
  int capturedCount = flagArrayUpdate(flags, &playerRect, dt);
  
  /* Award points for captures */
  for (int i = 0; i < capturedCount; i++) {
    scoreRegisterFlagCapture(ctx->scoreTracker, FLAG_CAPTURE_POINTS);
  }
}

/**
 * @brief Handle melee combat between player and enemies.
 */
static void updateMeleeCombat(GameContext *ctx) {
  if (!ctx->player->isMeleeAttacking)
    return;
    
  for (int i = 0; i < ctx->enemies->count; i++) {
    Enemy *enemy = ctx->enemies->data[i];
    if (!isEnemyAlive(enemy))
      continue;
      
    SDL_Rect enemyRect = {(int)enemy->base.pos.x, 
                          (int)enemy->base.pos.y,
                          SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
    
    if (playerMeleeHits(ctx->player, &enemyRect)) {
      int damage = playerGetMeleeDamage(ctx->player);
      enemy->base.health -= damage;
      
      if (!isEnemyAlive(enemy)) {
        scoreRegisterMeleeKill(ctx->scoreTracker, 150);
      }
    }
  }
}

/**
 * @brief Handle grenade explosions and damage.
 */
static void updateGrenades(GameContext *ctx, float dt) {
  if (!ctx->grenadePool)
    return;
    
  grenadePoolUpdate(ctx->grenadePool, dt);
  
  /* Check grenade explosions for damage */
  for (int i = 0; i < MAX_GRENADES; i++) {
    Grenade *grenade = ctx->grenadePool->grenades[i];
    if (!grenade || !isGrenadeExploding(grenade))
      continue;
    
    SDL_Rect explosionRect = grenadeGetExplosionBounds(grenade);
    
    /* Damage player if not their grenade */
    if (grenade->source == GRENADE_SOURCE_ENEMY) {
      SDL_Rect playerRect = {(int)ctx->player->base.pos.x, 
                              (int)ctx->player->base.pos.y,
                              SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
      if (SDL_HasIntersection(&explosionRect, &playerRect)) {
        ctx->player->base.health -= grenade->explosionDamage;
        scoreRegisterDamage(ctx->scoreTracker, grenade->explosionDamage);
      }
    }
    
    /* Damage enemies if player grenade */
    if (grenade->source == GRENADE_SOURCE_PLAYER) {
      for (int j = 0; j < ctx->enemies->count; j++) {
        Enemy *enemy = ctx->enemies->data[j];
        if (!isEnemyAlive(enemy))
          continue;
          
        SDL_Rect enemyRect = {(int)enemy->base.pos.x, 
                              (int)enemy->base.pos.y,
                              SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
        
        if (SDL_HasIntersection(&explosionRect, &enemyRect)) {
          enemy->base.health -= grenade->explosionDamage;
          
          if (!isEnemyAlive(enemy)) {
            scoreRegisterGrenadeKill(ctx->scoreTracker, 200);
          }
        }
      }
    }
    
    /* Trigger any traps hit by explosion */
    if (ctx->map->traps) {
      TrapArray *traps = (TrapArray *)ctx->map->traps;
      trapArrayTriggerAt(traps, grenade->base.pos.x, grenade->base.pos.y);
    }
  }
}

void gameLoop(GameContext *ctx, bool *gameRunning, GameState *gameState,
              Uint32 *last) {
  SDL_Event e;

  while (*gameRunning && *gameState == STATE_GAME) {
    handleGameEventsExtended(&e, ctx->player, gameState, gameRunning,
                             ctx->bulletPool, ctx->grenadePool);

    // Calculate delta time.
    Uint32 now = SDL_GetTicks();
    float dt = (now - *last) / 1000.0f;
    *last = now;

    /* Update level timer */
    scoreUpdateElapsedTime(ctx->scoreTracker, dt);

    if (ctx->player->base.update) {
      ctx->player->base.update(&ctx->player->base, dt);
    }
    enemyArrayUpdate(ctx->enemies, dt, ctx->bulletPool, ctx->player);
    applyPhysics(ctx->player, ctx->enemies, ctx->map, dt);
    bulletPoolUpdate(ctx->bulletPool, dt);
    TrapArray *traps = NULL;
    if (ctx->map->traps) {
      traps = (TrapArray *)ctx->map->traps;
    }
    handleCollisions(ctx->bulletPool, ctx->player, ctx->enemies, traps);

    /* Update new gameplay features */
    updateParachuteSpawns(ctx, dt);
    updateGrenades(ctx, dt);
    updateTraps(ctx, dt);
    updateFlags(ctx, dt);
    updateMeleeCombat(ctx);

    if (!isPlayerAlive(ctx->player)) {
      printf("Player is dead!\n");
      
      /* Display final stats */
      char timeStr[16];
      scoreGetElapsedTimeString(ctx->scoreTracker, timeStr, sizeof(timeStr));
      printf("=== LEVEL COMPLETE ===\n");
      printf("Time: %s\n", timeStr);
      printf("Score: %d\n", ctx->scoreTracker->currentScore);
      printf("Enemies Killed: %d\n", ctx->scoreTracker->enemiesKilled);
      printf("Melee Kills: %d\n", ctx->scoreTracker->meleeKills);
      printf("Grenade Kills: %d\n", ctx->scoreTracker->grenadeKills);
      printf("Flags Captured: %d\n", ctx->scoreTracker->flagsCaptured);
      printf("Accuracy: %.1f%%\n", scoreGetAccuracy(ctx->scoreTracker));
      printf("======================\n");
      
      *gameRunning = false;
      *gameState = STATE_MENU;
    }
    
    /* Check if all flag objectives are complete */
    if (ctx->map->flags) {
      FlagArray *flags = (FlagArray *)ctx->map->flags;
      if (flagArrayObjectivesComplete(flags)) {
        printf("All flags captured! Level complete!\n");
        
        char timeStr[16];
        scoreGetElapsedTimeString(ctx->scoreTracker, timeStr, sizeof(timeStr));
        printf("=== LEVEL COMPLETE ===\n");
        printf("Time: %s\n", timeStr);
        printf("Score: %d\n", ctx->scoreTracker->currentScore);
        printf("======================\n");
      }
    }

    // Render everything.
    renderGame(ctx);
    SDL_Delay(16);
  }
}

void loadLevel(SDL_Renderer *renderer, TextureManager *texManager,
               GameState *gameState, Map *map) {
  // Create game objects dynamically.
  Player *player = playerCreate(320, 400);

  // Allocate and initialize the enemy array on the heap.
  EnemyArray *enemies = malloc(sizeof(EnemyArray));
  enemyArrayInit(enemies);

  // Create one enemy per platform.
  for (size_t i = 0; i < map->platformCount; i++) {
    Point enemyPos = getRandomPositionOnPlatform(&map->platforms[i]);
    Enemy *enemy = enemyCreate(enemyPos.x, enemyPos.y);
    enemyArrayAdd(enemies, enemy);
  }

  // Allocate and initialize the bullet pool on the heap.
  BulletPool *bulletPool = malloc(sizeof(BulletPool));
  bulletPoolInit(bulletPool, 100);

  // Allocate and initialize the grenade pool.
  GrenadePool *grenadePool = malloc(sizeof(GrenadePool));
  grenadePoolInit(grenadePool);

  // Initialize score tracker for this level.
  ScoreTracker *scoreTracker = scoreGetGlobal();
  scoreReset(scoreTracker);

  bool gameRunning = true;
  Uint32 last = SDL_GetTicks();

  // Create the game context struct with heap-allocated objects.
  GameContext ctx = {
      .renderer = renderer,
      .texManager = texManager,
      .map = map,
      .player = player,
      .enemies = enemies,
      .bulletPool = bulletPool,
      .grenadePool = grenadePool,
      .scoreTracker = scoreTracker,
      .parachuteSpawnTimer = PARACHUTE_SPAWN_INTERVAL,
      .parachuteSpawnInterval = PARACHUTE_SPAWN_INTERVAL,
      .parachuteWaveCount = PARACHUTE_SPAWN_COUNT,
  };

  // Start the game loop.
  gameLoop(&ctx, &gameRunning, gameState, &last);

  // --- Clean Up Game Objects ---
  playerDestroy(player);
  enemyArrayDestroy(enemies);
  bulletPoolDestroy(bulletPool);
  grenadePoolDestroy(grenadePool);
  destroyTextureManager(texManager);

  // Free the heap-allocated memory.
  free(enemies);
  free(bulletPool);
  free(grenadePool);
}

void runGame(SDL_Renderer *renderer) {
  GameState gameState = STATE_MENU;

  while (gameState != STATE_EXIT) {
    if (gameState == STATE_MENU) {
      runMainMenu(renderer, &gameState);
      printf("Menu exited with state: %d\n", gameState);
    } else if (gameState == STATE_GAME) {
      const char *maps[] = MAPS;
      size_t numMaps = sizeof(maps) / sizeof(maps[0]);
      MapManager manager;

      initMapManager(&manager, maps, numMaps);
      Map map;
      if (getNextMap(&manager, &map) == 0) {
        printf("Loaded map from: %s\n",
               maps[(manager.currentIndex + manager.mapCount - 1) %
                    manager.mapCount]);
        TextureManager texManager = initTextureManager(
            renderer, PLAYER_TEXTURE_PATH, ENEMY_TEXTURE_PATH,
            BULLET_TEXTURE_PATH, map.backgroundImage, FONT_PATH);
        loadLevel(renderer, &texManager, &gameState, &map);
        mapDestructor(&map);
      } else {
        fprintf(stderr, "Error loading map from: %s\n",
                maps[(manager.currentIndex + manager.mapCount - 1) %
                     manager.mapCount]);
      }
    }
  }
}
