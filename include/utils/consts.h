#ifndef CONSTS_H
#define CONSTS_H

/* --------------------- Window and Display Settings --------------------- */
#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

#define WINDOWED_MODE_WIDTH 1280
#define WINDOWED_MODE_HEIGHT 720

#define WINDOW_TITLE "SDL Platformer"

/* --------------------- Game World Dimensions --------------------- */
#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080

/* --------------------- Texture and Font Paths --------------------- */
#define BACKGROUND_TEXTURE_PATH "resources/textures/background.png"
#define PLAYER_TEXTURE_PATH "resources/textures/player.png"
#define ENEMY_TEXTURE_PATH "resources/textures/enemy.png"
#define PLATFORM_TEXTURE_PATH "resources/textures/platform.png"
#define BULLET_TEXTURE_PATH "resources/textures/bullet.png"

#define FONT_PATH "resources/fonts/FreeSans.ttf"

/* --------------------- Map Files --------------------- */
#define MAPS                                                                   \
  {                                                                            \
    "resources/maps/mountains.cfg", "resources/maps/urban.cfg",                \
        "resources/maps/desert.cfg", "resources/maps/industrial.cfg"          \
  }

/* --------------------- Sprite and Animation Dimensions ---------------------
 */
/* Player/Enemy Sprite Settings */
#define SPRITE_WIDTH 40
#define SPRITE_HEIGHT 100
#define SPRITES_PER_ROW 6

/* Humanoid Frame (likely same as player/enemy sprite) */
#define HUMANOID_FRAME_WIDTH 40
#define HUMANOID_FRAME_HEIGHT 100

/* Bullet Sprite and Dimensions */
#define BULLET_SPRITE_WIDTH 20
#define BULLET_SPRITE_HEIGHT 20

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 10

/* --------------------- Health Bar Settings --------------------- */
#define HEALTH_BAR_OFFSET_X 20
#define HEALTH_BAR_OFFSET_Y 20

#define HEALTH_BAR_SCALE 2

#define HEALTH_BAR_BG_WIDTH 102
#define HEALTH_BAR_BG_HEIGHT 12

#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_HEIGHT 10

#define HEALTH_BAR_TEXT "Level: "

/* --------------------- Player and Enemy Movement --------------------- */
#define PLAYER_MOVE_SPEED 5
#define PLAYER_JUMP_SPEED 600
#define PLAYER_FALL_INCREMENT 800

#define ENEMY_MOVE_SPEED 60
#define ENEMY_JUMP_SPEED 200
#define ENEMY_FALL_INCREMENT 800
/* Enemy shooting */
#define ENEMY_BULLET_SPEED 200

/* --------------------- Other Gameplay Constants --------------------- */
#define NUM_BULLETS 100

/* --------------------- New Gameplay Features --------------------- */
/* Melee Combat */
#define MELEE_ATTACK_RANGE 50
#define MELEE_ATTACK_DAMAGE 30
#define MELEE_ATTACK_COOLDOWN 0.5f

/* Grenades */
#define GRENADE_FUSE_TIME 2.0f
#define GRENADE_EXPLOSION_DURATION 0.3f
#define GRENADE_EXPLOSION_RADIUS 80
#define GRENADE_EXPLOSION_DAMAGE 50
#define GRENADE_THROW_SPEED 300
#define MAX_GRENADES 20

/* Parachute Enemies */
#define PARACHUTE_SPAWN_INTERVAL 15.0f
#define PARACHUTE_SPAWN_COUNT 3
#define PARACHUTE_DESCENT_SPEED 50.0f

/* Ladders */
#define LADDER_CLIMB_SPEED 150
#define LADDER_WIDTH 30

/* Traps */
#define TRAP_FUEL_BARREL_DAMAGE 75
#define TRAP_SPIKE_DAMAGE 25
#define TRAP_FIRE_DAMAGE 10

/* Flags */
#define FLAG_CAPTURE_TIME 3.0f
#define FLAG_CAPTURE_POINTS 500

/* Level Timer */
#define TIMER_DISPLAY_X 1750
#define TIMER_DISPLAY_Y 20

#endif // CONSTS_H
