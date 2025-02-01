#include "utils/time_manager.h"

// The singleton instance of TimeManager as a static variable.
static struct {
  int globalTime;
} timeManager;

void timeManagerInit() { timeManager.globalTime = 0; }

void timeManagerUpdate() { timeManager.globalTime++; }

void timeManagerReset() { timeManager.globalTime = 0; }

int timeManagerGetTime() { return timeManager.globalTime; }
