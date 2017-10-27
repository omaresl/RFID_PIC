#include <pic18f46k40.h>
#include "stdtypedef.h"
#include "app_LED.h"

/*************************************
 * Definitions
 *************************************/
typedef enum {
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    PURPLE,
    WHITE

} E_COLOR_STATE;
/*************************************
 * Variables
 *************************************/
E_COLOR_STATE re_ColorState = BLACK;

/*************************************
 * Code
 *************************************/

/*************************************
 * Function Name: app_LED_Task
 * Description: TBD
 *************************************/
void app_LED_Task(void) {
    switch (re_ColorState) {
        case BLACK:
        {
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();

            re_ColorState = RED;

        }
        break;
        case RED:
        {
            LED_RED_ON();
            LED_GREEN_OFF();
            LED_BLUE_OFF();

            re_ColorState = GREEN;
        }
        break;
        case GREEN:
        {
            LED_RED_OFF();
            LED_GREEN_ON();
            LED_BLUE_OFF();

            re_ColorState = BLUE;
        }
        break;
        case BLUE:
        {
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_ON();

            re_ColorState = YELLOW;
        }
        break;
        case YELLOW:
        {
            LED_RED_ON();
            LED_GREEN_ON();
            LED_BLUE_OFF();

            re_ColorState = CYAN;
        }
        break;
        case CYAN:
        {
            LED_RED_OFF();
            LED_GREEN_ON();
            LED_BLUE_ON();

            re_ColorState = PURPLE;
        }
        break;
        case PURPLE:
        {
            LED_RED_ON();
            LED_GREEN_OFF();
            LED_BLUE_ON();

            re_ColorState = WHITE;
        }
        break;
        case WHITE:
        {
            LED_RED_ON();
            LED_GREEN_ON();
            LED_BLUE_ON();

            re_ColorState = BLACK;
        }
        break;
        default:
        {
            re_ColorState = BLACK;
        }
        break;
    }
}