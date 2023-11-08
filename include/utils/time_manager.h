#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

// Initializes the TimeManager singleton.
void timeManagerInit();

// Updates the global time within the TimeManager.
void timeManagerUpdate();

// Resets the global time to zero.
void timeManagerReset();

// Retrieves the current global time.
int timeManagerGetTime();

#endif // TIME_MANAGER_H
