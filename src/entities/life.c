#include "entities/life.h"

void lifeConstructor(Life *life, short full) {
  life->current = full;
  life->full = full;
}

void lifeCopyConstructor(Life *destination, const Life *source) {
  destination->current = source->current;
  destination->full = source->full;
}

void lifeTakeDamage(Life *life, short damage) {
  life->current -= damage;
  if (life->current < 0) {
    life->current = 0;
  }
}

bool lifeIsAlive(Life *life) { return life->current > 0; }

void lifeDie(Life *life) { life->current = 0; }

void lifeHeal(Life *life, short amount) {
  life->current += amount;
  if (life->current > life->full) {
    life->current = life->full;
  }
}

void lifeLevelUp(Life *life, short increaseAmount) {
  life->full += increaseAmount;
  life->current = life->full; // Regenerate to full health on level up
}
