/*
 * File: Resistor_calculator.ino
 * Author: Ahmad AlAmrey (ahmad.amrey@gmail.com)
 * Date: August 18, 2024
 * Description: This code reads an analog value from pin A0
 *              whenever a pulled-up digital input on pin D0 is pressed (active low).
 *              It allows the user to select resistor colors for different bands and calculates the resistor value.
 */

#include <LiquidCrystal.h>
#include "calculator_app.h"
#include "resistor_4_bands_calc.h"

// Define pin numbers
const int analogPin = A0; // Pin connected to the analog sensor
const int buttonPin = 2;  // Pin connected to the button (active low)
// LCD pins
const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;

// Define the calculator applications
typedef enum
{
  APPS_RESISTOR_4_BANDS_CALC = 0,
  APPS_MAX,
} apps_t;

// Mapping of analog values to virtual buttons
const uint16_t analog_v_btn_mapping[VIRT_BUTTON_MAX + 1] = {
    0,
    6,
    85,
    174,
    260,
    340,
    420,
    490,
    570,
    645,
    720,
    805,
    900,
    975,
    1500, // Something far
};

const unsigned long led_toggle_delay_ms = 500; // LED toggle delay
const unsigned long debounceDelay = 50;        // Debounce delay
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Resistor4BandsCalc res_4_calc = Resistor4BandsCalc(&lcd);

// Variables to store state
int analogValue = 0;
int bandIndex = 0;
int bandValues[4] = {0, 0, 0, 0}; // To store color bands

void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(analogPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("4 Bands Calc");
}

void loop()
{
  static int lastButtonState = LOW;
  static int buttonState;
  static unsigned long lastDebounceTime = 0;
  static unsigned long next_led_toggle_time = 0;

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
      {
        process_btn_press();
      }
    }
  }

  lastButtonState = reading;

  if (millis() > next_led_toggle_time)
  {
    next_led_toggle_time = millis() + led_toggle_delay_ms;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void process_btn_press()
{
  VirtualButton selected_btn = VIRT_BUTTON_BLACK;
  analogValue = analogRead(analogPin);

  // Determine which button is selected based on the analog value
  for (int i = 0; i < VIRT_BUTTON_MAX; i++)
  {
    if (analogValue < analog_v_btn_mapping[i + 1])
    {
      selected_btn = (VirtualButton)i;
      break;
    }
  }

  switch (selected_btn)
  {
  case VIRT_BUTTON_BLACK:
  case VIRT_BUTTON_BROWN:
  case VIRT_BUTTON_RED:
  case VIRT_BUTTON_ORANGE:
  case VIRT_BUTTON_YELLOW:
  case VIRT_BUTTON_GREEN:
  case VIRT_BUTTON_BLUE:
  case VIRT_BUTTON_VIOLET:
  case VIRT_BUTTON_GREY:
  case VIRT_BUTTON_WHITE:
  case VIRT_BUTTON_GOLD:
  case VIRT_BUTTON_SILVER:
  case VIRT_BUTTON_CANCEL:
    res_4_calc.screenApp(selected_btn);
    break;

  default:
    // Just ignore it
    break;
  }
}
