/**
 ******************************************************************************
 * @file    rotary_decoder.c
 * @author  Maksym Zadoya
 * @brief   Auswertung eines inkrementellen Drehgebers.
 *          Ermittelt Drehrichtung, Drehwinkel und Winkelgeschwindigkeit
 *          anhand der Phasenwechsel und Zeitmessung.
 * @date    2025/11/20
 * @version 1.2 vom 2025/12/29
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

// Standard C Libs
#include <stdbool.h>
#include <stdint.h>

// Board Libs
#include "stm32f429xx.h"

// App modules
#include "timer.h"
#include "rotary_decoder.h"
#include "fehler.h"
#include "global.h"

#define MASK_PIN01F 0x03U
#define TIME_WINDOW_MS 250
#define INACTIVITY_TIMEOUT 500
#define TIMER_FREQUENCY 90000000.0

static uint16_t lastPhase = 0;
static uint32_t time1 = 0;
static uint32_t lastActivity = 0;
static uint32_t currentTransition = 0;
int32_t transitionCount = 0;

void initDecoder() {
  lastPhase = GPIOF->IDR & MASK_PIN01F;
  transitionCount = 0;
  currentTransition = 0;
  time1 = TIM2->CNT;
  lastActivity = TIM2->CNT;
}

/**
 * @brief Bestimmt die Drehrichtung.
 * 
 * @param direction Zeiger auf Drehrichtung 
 * @param phase Aktulle Phase des Drehgebers 
 */
static int getPhase(uint16_t *direction, uint16_t phase) {
  if (lastPhase == PHASE_B) {

    if (phase == PHASE_A) {
      // rueckwaertslauf
      *direction = BACKWARD;
    } else if (phase == PHASE_C) {
      // vorwaerts
      *direction = FORWARD;
    } else if (phase == PHASE_B) {
      // stehen geblieben
      *direction = IDLE;
    } else if (phase == PHASE_D) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_A) {
    if (phase == PHASE_B) {
      // vorwaerts
      *direction = FORWARD;
    } else if (phase == PHASE_D) {
      // rueckwaerts
      *direction = BACKWARD;
    } else if (phase == PHASE_A) {
      // stehen geblieben
      *direction = IDLE;
    } else if (phase == PHASE_C) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_C) {
    if (phase == PHASE_D) {
      // voerwaerts
      *direction = FORWARD;
    } else if (phase == PHASE_B) {
      // rueckwaerts
      *direction = BACKWARD;
    } else if (phase == PHASE_C) {
      // stehen geblieben
      *direction = IDLE;
    } else if (phase == PHASE_A) {
      return ENCODER_ERROR;
    }
  }

  else if (lastPhase == PHASE_D) {
    if (phase == PHASE_A) {
      // voerwaerts
      *direction = FORWARD;
    } else if (phase == PHASE_C) {
      // rueckwaers
      *direction = BACKWARD;
    } else if (phase == PHASE_D) {
      // stehen geblieben
      *direction = IDLE;
    } else if (phase == PHASE_B) {
      return ENCODER_ERROR;
    }
  }

  return ENCODER_CHANGED;
}

int encoderUpdate(uint16_t gpioState, uint16_t *direction, uint32_t time2, double *angle, double *speed) {
  uint8_t phase = gpioState & MASK_PIN01F;
  int rc = ENCODER_NO_CHANGE;
  if (phase != lastPhase) {
    currentTransition++;
    rc = getPhase(direction, phase);
    if (*direction == BACKWARD) {
      transitionCount--;
    } else if (*direction == FORWARD) {
      transitionCount++;
    }
    (*angle) = transitionCount * 0.3;

    lastPhase = phase;

    if (time2 - time1 >= TIME_WINDOW_MS * 1000 * TICKS_PER_US) {
      double localAngle = currentTransition * 0.3;
      double timeSec = (time2 - time1) / TIMER_FREQUENCY;
      if (timeSec == 0) {
        return ENCODER_ERROR;
      }
      (*speed) = localAngle / timeSec;
      time1 = time2;
      currentTransition = 0;
      return ENCODER_TIME_UPDATED;
    }
  }
  else if (time2 - time1 >= INACTIVITY_TIMEOUT * 1000 * TICKS_PER_US && phase == lastPhase) {
      (*speed) = 0;
      time1 = time2;
      currentTransition = 0;
      return ENCODER_TIME_UPDATED;
    }

  return rc;
}

//EOF