/**
 ******************************************************************************
 * @file    encoder_interrupt.c
 * @author  Maksym Zadoya
 * @brief   Auswertung eines inkrementellen Drehgebers mittels Interrupt.
 *          Ermittelt Drehrichtung, Drehwinkel und Winkelgeschwindigkeit
 *          anhand der Phasenwechsel und Zeitmessung.
 * @date    2025/12/30
 * @version 1.0 vom 2025/12/30
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "fehler.h"
#include "global.h"
#include "rotary_decoder.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

#include "encoder_interrupt.h"
#include "global.h"
#include "timer.h"
#include <stdint.h>
#include "display.h"

#define MODER_MASK_PIN_0 (0x03U << (2 * 0))
#define MODER_MASK_PIN_1 (0x03U << (2 * 1))
#define MASK_PIN01F 0x03U
#define TIME_WINDOW_MS 250
#define INACTIVITY_TIMEOUT 500
#define TIMER_FREQUENCY 90000000.0

volatile static uint32_t lastEncoderCounter;
volatile static uint16_t lastUpdate;
volatile static uint32_t startTime;
static uint32_t currentTime;
uint16_t currentPhase;

volatile EncoderData data;

void resetEncoder(void) {
  data.encoderPosition = 0;
  data.error = false;
  data.ledTransitionCounter = 0;
  data.timestamp = 0;
  lastUpdate = GPIOF->IDR & MASK_PIN01F;
  startTime = TIM2->CNT;
  currentTime = startTime;
  lastEncoderCounter = 0;
}

/**
 * @brief Bestimmt die Drehrichtung.
 *
 * @param direction Zeiger auf Drehrichtung
 * @param phase Aktulle Phase des Drehgebers
 */
static inline void getPhase(void) {
  currentPhase = GPIOF->IDR & MASK_PIN01F;
  if (lastUpdate == PHASE_B) {

    if (currentPhase == PHASE_A) {
      // rueckwaertslauf
      data.ledTransitionCounter--;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_C) {
      // vorwaerts
      data.ledTransitionCounter++;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_D) {
      data.error = true;
    }
  }

  else if (lastUpdate == PHASE_A) {
    if (currentPhase == PHASE_B) {
      // vorwaerts
      data.ledTransitionCounter++;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_D) {
      // rueckwaerts
      data.ledTransitionCounter--;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_C) {
      data.error = true;
    }
  }

  else if (lastUpdate == PHASE_C) {
    if (currentPhase == PHASE_D) {
      // voerwaerts
      data.ledTransitionCounter++;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_B) {
      // rueckwaerts
      data.ledTransitionCounter--;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_A) {
      data.error = true;
    }
  }

  else if (lastUpdate == PHASE_D) {
    if (currentPhase == PHASE_A) {
      // voerwaerts
      data.ledTransitionCounter++;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_C) {
      // rueckwaers
      data.ledTransitionCounter--;
      data.encoderPosition++;
    } else if (currentPhase == PHASE_B) {
      data.error = true;
    }
  }
  lastUpdate = currentPhase;
}

void initInterrupt(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  GPIOF->MODER = GPIOF->MODER & ~MODER_MASK_PIN_0;
  GPIOF->MODER = GPIOF->MODER & ~MODER_MASK_PIN_1;

  SYSCFG->EXTICR[0] &= ~(0x0f << (4 * 0)); // Сlear EXTI0
  SYSCFG->EXTICR[0] &= ~(0x0f << (4 * 1)); // CLEAR EXTI1

  SYSCFG->EXTICR[0] |= 0x05 << (4 * 0); // EXTIO -> Port F
  SYSCFG->EXTICR[0] |= 0x05 << (4 * 1); // EXTI1 -> Port F

  // Trigger auswahlen

  EXTI->RTSR |=
      0x03; // Trigger auf positive Flanke sowohl fuer EXTI0 als auch fuer EXTI1
  EXTI->FTSR |=
      0x03; // Trigger auf negative Flanke sowohl fuer EXTI0 als auch fuer EXTI1
  EXTI->IMR |= 0x03; // Unmask

  // Priority setzen
  NVIC_SetPriority(EXTI0_IRQn, 0); // Setup EXTI0 auf hoechste Prio.
  NVIC_EnableIRQ(EXTI0_IRQn);      // Aktivieren

  NVIC_SetPriority(EXTI1_IRQn, 0); // Setup EXTI0 auf hoechste Prio.
  NVIC_EnableIRQ(EXTI1_IRQn);      // Aktivieren

  resetEncoder();
}

