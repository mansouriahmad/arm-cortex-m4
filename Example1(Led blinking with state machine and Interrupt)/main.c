#include <stdint.h>
#include "bsp.h"



#if 0
/* background code: sequential with blocking version */
int main() {
	  BSP_init();
    while (1) {
			  BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
			  BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }

    //return 0;
}
#endif

typedef enum { IDLE, OFF_STATE, ON_STATE} State;

/* background code: non-blocking version */
int main() {
    BSP_init();
	sw1_pushed = false;

	State redState = IDLE;
	State greenState = IDLE;
	State blueState = IDLE;
	
	static uint32_t redStart, greenStart, blueStart;	
	
    while (1) {
        /* Blinky polling state machine */

        switch (greenState) {
            case IDLE:
                greenStart = BSP_tickCtr();
				BSP_ledGreenOff();
                greenState = OFF_STATE; /* initial transition */
                break;
            case OFF_STATE:
                if ((BSP_tickCtr() - greenStart) > BSP_TICKS_PER_SEC * 15U / 16U ) {
                    BSP_ledGreenOn();
                    greenStart = BSP_tickCtr();
                    greenState = ON_STATE; /* state transition */
                }
                break;
            case ON_STATE:
                if ((BSP_tickCtr() - greenStart) > BSP_TICKS_PER_SEC / 16U) {
                    BSP_ledGreenOff();
                    greenStart = BSP_tickCtr();
                    greenState = OFF_STATE; /* state transition */
                }
                break;
            default:
                //error();
                break;
        }
        switch (blueState) {
            case IDLE:
                blueStart = BSP_tickCtr();
				BSP_ledGreenOff();
                blueState = OFF_STATE; /* initial transition */
                break;
            case OFF_STATE:
                if ((BSP_tickCtr() - blueStart) > BSP_TICKS_PER_SEC * 23U / 17U ) {
                    BSP_ledBlueOn();
                    blueStart = BSP_tickCtr();
                    blueState = ON_STATE; /* state transition */
                }
                break;
            case ON_STATE:
                if ((BSP_tickCtr() - blueStart) > BSP_TICKS_PER_SEC / 13U) {
                    BSP_ledBlueOff();
                    blueStart = BSP_tickCtr();
                    blueState = OFF_STATE; /* state transition */
                }
                break;
            default:
                //error();
                break;
        }
		
        switch (redState) {
            case IDLE:
				if (sw1_pushed) {
					sw1_pushed = false;
					redStart = BSP_tickCtr();
					BSP_ledRedOff();
					redState = OFF_STATE; /* initial transition */
				}
                break;
            case OFF_STATE:
                if ((BSP_tickCtr() - redStart) > BSP_TICKS_PER_SEC * 4U / 2U ) {
                    BSP_ledRedOn();
                    redStart = BSP_tickCtr();
                    redState = ON_STATE; /* state transition */
                }
                break;
            case ON_STATE:
                if ((BSP_tickCtr() - redStart) > BSP_TICKS_PER_SEC / 2U) {
                    BSP_ledRedOff();
                    redStart = BSP_tickCtr();
                    redState = IDLE; /* state transition */
                }
                break;
            default:
                //error();
                break;
        }
    }
    //return 0;
}
