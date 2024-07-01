#ifndef CONSTS_H
#define CONSTS_H

#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080
#define SPRITE_WIDTH 40
#define SPRITE_HEIGHT 100
#define SPRITES_PER_ROW 6
#define BULLET_SPRITE_WIDTH 20
#define BULLET_SPRITE_HEIGHT 20
#define HEALTH_BAR_OFFSET_X 20
#define HEALTH_BAR_OFFSET_Y 20
#define HEALTH_BAR_SCALE 2
#define HEALTH_BAR_BG_WIDTH 102
#define HEALTH_BAR_BG_HEIGHT 12
#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_HEIGHT 10
#define HEALTH_BAR_TEXT "Level: "

#define BACKGROUND_TEXTURE_PATH "resources/textures/background.png"
#define BULLET_TEXTURE_PATH "resources/textures/bullet.png"
#define PLAYER_TEXTURE_PATH "resources/textures/player.png"
#define ENEMY_TEXTURE_PATH "resources/textures/enemy.png"
#define PLATFORM_TEXTURE_PATH "resources/textures/platform.png"

#define WORLD_WIDTH 3000
#define WORLD_HEIGHT 1200

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

#define WINDOWED_MODE_WIDTH 1280
#define WINDOWED_MODE_HEIGHT 720

#define HUMANOID_FRAME_HEIGHT 100
#define HUMANOID_FRAME_WIDTH 40

#define BULLET_HEIGHT 10
#define BULLET_WIDTH 10

#define MOVE_SPEED 5
#define JUMP_SPEED 200
#define FALL_INCREMENT 20

#define NUM_BULLETS 100

#define MAP_FILE_PATH "resources/maps/mountains.cfg"
#define FONT_PATH "resources/fonts/FreeSans.ttf"
#define WINDOW_TITLE "SDL Platformer"

#endif
