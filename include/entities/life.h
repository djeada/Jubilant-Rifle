#ifndef LIFE_H
#define LIFE_H

#include <stdbool.h>

typedef struct {
    short current;
    short full;
} Life;

void lifeConstructor(Life* life, short full);
void lifeCopyConstructor(Life* destination, const Life* source);
void lifeTakeDamage(Life* life, short damage);
bool lifeIsAlive(Life* life);
void lifeDie(Life* life);
void lifeHeal(Life* life, short amount);
void lifeLevelUp(Life* life, short increaseAmount);

#endif // LIFE_H
