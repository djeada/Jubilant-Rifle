#include "map/trap.h"
#include <stdio.h>
#include <string.h>

#define TRAP_INITIAL_CAPACITY 8
#define FUEL_BARREL_HEALTH 50
#define FUEL_BARREL_DAMAGE 75
#define FUEL_BARREL_RADIUS 100.0f
#define FUEL_BARREL_DURATION 3.0f
#define SPIKE_DAMAGE 25
#define ELECTRIC_DAMAGE 15
#define ELECTRIC_INTERVAL 0.5f
#define FIRE_DAMAGE 10
#define FIRE_INTERVAL 0.3f

void trapArrayInit(TrapArray *arr) {
  if (!arr)
    return;

  arr->traps = NULL;
  arr->count = 0;
  arr->capacity = 0;
}

void trapArrayDestroy(TrapArray *arr) {
  if (!arr)
    return;

  free(arr->traps);
  arr->traps = NULL;
  arr->count = 0;
  arr->capacity = 0;
}

int trapArrayAdd(TrapArray *arr, const Trap *trap) {
  if (!arr || !trap)
    return -1;

  /* Grow array if needed */
  if (arr->count >= arr->capacity) {
    size_t newCapacity = (arr->capacity == 0) ? TRAP_INITIAL_CAPACITY 
                                               : arr->capacity * 2;
    Trap *newTraps = realloc(arr->traps, newCapacity * sizeof(Trap));
    if (!newTraps)
      return -1;
    
    arr->traps = newTraps;
    arr->capacity = newCapacity;
  }

  arr->traps[arr->count] = *trap;
  arr->count++;
  return 0;
}

void trapArrayUpdate(TrapArray *arr, float dt) {
  if (!arr)
    return;

  for (size_t i = 0; i < arr->count; i++) {
    Trap *trap = &arr->traps[i];
    
    switch (trap->state) {
    case TRAP_STATE_TRIGGERED:
      /* Update effect timer for one-time triggered traps */
      trap->effectTimer -= dt;
      if (trap->effectTimer <= 0) {
        trap->state = TRAP_STATE_DESTROYED;
      }
      break;
      
    case TRAP_STATE_ACTIVE:
      /* Update damage timer for continuous traps */
      trap->damageTimer -= dt;
      if (trap->damageTimer <= 0) {
        trap->damageTimer = trap->damageInterval;
      }
      break;
      
    default:
      break;
    }
  }
}

int trapArrayCheckDamage(TrapArray *arr, const SDL_Rect *entityRect) {
  if (!arr || !entityRect)
    return 0;

  int totalDamage = 0;

  for (size_t i = 0; i < arr->count; i++) {
    Trap *trap = &arr->traps[i];
    
    if (trap->state == TRAP_STATE_DESTROYED)
      continue;

    bool inRange = false;
    
    if (trap->state == TRAP_STATE_TRIGGERED && trap->effectRadius > 0) {
      /* Check explosion radius */
      int centerX = trap->rect.x + trap->rect.w / 2;
      int centerY = trap->rect.y + trap->rect.h / 2;
      int entityCenterX = entityRect->x + entityRect->w / 2;
      int entityCenterY = entityRect->y + entityRect->h / 2;
      
      int dx = centerX - entityCenterX;
      int dy = centerY - entityCenterY;
      float dist = (float)(dx * dx + dy * dy);
      float radius = trap->effectRadius;
      
      inRange = dist <= (radius * radius);
    } else {
      /* Check direct collision */
      inRange = SDL_HasIntersection(&trap->rect, entityRect);
    }
    
    if (inRange) {
      switch (trap->type) {
      case TRAP_TYPE_FUEL_BARREL:
        if (trap->state == TRAP_STATE_TRIGGERED) {
          totalDamage += trap->damage;
        }
        break;
        
      case TRAP_TYPE_SPIKE_PIT:
        totalDamage += trap->damage;
        break;
        
      case TRAP_TYPE_ELECTRIC_FENCE:
      case TRAP_TYPE_FIRE_GRATE:
        if (trap->damageTimer <= 0) {
          totalDamage += trap->damage;
          trap->damageTimer = trap->damageInterval;
        }
        trap->state = TRAP_STATE_ACTIVE;
        break;
      }
    }
  }

  return totalDamage;
}

bool trapArrayTriggerAt(TrapArray *arr, float x, float y) {
  if (!arr)
    return false;

  SDL_Point point = {(int)x, (int)y};
  
  for (size_t i = 0; i < arr->count; i++) {
    Trap *trap = &arr->traps[i];
    
    if (trap->state != TRAP_STATE_INACTIVE)
      continue;

    if (SDL_PointInRect(&point, &trap->rect)) {
      /* Only fuel barrels can be triggered by shooting */
      if (trap->type == TRAP_TYPE_FUEL_BARREL) {
        trap->state = TRAP_STATE_TRIGGERED;
        trap->effectTimer = trap->effectDuration;
        return true;
      }
    }
  }

  return false;
}

