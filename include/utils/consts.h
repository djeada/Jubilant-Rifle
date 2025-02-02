#ifndef CONSTS_H
#define CONSTS_H

/* --------------------- Window and Display Settings --------------------- */
#define DEFAULT_WINDOW_WIDTH    1920
#define DEFAULT_WINDOW_HEIGHT   1080

#define WINDOWED_MODE_WIDTH     1280
#define WINDOWED_MODE_HEIGHT    720

#define WINDOW_TITLE            "SDL Platformer"

/* --------------------- Game World Dimensions --------------------- */
#define GAME_WIDTH              1920
#define GAME_HEIGHT             1080

#define WORLD_WIDTH             3000
#define WORLD_HEIGHT            1200

/* --------------------- Texture and Font Paths --------------------- */
#define BACKGROUND_TEXTURE_PATH "resources/textures/background.png"
#define PLAYER_TEXTURE_PATH     "resources/textures/player.png"
#define ENEMY_TEXTURE_PATH      "resources/textures/enemy.png"
#define PLATFORM_TEXTURE_PATH   "resources/textures/platform.png"
#define BULLET_TEXTURE_PATH     "resources/textures/bullet.png"

#define FONT_PATH               "resources/fonts/FreeSans.ttf"

/* --------------------- Map Files --------------------- */
#define MAPS { \
    "resources/maps/mountains.cfg", \
    "resources/maps/mountains.cfg", \
    "resources/maps/mountains.cfg"  \
}

/* --------------------- Sprite and Animation Dimensions --------------------- */
/* Player/Enemy Sprite Settings */
#define SPRITE_WIDTH            40
#define SPRITE_HEIGHT           100
#define SPRITES_PER_ROW         6

/* Humanoid Frame (likely same as player/enemy sprite) */
#define HUMANOID_FRAME_WIDTH    40
#define HUMANOID_FRAME_HEIGHT   100

/* Bullet Sprite and Dimensions */
#define BULLET_SPRITE_WIDTH     20
#define BULLET_SPRITE_HEIGHT    20

#define BULLET_WIDTH            10
#define BULLET_HEIGHT           10

/* --------------------- Health Bar Settings --------------------- */
#define HEALTH_BAR_OFFSET_X     20
#define HEALTH_BAR_OFFSET_Y     20

#define HEALTH_BAR_SCALE        2

#define HEALTH_BAR_BG_WIDTH     102
#define HEALTH_BAR_BG_HEIGHT    12

#define HEALTH_BAR_WIDTH        100
#define HEALTH_BAR_HEIGHT       10

#define HEALTH_BAR_TEXT         "Level: "

/* --------------------- Player and Enemy Movement --------------------- */
#define PLAYER_MOVE_SPEED       5
#define PLAYER_JUMP_SPEED       200
#define PLAYER_FALL_INCREMENT   20

#define ENEMY_MOVE_SPEED        2
#define ENEMY_JUMP_SPEED        200
#define ENEMY_FALL_INCREMENT    20

/* --------------------- Other Gameplay Constants --------------------- */
#define NUM_BULLETS             100

#endif // CONSTS_H
