#include <Arduino.h>
#include <clickButton.h>
#include <WS2811.h>
#include <EEPROM.h>

//=============================================================================
// Global constants for WS2182 interface, beeper interface and button(s)
//=============================================================================
#define LED_COUNT           1
#define LED_PIN             0
#define BUTTON_PIN          2
#define BEEPER_PIN          1

#define ONE_SECOND          1000L
#define BEEP_TIME           1 * (ONE_SECOND)
#define TOTAL_TIME          5 * (60 * ONE_SECOND)

#define MAX_GREEN_VALUE     255
#define MAX_RED_VALUE       255
#define MAX_VALUE_ALL       MAX_GREEN_VALUE + MAX_RED_VALUE

#define STEP_INTERVAL_TIME  TOTAL_TIME / MAX_VALUE_ALL

//=============================================================================
// Types
//=============================================================================
typedef enum {

    timerStateIdle = 0,
    timerActivated,
    timerExpired,
} timerStateHandler_t;

//=============================================================================
// Global objects for WS2812 NeoPixelBus object
//=============================================================================
DEFINE_WS2811_FN(WS2811RGB, PORTB, LED_PIN)
RGB_t rgb[LED_COUNT];

//=============================================================================
// Global objects for ClickButton object
//=============================================================================
ClickButton button(BUTTON_PIN, LOW, CLICKBTN_PULLUP);

//=============================================================================
//  Helper function
//=============================================================================
static RGB_t encodeColour(uint16_t colourValue) {

    // This function will encode colourValue into a scale from GREEN [0] to
    // RED [511]

    RGB_t desiredColour;

    if (colourValue <= MAX_VALUE_ALL) {
        // Handle GREEN to YELLOW [0 ... 255]
        desiredColour.r = colourValue;
        desiredColour.g = MAX_GREEN_VALUE;
        desiredColour.b = 0;

        if (colourValue > MAX_GREEN_VALUE) {
            // Handle YELLOW to RED [256 ... 511]
            desiredColour.r = MAX_RED_VALUE;
            desiredColour.g = MAX_GREEN_VALUE - (colourValue - MAX_GREEN_VALUE);
            desiredColour.b = 0;
        }
    } else {
        desiredColour.r = 0;
        desiredColour.g = 0;
        desiredColour.b = 0;
    }

    return (desiredColour);
}

//=============================================================================
// Setup function
//=============================================================================
void setup() {

    pinMode(LED_PIN, OUTPUT);
    pinMode(BEEPER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

//=============================================================================
// Loop function
//
// Basic operation:
// Short press: Sets LED to GREEN, short Beep and starts TOTAL_TIME timer.
//              During TOTAL_TIME period LED will fade from GREEN to RED.
//              While LED is RED long beep for 1 second. LED remains on.
//
// Long press:  Programming mode.
//=============================================================================
void loop() {

    static timerStateHandler_t timerState;
    static uint32_t lastUpdateTime = 0;
    static uint16_t ledColour = 0;
    uint32_t currentTime = 0;

    currentTime = millis();
    button.Update();

    switch (timerState) {
        case timerStateIdle:
        {
            if (button.clicks == 1) {
                tone(BEEPER_PIN, 1000, BEEP_TIME);

                ledColour = 0;
                rgb[0] = encodeColour(ledColour);
                WS2811RGB(rgb, ARRAYLEN(rgb));

                lastUpdateTime = currentTime;
                timerState = timerActivated;
            }
        }
        break;

        case timerActivated:
        {
            if ((currentTime - lastUpdateTime) >=  STEP_INTERVAL_TIME) {

                if (ledColour > MAX_VALUE_ALL) {
                    tone(BEEPER_PIN, 1000, BEEP_TIME);

                    timerState = timerExpired;
                    break;
                } else {
                    rgb[0] = encodeColour(ledColour);
                    WS2811RGB(rgb, ARRAYLEN(rgb));
                }

                ledColour++;

                lastUpdateTime = currentTime;
            }
        }
        break;

        case timerExpired:
        {
            if ((currentTime - lastUpdateTime) >=  BEEP_TIME) {
                timerState = timerStateIdle;
            }
        }
        break;

        default:
            break;
    }
}
