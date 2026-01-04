#ifndef SCORE_H
#define SCORE_H

/**
 * @file score.h
 * @brief Score tracking and high score management.
 *
 * Provides functionality for tracking player scores, managing high scores,
 * and persisting scores between game sessions.
 */

#include <stdbool.h>
#include <stddef.h>

/* Maximum number of high scores to track */
#define MAX_HIGH_SCORES 10

/* Maximum length for player names */
#define MAX_PLAYER_NAME 32

/**
 * @brief Represents a single high score entry.
 */
typedef struct {
  char playerName[MAX_PLAYER_NAME];
  int score;
  int level;
  int enemiesKilled;
} HighScoreEntry;

/**
 * @brief High score table.
 */
typedef struct {
  HighScoreEntry entries[MAX_HIGH_SCORES];
  size_t count;
} HighScoreTable;

/**
 * @brief Current game session score tracking.
 */
typedef struct {
  int currentScore;      /**< Current score in this session */
  int enemiesKilled;     /**< Total enemies killed */
  int bulletsHit;        /**< Total bullets that hit targets */
  int bulletsFired;      /**< Total bullets fired */
  int damageTaken;       /**< Total damage taken */
  int highestCombo;      /**< Highest combo achieved */
  int currentCombo;      /**< Current combo count */
  float comboTimer;      /**< Time remaining in current combo */
  int levelReached;      /**< Highest level reached */
} ScoreTracker;

/**
 * @brief Initialize a score tracker with default values.
 * @param tracker The tracker to initialize.
 */
void scoreTrackerInit(ScoreTracker *tracker);

/**
 * @brief Add points to the score.
 * @param tracker The score tracker.
 * @param points Points to add.
 */
void scoreAddPoints(ScoreTracker *tracker, int points);

/**
 * @brief Register an enemy kill.
 * @param tracker The score tracker.
 * @param basePoints Base points for the kill (modified by combo).
 */
void scoreRegisterKill(ScoreTracker *tracker, int basePoints);

/**
 * @brief Register a bullet hit.
 * @param tracker The score tracker.
 */
void scoreRegisterHit(ScoreTracker *tracker);

/**
 * @brief Register a bullet fired.
 * @param tracker The score tracker.
 */
void scoreRegisterShot(ScoreTracker *tracker);

/**
 * @brief Register damage taken by the player.
 * @param tracker The score tracker.
 * @param damage Amount of damage taken.
 */
void scoreRegisterDamage(ScoreTracker *tracker, int damage);

/**
 * @brief Update the combo timer.
 * @param tracker The score tracker.
 * @param dt Delta time since last update.
 */
void scoreUpdateCombo(ScoreTracker *tracker, float dt);

/**
 * @brief Calculate accuracy as a percentage.
 * @param tracker The score tracker.
 * @return Accuracy percentage (0-100).
 */
float scoreGetAccuracy(const ScoreTracker *tracker);

/**
 * @brief Reset the score tracker for a new game.
 * @param tracker The score tracker to reset.
 */
void scoreReset(ScoreTracker *tracker);

/**
 * @brief Initialize high score table.
 * @param table The high score table to initialize.
 */
void highScoreTableInit(HighScoreTable *table);

/**
 * @brief Load high scores from a file.
 * @param table The table to populate.
 * @param filePath Path to the high scores file.
 * @return 0 on success, -1 on failure.
 */
int highScoreTableLoad(HighScoreTable *table, const char *filePath);

/**
 * @brief Save high scores to a file.
 * @param table The table to save.
 * @param filePath Path to the output file.
 * @return 0 on success, -1 on failure.
 */
int highScoreTableSave(const HighScoreTable *table, const char *filePath);

/**
 * @brief Check if a score qualifies as a high score.
 * @param table The high score table.
 * @param score The score to check.
 * @return true if it's a high score, false otherwise.
 */
bool highScoreIsHighScore(const HighScoreTable *table, int score);

/**
 * @brief Add a new high score entry.
 * @param table The high score table.
 * @param entry The entry to add.
 * @return The position in the table (0 = highest), or -1 if not added.
 */
int highScoreAddEntry(HighScoreTable *table, const HighScoreEntry *entry);

/**
 * @brief Get the global score tracker instance.
 * @return Pointer to the global score tracker.
 */
ScoreTracker *scoreGetGlobal(void);

#endif /* SCORE_H */
