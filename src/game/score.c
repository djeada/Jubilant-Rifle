#include "game/score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Combo settings */
#define COMBO_DURATION 3.0f     /* Seconds before combo resets */
#define COMBO_MULTIPLIER 0.1f   /* Additional multiplier per combo level */
#define MAX_COMBO_MULTIPLIER 5.0f

/* Global score tracker instance */
static ScoreTracker g_scoreTracker;
static bool g_scoreInitialized = false;

void scoreTrackerInit(ScoreTracker *tracker) {
  if (!tracker)
    return;

  tracker->currentScore = 0;
  tracker->enemiesKilled = 0;
  tracker->bulletsHit = 0;
  tracker->bulletsFired = 0;
  tracker->damageTaken = 0;
  tracker->highestCombo = 0;
  tracker->currentCombo = 0;
  tracker->comboTimer = 0;
  tracker->levelReached = 1;
  tracker->elapsedTime = 0.0f;
  tracker->flagsCaptured = 0;
  tracker->meleeKills = 0;
  tracker->grenadeKills = 0;
}

void scoreAddPoints(ScoreTracker *tracker, int points) {
  if (!tracker || points <= 0)
    return;

  /* Apply combo multiplier */
  float multiplier = 1.0f + (tracker->currentCombo * COMBO_MULTIPLIER);
  if (multiplier > MAX_COMBO_MULTIPLIER) {
    multiplier = MAX_COMBO_MULTIPLIER;
  }

  int adjustedPoints = (int)(points * multiplier);
  tracker->currentScore += adjustedPoints;
}

void scoreRegisterKill(ScoreTracker *tracker, int basePoints) {
  if (!tracker)
    return;

  tracker->enemiesKilled++;
  tracker->currentCombo++;
  tracker->comboTimer = COMBO_DURATION;

  /* Update highest combo */
  if (tracker->currentCombo > tracker->highestCombo) {
    tracker->highestCombo = tracker->currentCombo;
  }

  /* Add points with combo multiplier */
  scoreAddPoints(tracker, basePoints);
}

void scoreRegisterHit(ScoreTracker *tracker) {
  if (!tracker)
    return;
  tracker->bulletsHit++;
}

void scoreRegisterShot(ScoreTracker *tracker) {
  if (!tracker)
    return;
  tracker->bulletsFired++;
}

void scoreRegisterDamage(ScoreTracker *tracker, int damage) {
  if (!tracker || damage <= 0)
    return;

  tracker->damageTaken += damage;

  /* Taking damage breaks the combo */
  tracker->currentCombo = 0;
  tracker->comboTimer = 0;
}

void scoreUpdateCombo(ScoreTracker *tracker, float dt) {
  if (!tracker)
    return;

  if (tracker->comboTimer > 0) {
    tracker->comboTimer -= dt;
    if (tracker->comboTimer <= 0) {
      tracker->currentCombo = 0;
      tracker->comboTimer = 0;
    }
  }
}

float scoreGetAccuracy(const ScoreTracker *tracker) {
  if (!tracker || tracker->bulletsFired == 0)
    return 0.0f;

  return (float)tracker->bulletsHit / (float)tracker->bulletsFired * 100.0f;
}

void scoreReset(ScoreTracker *tracker) {
  scoreTrackerInit(tracker);
}

void scoreUpdateElapsedTime(ScoreTracker *tracker, float dt) {
  if (!tracker)
    return;
  tracker->elapsedTime += dt;
}

void scoreGetElapsedTimeString(const ScoreTracker *tracker, char *buffer, size_t bufferSize) {
  if (!tracker || !buffer || bufferSize == 0)
    return;
  
  int totalSeconds = (int)tracker->elapsedTime;
  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;
  snprintf(buffer, bufferSize, "%02d:%02d", minutes, seconds);
}

void scoreRegisterFlagCapture(ScoreTracker *tracker, int basePoints) {
  if (!tracker)
    return;
  
  tracker->flagsCaptured++;
  scoreAddPoints(tracker, basePoints);
}

void scoreRegisterMeleeKill(ScoreTracker *tracker, int basePoints) {
  if (!tracker)
    return;
  
  tracker->meleeKills++;
  scoreRegisterKill(tracker, basePoints);
}

void scoreRegisterGrenadeKill(ScoreTracker *tracker, int basePoints) {
  if (!tracker)
    return;
  
  tracker->grenadeKills++;
  scoreRegisterKill(tracker, basePoints);
}

void highScoreTableInit(HighScoreTable *table) {
  if (!table)
    return;

  memset(table->entries, 0, sizeof(table->entries));
  table->count = 0;
}

int highScoreTableLoad(HighScoreTable *table, const char *filePath) {
  if (!table || !filePath)
    return -1;

  highScoreTableInit(table);

  FILE *file = fopen(filePath, "r");
  if (!file) {
    /* File doesn't exist, use empty table (not an error) */
    return 0;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) && table->count < MAX_HIGH_SCORES) {
    HighScoreEntry *entry = &table->entries[table->count];

    /* Parse line: "name,score,level,kills" */
    char *token = strtok(line, ",");
    if (!token)
      continue;
    strncpy(entry->playerName, token, MAX_PLAYER_NAME - 1);
    entry->playerName[MAX_PLAYER_NAME - 1] = '\0';

    token = strtok(NULL, ",");
    if (!token)
      continue;
    entry->score = atoi(token);

    token = strtok(NULL, ",");
    if (!token)
      continue;
    entry->level = atoi(token);

    token = strtok(NULL, ",\n\r");
    if (!token)
      continue;
    entry->enemiesKilled = atoi(token);

    table->count++;
  }

  fclose(file);
  return 0;
}

int highScoreTableSave(const HighScoreTable *table, const char *filePath) {
  if (!table || !filePath)
    return -1;

  FILE *file = fopen(filePath, "w");
  if (!file)
    return -1;

  for (size_t i = 0; i < table->count; i++) {
    const HighScoreEntry *entry = &table->entries[i];
    fprintf(file, "%s,%d,%d,%d\n", entry->playerName, entry->score,
            entry->level, entry->enemiesKilled);
  }

  fclose(file);
  return 0;
}

bool highScoreIsHighScore(const HighScoreTable *table, int score) {
  if (!table)
    return false;

  /* If table isn't full, any score qualifies */
  if (table->count < MAX_HIGH_SCORES)
    return true;

  /* Check if score beats the lowest entry */
  return score > table->entries[table->count - 1].score;
}

int highScoreAddEntry(HighScoreTable *table, const HighScoreEntry *entry) {
  if (!table || !entry)
    return -1;

  /* Find insertion position */
  size_t insertPos = 0;
  while (insertPos < table->count &&
         entry->score <= table->entries[insertPos].score) {
    insertPos++;
  }

  /* If score doesn't qualify, don't add */
  if (insertPos >= MAX_HIGH_SCORES)
    return -1;

  /* Shift entries down */
  if (table->count < MAX_HIGH_SCORES) {
    table->count++;
  }
  for (size_t i = table->count - 1; i > insertPos; i--) {
    table->entries[i] = table->entries[i - 1];
  }

  /* Insert new entry */
  table->entries[insertPos] = *entry;

  return (int)insertPos;
}

ScoreTracker *scoreGetGlobal(void) {
  if (!g_scoreInitialized) {
    scoreTrackerInit(&g_scoreTracker);
    g_scoreInitialized = true;
  }
  return &g_scoreTracker;
}
