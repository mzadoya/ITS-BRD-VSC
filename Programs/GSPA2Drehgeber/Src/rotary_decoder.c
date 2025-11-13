#include "rotary_decoder.h"
#include "fehler.h"
#include "global.h"
#include "stm32f429xx.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>

#define MASK_PIN01F 0x03U
#define TIME_WINDOW_MS 250
#define INACTIVITY_TIMEOUT 10000000
#define TIMER_FREQUENCY 90000000.0
static uint16_t lastPhase = 0;
static uint32_t time1 = 0;
static uint32_t lastActivity = 0;
int32_t transitionCount = 0;
double speed = 0.0;
double angle = 0.0;
static uint32_t currentTransition = 0;

void initDecoder() {
  lastPhase = GPIOF->IDR & MASK_PIN01F;
  transitionCount = 0;
  currentTransition = 0;
  time1 = TIM2->CNT;
  lastActivity = TIM2->CNT;
}

int getPhase(uint16_t *direction, uint16_t result) {

  if (lastPhase == PHASE_B) {

    if (result == PHASE_A) {
      // rueckwaertslauf
      *direction = BACKWARD;
    } else if (result == PHASE_C) {
      // vorwaerts
      *direction = FORWARD;
    } else if (result == PHASE_B) {
      // stehen geblieben
      *direction = IDLE;
    } else if (result == PHASE_D) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_A) {
    if (result == PHASE_B) {
      // vorwaerts
      *direction = FORWARD;
    } else if (result == PHASE_D) {
      // rueckwaerts
      *direction = BACKWARD;
    } else if (result == PHASE_A) {
      // stehen geblieben
      *direction = IDLE;
    } else if (result == PHASE_C) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_C) {
    if (result == PHASE_D) {
      // voerwaerts
      *direction = FORWARD;
    } else if (result == PHASE_B) {
      // rueckwaerts
      *direction = BACKWARD;
    } else if (result == PHASE_C) {
      // stehen geblieben
      *direction = IDLE;
    } else if (result == PHASE_A) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_D) {
    if (result == PHASE_A) {
      // voerwaerts
      *direction = FORWARD;
    } else if (result == PHASE_C) {
      // rueckwaers
      *direction = BACKWARD;
    } else if (result == PHASE_D) {
      // stehen geblieben
      *direction = IDLE;
    } else if (result == PHASE_B) {
      return ENCODER_ERROR;
    }
  }

  return ENCODER_CHANGED;
}

int ecoderUpdate(uint16_t eingabe, uint16_t *direction, uint32_t time2) {
  uint8_t result = eingabe & MASK_PIN01F;
  int rc = ENCODER_NO_CHANGE;
  if (result != lastPhase) {
    currentTransition++;
    rc = getPhase(direction, result);
    if(*direction == BACKWARD)
    {
      transitionCount--; // fuer LEDS
    }
    else if(*direction == FORWARD)
    {
      transitionCount++;
    }
    
    angle = transitionCount * 0.3;
      
    lastPhase = result;
  }

  if (time2 - time1 >= TIME_WINDOW_MS * 1000 * TICKS_PER_US) {
    double localAngle = currentTransition * 0.3;
    double timeSec = (time2 - time1) / TIMER_FREQUENCY;
    if (timeSec == 0) {
      return ENCODER_ERROR; // TODO ERR
    }
    speed = localAngle / timeSec; // 360/1200
    time1 = time2;
    currentTransition = 0;
    return ENCODER_TIME_UPDATED;
  }

  return rc;
}