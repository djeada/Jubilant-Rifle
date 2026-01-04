#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Game configuration system for runtime settings.
 *
 * Provides a centralized configuration system that allows game settings
 * to be loaded from files and modified at runtime, making the game
 * more extensible and user-friendly.
 */

#include <stdbool.h>

/**
 * @brief Graphics quality presets.
 */
typedef enum {
  GRAPHICS_LOW,
  GRAPHICS_MEDIUM,
  GRAPHICS_HIGH
} GraphicsQuality;

/**
 * @brief Audio configuration settings.
 */
typedef struct {
  float masterVolume;   /**< Master volume (0.0 - 1.0) */
  float musicVolume;    /**< Music volume (0.0 - 1.0) */
  float sfxVolume;      /**< Sound effects volume (0.0 - 1.0) */
  bool musicEnabled;    /**< Whether music is enabled */
  bool sfxEnabled;      /**< Whether sound effects are enabled */
} AudioConfig;

/**
 * @brief Video/display configuration settings.
 */
typedef struct {
  int windowWidth;          /**< Window width in pixels */
  int windowHeight;         /**< Window height in pixels */
  bool fullscreen;          /**< Whether to run in fullscreen */
  bool vsync;               /**< Whether VSync is enabled */
  GraphicsQuality quality;  /**< Graphics quality preset */
  int targetFPS;            /**< Target frames per second */
} VideoConfig;

/**
 * @brief Gameplay configuration settings.
 */
typedef struct {
  float playerSpeed;        /**< Player movement speed multiplier */
  float enemySpeed;         /**< Enemy movement speed multiplier */
  float bulletSpeed;        /**< Bullet speed multiplier */
  int startingHealth;       /**< Player starting health */
  float difficultyScale;    /**< Overall difficulty multiplier */
  bool showFPS;             /**< Whether to display FPS counter */
  bool showDebugInfo;       /**< Whether to show debug information */
} GameplayConfig;

/**
 * @brief Complete game configuration.
 */
typedef struct {
  AudioConfig audio;
  VideoConfig video;
  GameplayConfig gameplay;
} GameConfig;

/**
 * @brief Initialize configuration with default values.
 * @param config The configuration to initialize.
 */
void configInit(GameConfig *config);

/**
 * @brief Load configuration from a file.
 * @param config The configuration to populate.
 * @param filePath Path to the configuration file.
 * @return 0 on success, -1 on failure.
 */
int configLoad(GameConfig *config, const char *filePath);

/**
 * @brief Save configuration to a file.
 * @param config The configuration to save.
 * @param filePath Path to the output file.
 * @return 0 on success, -1 on failure.
 */
int configSave(const GameConfig *config, const char *filePath);

/**
 * @brief Get the global configuration instance.
 * @return Pointer to the global configuration.
 */
GameConfig *configGetGlobal(void);

/**
 * @brief Apply video configuration changes.
 * @param config The video configuration to apply.
 * @return 0 on success, -1 on failure.
 */
int configApplyVideo(const VideoConfig *config);

/**
 * @brief Apply audio configuration changes.
 * @param config The audio configuration to apply.
 * @return 0 on success, -1 on failure.
 */
int configApplyAudio(const AudioConfig *config);

#endif /* CONFIG_H */