static inline void handleEncoderUpdate(void) {

  currentTime = TIM2->CNT;
  data.timestamp = currentTime - startTime;
  getPhase();
}
/**
 * @brief Interrupt-Service-Routine für das Encoder-Signal auf Leitung 0.
 */
void EXTI0_IRQHandler(void) {
  handleEncoderUpdate();
  EXTI->PR = (1 << 0);
}
/**
 * @brief Interrupt-Service-Routine für das Encoder-Signal auf Leitung 1.
 */
void EXTI1_IRQHandler(void) {
  handleEncoderUpdate();
  EXTI->PR = (1 << 1);
}

/**
 * @brief Prüft, ob die Encoder-Daten während des Lesens stabil (konsistent) geblieben sind.
 * @param snapConfirmed Puffer, in den die konsistenten Daten kopiert werden.
 * @return PASSED, wenn die Daten stabil sind, sonst FAIL.
 */
static int checkConsistency(EncoderData *snapConfirmed) {
    EncoderData snap1 = data; 
    EncoderData snap2 = data; 

    if ((snap1).encoderPosition == snap2.encoderPosition &&
        (snap1).ledTransitionCounter == snap2.ledTransitionCounter &&
        (snap1).timestamp == snap2.timestamp )  {

          (*snapConfirmed).encoderPosition = snap2.encoderPosition;
          (*snapConfirmed).ledTransitionCounter = snap2.ledTransitionCounter;
          (*snapConfirmed).timestamp = snap2.timestamp;

          return PASSED;
    }
    return FAIL;
}

static void getDirection(uint8_t *direction, uint32_t ledTransitionCounter) {
  static int32_t lastLedTransitionCounter = 0;

  if (ledTransitionCounter > lastLedTransitionCounter) {
    (*direction) = FORWARD;
  }
  else if (ledTransitionCounter < lastLedTransitionCounter) {
    (*direction) = BACKWARD;
  }
  else {
    (*direction) = IDLE;
  }
  lastLedTransitionCounter = ledTransitionCounter;
}

int encoderUpdater(double *angle, double *speed, uint8_t *direction) {
   if (data.error == true) {
    return ENCODER_DATA_INCOSISTENT;
  }
  int i = 0;
  int rc = OK;
 
  EncoderData snapConfirmed;

  do {
    rc = checkConsistency(&snapConfirmed);
    if (rc == FAIL && i == 9) {
      return ENCODER_DATA_INCOSISTENT;
    }
    i++;
  }
  while (rc == FAIL && i < 10);
  getDirection(&(*direction), snapConfirmed.ledTransitionCounter);
  (*angle) = snapConfirmed.ledTransitionCounter * 0.3;
  
  if (snapConfirmed.timestamp >= TIME_WINDOW_MS * 1000 * TICKS_PER_US) {
    double localAngle = (int32_t) (snapConfirmed.encoderPosition - lastEncoderCounter) * 0.3;
    double timeSec = snapConfirmed.timestamp/ TIMER_FREQUENCY;
    if (timeSec == 0) {
      return ENCODER_TIME_INVALID;
    }
    
    (*speed) = localAngle / timeSec;
    startTime = currentTime;
    
    lastEncoderCounter = snapConfirmed.encoderPosition;
    data.timestamp = 0;
    return ENCODER_TIME_UPDATED;
  } else if (snapConfirmed.timestamp>= INACTIVITY_TIMEOUT * 1000 * TICKS_PER_US && currentPhase == lastUpdate) { 
    (*speed) = 0;
    startTime = currentTime;
    lastEncoderCounter = snapConfirmed.encoderPosition;
    return ENCODER_TIME_UPDATED;
  }

  return rc;
}


void getEncoderLedTransitionCounter(int32_t *dataToMain) { // schlecht? 
  (*dataToMain) = data.ledTransitionCounter;
}