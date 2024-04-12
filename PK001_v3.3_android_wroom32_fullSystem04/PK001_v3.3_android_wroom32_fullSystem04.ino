/*
 * ANDROID GAMEPAD
 * {A=1, B=2, C=3, X=4, Y=5, Z=6, L1=7, R1=8, L2=9, R2=10,
 * Select=11, Start=12, PS=13, L3=14 , R3=15 , DPAD_R = 22 , DPAD_DN = 20} 
 */

#include <Arduino.h>
#include <BleGamepad.h>

int batteryLevel = 100;
#define blueLED 25
#define greenLED 26

// Variable to track button state
int buttonState = 0;
// Variable to track LED state
bool ledOn = false;
// Variable to track time of last button press
unsigned long lastButtonPressTime = 0;
// Variable to track whether LED is blinking
bool blinking = false;

#define HOME 13
#define BUTTON_R1_PIN 15
#define BUTTON_R2_PIN 2
#define BUTTON_PS_PIN 4
#define BUTTON_L1_PIN 18
#define BUTTON_L2_PIN 19

#define LOW_BATTERY_LEVEL 3.5  // Example low battery voltage level (in volts)

bool buttonsEnabled = false;


typedef enum { ANDROID } GamepadModes;
GamepadModes gamepadMode = ANDROID;

BleGamepad bleGamepad("Breath Buddy", "BreathBuddy", 100);
BleGamepadConfiguration bleGamepadConfig;


void setup() {

  Serial.begin(115200);
  pinMode(HOME, INPUT_PULLUP);  //+5V---[Internal 50k]---[pin]---[switch]---GND
  pinMode(BUTTON_R1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_R2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_L1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_L2_PIN, INPUT_PULLUP);

  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);  // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
  bleGamepad.sendReport();                                      // to disable auto reporting, and then use bleGamepad.sendReport(); as needed
  bleGamepad.begin(&bleGamepadConfig);
  Serial.println("Starting BLE work!");

  delay(500);
  Serial.println("Breath Buddy is ready, captain!!");
}

void loop() {


  if (bleGamepad.isConnected()) {

    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, HIGH);

    switch (gamepadMode) {
      case ANDROID:

        if (digitalRead(HOME) == LOW) {
          buttonsEnabled = !buttonsEnabled;  // Toggle the flag
          delay(50);  // Debounce delay

          buttonState = digitalRead(HOME);
          psButtonPressed();

            // If the button is pressed and it has been more than 1 second since the last press
            if (buttonState == HIGH && millis() - lastButtonPressTime > 1000) {
            // Update lastButtonPressTime
            lastButtonPressTime = millis();
            // If LED is not currently blinking
            if (!blinking) {
              // Set LED state to on
              ledOn = true;
              // Set blinking to true
              blinking = true;
            }
          }
          // If LED is currently blinking
          if (blinking) {
            // Blink the LED for 3 seconds
            unsigned long startTime = millis();
            while (millis() - startTime < 500) {
              digitalWrite(greenLED, HIGH);
              delay(50);
              digitalWrite(greenLED, LOW);
              delay(50);
            }
            // Set LED state to off
            ledOn = false;
            // Set blinking to false
            blinking = false;
          }
          // Update LED state
          digitalWrite(greenLED, ledOn ? HIGH : LOW);
          buttonsEnabled = true;
        }

        if (buttonsEnabled) {
          // Check for button presses only if buttons are enabled

          checkButtonR1Press();
          checkButtonR2Press();
          checkButtonPSPress();
          checkButtonL1Press();
          checkButtonL2Press();
        }
    }

    bleGamepad.sendReport();
  } else {

    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(blueLED, LOW);  // turn the LED off by making the voltage LOW
    delay(100);
  }
}


void checkButtonR1Press() {
  if (digitalRead(BUTTON_R1_PIN) == HIGH) {
    Serial.println(F("Sensor1 is on"));
    bleGamepad.press(BUTTON_8);  //R1
  } else {
    bleGamepad.release(BUTTON_8);  //R1
  }
}
void checkButtonR2Press() {
  if (digitalRead(BUTTON_R2_PIN) == HIGH) {
    Serial.println(F("Sensor2 is on"));
    bleGamepad.press(BUTTON_10);  //R2
  } else {
    bleGamepad.release(BUTTON_10);  //R2
  }
}
void checkButtonPSPress() {
  if (digitalRead(BUTTON_PS_PIN) == HIGH) {
    Serial.println(F("Sensor3 is on"));  //PS
    bleGamepad.press(BUTTON_13);
  } else {
    bleGamepad.release(BUTTON_13);  //PS
  }
}
void checkButtonL1Press() {
  if (digitalRead(BUTTON_L1_PIN) == HIGH) {
    Serial.println(F("Sensor4 is on"));
    bleGamepad.press(BUTTON_7);  //L1
  } else {
    bleGamepad.release(BUTTON_7);  //L1
  }
}
void checkButtonL2Press() {
  if (digitalRead(BUTTON_L2_PIN) == HIGH) {
    Serial.println(F("Sensor5 is on"));
    bleGamepad.press(BUTTON_9);  //L2
  } else {
    bleGamepad.release(BUTTON_9);  //L2
  }
}

void psButtonPressed() {

  if (digitalRead(HOME) == LOW) {
    // Home button is pressed
    Serial.println("Press button and start.");
    bleGamepad.press(BUTTON_12);
    // return true;
  } else {
    // Home button is not pressed
    bleGamepad.release(BUTTON_12);
    // return false;
  }
}
