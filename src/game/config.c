#include "game/config.h"
#include "utils/consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global configuration instance */
static GameConfig g_config;
static bool g_configInitialized = false;

void configInit(GameConfig *config) {
  if (!config)
    return;

  /* Audio defaults */
  config->audio.masterVolume = 1.0f;
  config->audio.musicVolume = 0.7f;
  config->audio.sfxVolume = 0.8f;
  config->audio.musicEnabled = true;
  config->audio.sfxEnabled = true;

  /* Video defaults */
  config->video.windowWidth = WINDOWED_MODE_WIDTH;
  config->video.windowHeight = WINDOWED_MODE_HEIGHT;
  config->video.fullscreen = false;
  config->video.vsync = true;
  config->video.quality = GRAPHICS_MEDIUM;
  config->video.targetFPS = 60;

  /* Gameplay defaults */
  config->gameplay.playerSpeed = 1.0f;
  config->gameplay.enemySpeed = 1.0f;
  config->gameplay.bulletSpeed = 1.0f;
  config->gameplay.startingHealth = 100;
  config->gameplay.difficultyScale = 1.0f;
  config->gameplay.showFPS = false;
  config->gameplay.showDebugInfo = false;
}

/**
 * Helper to parse a float value from a config line.
 */
static int parseConfigFloat(const char *line, const char *key, float *value) {
  const char *found = strstr(line, key);
  if (!found)
    return -1;
  found = strchr(found, '=');
  if (!found)
    return -1;
  *value = strtof(found + 1, NULL);
  return 0;
}

/**
 * Helper to parse an int value from a config line.
 */
static int parseConfigInt(const char *line, const char *key, int *value) {
  const char *found = strstr(line, key);
  if (!found)
    return -1;
  found = strchr(found, '=');
  if (!found)
    return -1;
  *value = (int)strtol(found + 1, NULL, 10);
  return 0;
}

/**
 * Helper to parse a bool value from a config line.
 */
static int parseConfigBool(const char *line, const char *key, bool *value) {
  const char *found = strstr(line, key);
  if (!found)
    return -1;
  found = strchr(found, '=');
  if (!found)
    return -1;
  found++;
  while (*found == ' ')
    found++;
  *value = (strncmp(found, "true", 4) == 0 || *found == '1');
  return 0;
}

int configLoad(GameConfig *config, const char *filePath) {
  if (!config || !filePath)
    return -1;

  /* Start with defaults */
  configInit(config);

  FILE *file = fopen(filePath, "r");
  if (!file) {
    /* File doesn't exist, use defaults (not an error) */
    return 0;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    /* Skip comments and empty lines */
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
      continue;

    /* Audio settings */
    parseConfigFloat(line, "master_volume", &config->audio.masterVolume);
    parseConfigFloat(line, "music_volume", &config->audio.musicVolume);
    parseConfigFloat(line, "sfx_volume", &config->audio.sfxVolume);
    parseConfigBool(line, "music_enabled", &config->audio.musicEnabled);
    parseConfigBool(line, "sfx_enabled", &config->audio.sfxEnabled);

    /* Video settings */
    parseConfigInt(line, "window_width", &config->video.windowWidth);
    parseConfigInt(line, "window_height", &config->video.windowHeight);
    parseConfigBool(line, "fullscreen", &config->video.fullscreen);
    parseConfigBool(line, "vsync", &config->video.vsync);
    parseConfigInt(line, "target_fps", &config->video.targetFPS);

    /* Gameplay settings */
    parseConfigFloat(line, "player_speed", &config->gameplay.playerSpeed);
    parseConfigFloat(line, "enemy_speed", &config->gameplay.enemySpeed);
    parseConfigFloat(line, "bullet_speed", &config->gameplay.bulletSpeed);
    parseConfigInt(line, "starting_health", &config->gameplay.startingHealth);
    parseConfigFloat(line, "difficulty_scale",
                     &config->gameplay.difficultyScale);
    parseConfigBool(line, "show_fps", &config->gameplay.showFPS);
    parseConfigBool(line, "show_debug_info", &config->gameplay.showDebugInfo);
  }

  fclose(file);
  return 0;
}

int configSave(const GameConfig *config, const char *filePath) {
  if (!config || !filePath)
    return -1;

  FILE *file = fopen(filePath, "w");
  if (!file)
    return -1;

  fprintf(file, "# Jubilant Rifle Configuration\n\n");

  fprintf(file, "# Audio Settings\n");
  fprintf(file, "master_volume = %.2f\n", config->audio.masterVolume);
  fprintf(file, "music_volume = %.2f\n", config->audio.musicVolume);
  fprintf(file, "sfx_volume = %.2f\n", config->audio.sfxVolume);
  fprintf(file, "music_enabled = %s\n",
          config->audio.musicEnabled ? "true" : "false");
  fprintf(file, "sfx_enabled = %s\n",
          config->audio.sfxEnabled ? "true" : "false");

  fprintf(file, "\n# Video Settings\n");
  fprintf(file, "window_width = %d\n", config->video.windowWidth);
  fprintf(file, "window_height = %d\n", config->video.windowHeight);
  fprintf(file, "fullscreen = %s\n",
          config->video.fullscreen ? "true" : "false");
  fprintf(file, "vsync = %s\n", config->video.vsync ? "true" : "false");
  fprintf(file, "target_fps = %d\n", config->video.targetFPS);

  fprintf(file, "\n# Gameplay Settings\n");
  fprintf(file, "player_speed = %.2f\n", config->gameplay.playerSpeed);
  fprintf(file, "enemy_speed = %.2f\n", config->gameplay.enemySpeed);
  fprintf(file, "bullet_speed = %.2f\n", config->gameplay.bulletSpeed);
  fprintf(file, "starting_health = %d\n", config->gameplay.startingHealth);
  fprintf(file, "difficulty_scale = %.2f\n", config->gameplay.difficultyScale);
  fprintf(file, "show_fps = %s\n",
          config->gameplay.showFPS ? "true" : "false");
  fprintf(file, "show_debug_info = %s\n",
          config->gameplay.showDebugInfo ? "true" : "false");

  fclose(file);
  return 0;
}

GameConfig *configGetGlobal(void) {
  if (!g_configInitialized) {
    configInit(&g_config);
    g_configInitialized = true;
  }
  return &g_config;
}

int configApplyVideo(const VideoConfig *config) {
  (void)config;
  /* Video changes would be applied here when SDL is available */
  return 0;
}

int configApplyAudio(const AudioConfig *config) {
  (void)config;
  /* Audio changes would be applied here when SDL_mixer is available */
  return 0;
}
