#include "rotary_decoder.h"
#include "global.h"
#include "fehler.h"
#include "stm32f429xx.h"
#include <stdbool.h>
#include <stdint.h>


#define MASK_PIN01F 0x03U 
#define TIME_WINDOW_MS 250
#define TIMER_FREQUENCY 90000000.0
static uint8_t lastPhase = PHASE_A; 
static uint32_t time1 = 0;
static uint32_t time2 = 0;
static uint32_t transitionCount = 0;
static double wSpeed = 0;




void timerStart(void) {
   time1 = TIM2->CNT;
}

void updateSpeed(double *speed) {
     *speed = wSpeed; 
}

void updateTime(uint32_t * timeD) {
    *timeD = time2-time1;
}

void calculateSpeed() {
    double timeSec = (time2-time1) / TIMER_FREQUENCY;
    wSpeed = transitionCount * 0.3 / timeSec; // 360/1200
    time1 = time2;
    time2 = 0;
    transitionCount = 0;
}

int ecoderUpdate(uint8_t eingabe, uint8_t *direction) {

    uint8_t result = eingabe & MASK_PIN01F;

		if (result != lastPhase) {

            transitionCount++;
            time2 = TIM2->CNT;
            if (time2-time1 >= TIME_WINDOW_MS * 90000) {
                calculateSpeed();
            }

			if (lastPhase == PHASE_B) {

				if (result == PHASE_A) {
					//rueckwaertslauf 
                    *direction = BACKWARD;
				}
				else if (result == PHASE_C) {
					//vorwaerts
                     *direction = FORWARD;
				}
				else if (result == PHASE_B) {
					//stehen geblieben
                    *direction = IDLE;
				}
				else if (result == PHASE_D) {
					return ENCODER_ERROR;
				}
			}

			else if (lastPhase == PHASE_A) {
				if (result == PHASE_B) {
					//vorwaerts 
                     *direction = FORWARD;
				}
				else if (result == PHASE_D) {
					//rueckwaerts
                     *direction = BACKWARD;
				}
				else if (result == PHASE_A) {
					//stehen geblieben
                     *direction = IDLE;
				}
				else if (result == PHASE_C) {
					return ENCODER_ERROR;
				}
			}

			else if (lastPhase == PHASE_C) {
				if (result == PHASE_D) {
					//voerwaerts
                     *direction = FORWARD;
				}
				else if (result == PHASE_B) {
					//rueckwaerts
                     *direction = BACKWARD;
				}
				else if (result == PHASE_C) {
					//stehen geblieben
                     *direction = IDLE;
				}
				else if (result == PHASE_A) {
					return ENCODER_ERROR;
				}
			}

			else if (lastPhase == PHASE_D) {
				if (result == PHASE_A) {
					//voerwaerts
                     *direction = FORWARD;
				}
				else if (result == PHASE_C)  {
					//rueckwaers
                    *direction = BACKWARD;
				}
				else if (result == PHASE_D) {
					//stehen geblieben
                     *direction = IDLE;
				}
				else if (result == PHASE_B) {
					return ENCODER_ERROR;
				}
			}
			lastPhase = result;

            return ENCODER_CHANGED;
		}
    
    return ENCODER_ERROR;
}