bool trapArrayDamageTrap(TrapArray *arr, size_t trapIndex, int damage) {
  if (!arr || trapIndex >= arr->count)
    return false;

  Trap *trap = &arr->traps[trapIndex];
  
  if (trap->health <= 0)
    return false;

  trap->health -= damage;
  
  if (trap->health <= 0) {
    trap->state = TRAP_STATE_TRIGGERED;
    trap->effectTimer = trap->effectDuration;
    return true;
  }
  
  return false;
}

void trapArrayReset(TrapArray *arr) {
  if (!arr)
    return;

  for (size_t i = 0; i < arr->count; i++) {
    Trap *trap = &arr->traps[i];
    trap->state = TRAP_STATE_INACTIVE;
    trap->damageTimer = 0;
    trap->effectTimer = 0;
    
    /* Reset health based on type */
    if (trap->type == TRAP_TYPE_FUEL_BARREL) {
      trap->health = FUEL_BARREL_HEALTH;
    }
  }
}

int countTrapsInJson(const char *json) {
  if (!json)
    return 0;

  const char *start = strstr(json, "\"traps\"");
  if (!start)
    return 0;

  start = strchr(start, '[');
  if (!start)
    return 0;

  const char *end = strchr(start, ']');
  if (!end)
    return 0;

  /* Count '{' characters between '[' and ']' */
  int count = 0;
  const char *p = start;
  while (p < end) {
    if (*p == '{')
      count++;
    p++;
  }

  return count;
}

int parseTraps(const char *json, TrapArray *arr) {
  if (!json || !arr)
    return -1;

  const char *start = strstr(json, "\"traps\"");
  if (!start)
    return 0; /* No traps section is not an error */

  start = strchr(start, '[');
  if (!start)
    return -1;
  start++; /* Move past '[' */

  int trapCount = countTrapsInJson(json);
  if (trapCount <= 0)
    return 0;

  for (int i = 0; i < trapCount; i++) {
    /* Skip whitespace and commas */
    while (*start && (*start == ' ' || *start == '\n' || *start == ',' || 
           *start == '\t' || *start == '\r'))
      start++;
    
    if (*start != '{')
      return -1;

    Trap trap = {0};
    trap.state = TRAP_STATE_INACTIVE;
    trap.type = TRAP_TYPE_FUEL_BARREL; /* Default type */
    trap.damage = FUEL_BARREL_DAMAGE;
    trap.health = FUEL_BARREL_HEALTH;
    trap.effectRadius = FUEL_BARREL_RADIUS;
    trap.effectDuration = FUEL_BARREL_DURATION;
    trap.damageInterval = FIRE_INTERVAL;
    trap.damageTimer = 0;
    trap.effectTimer = 0;
    
    int charsRead = 0;
    int typeValue = 0;
    
    /* Try to parse with type */
    if (sscanf(start, " { \"x\" : %d , \"y\" : %d , \"width\" : %d , \"height\" : %d , \"type\" : %d } %n",
               &trap.rect.x, &trap.rect.y, &trap.rect.w, &trap.rect.h, 
               &typeValue, &charsRead) == 5) {
      trap.type = (TrapType)typeValue;
      
      /* Set properties based on type */
      switch (trap.type) {
      case TRAP_TYPE_SPIKE_PIT:
        trap.damage = SPIKE_DAMAGE;
        trap.health = 0;
        trap.effectRadius = 0;
        break;
      case TRAP_TYPE_ELECTRIC_FENCE:
        trap.damage = ELECTRIC_DAMAGE;
        trap.damageInterval = ELECTRIC_INTERVAL;
        trap.health = 0;
        trap.effectRadius = 0;
        break;
      case TRAP_TYPE_FIRE_GRATE:
        trap.damage = FIRE_DAMAGE;
        trap.damageInterval = FIRE_INTERVAL;
        trap.health = 0;
        trap.effectRadius = 0;
        break;
      default:
        break;
      }
      
      trapArrayAdd(arr, &trap);
      start += charsRead;
    } else if (sscanf(start, " { \"x\" : %d , \"y\" : %d , \"width\" : %d , \"height\" : %d } %n",
               &trap.rect.x, &trap.rect.y, &trap.rect.w, &trap.rect.h, 
               &charsRead) == 4) {
      trapArrayAdd(arr, &trap);
      start += charsRead;
    } else {
      return -1;
    }
  }

  return 0;
}
