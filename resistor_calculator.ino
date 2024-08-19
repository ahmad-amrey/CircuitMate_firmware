/*
 * File: Resistor_calculator.ino
 * Author: Ahmad AlAmrey (ahmad.amrey@gmail.com)
 * Date: August 18, 2024
 * Description: This code reads an analog value from pin A0
 *              whenever a pulled-up digital input on pin D0 is pressed (active low).
 *              It allows the user to select resistor colors for different bands and calculates the resistor value.
 */

#include <LiquidCrystal.h>

// Define pin numbers
const int analogPin = A0; // Pin connected to the analog sensor
const int buttonPin = 2;  // Pin connected to the button (active low)
// LCD pins
const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;

// Define virtual buttons corresponding to resistor color bands
typedef enum
{
  VIRT_BUTTON_BLACK = 0,
  VIRT_BUTTON_BROWN,
  VIRT_BUTTON_RED,
  VIRT_BUTTON_ORANGE,
  VIRT_BUTTON_YELLOW,
  VIRT_BUTTON_GREEN,
  VIRT_BUTTON_BLUE,
  VIRT_BUTTON_VIOLET,
  VIRT_BUTTON_GREY,
  VIRT_BUTTON_WHITE,
  VIRT_BUTTON_GOLD,
  VIRT_BUTTON_SILVER,
  VIRT_BUTTON_OPTION,
  VIRT_BUTTON_CANCEL,
  VIRT_BUTTON_MAX
} v_buttons_t;

// Define the screen states
typedef enum
{
  SCREEN_4_BANDS = 0,
  // SCREEN_5_BANDS,
  // SCREEN_NONE
} screen_state_t;

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

const char *btn_names[VIRT_BUTTON_MAX] = {
    "BLACK",
    "BROWN",
    "RED",
    "ORANGE",
    "YELLOW",
    "GREEN",
    "BLUE",
    "VIOLET",
    "GREY",
    "WHITE",
    "GOLD",
    "SILVER",
    "OPTION",
    "CANCEL"};

const unsigned long led_toggle_delay_ms = 500; // LED toggle delay
const unsigned long debounceDelay = 50;        // Debounce delay
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables to store state
int analogValue = 0;
int bandIndex = 0;
int bandValues[4] = {0, 0, 0, 0}; // To store color bands
screen_state_t screenState = SCREEN_4_BANDS;

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
  v_buttons_t selected_btn = VIRT_BUTTON_BLACK;
  analogValue = analogRead(analogPin);

  // Determine which button is selected based on the analog value
  for (int i = 0; i < VIRT_BUTTON_MAX; i++)
  {
    if (analogValue < analog_v_btn_mapping[i + 1])
    {
      selected_btn = (v_buttons_t)i;
      break;
    }
  }

  // Store the selected band value
  if (selected_btn < VIRT_BUTTON_OPTION)
  {
    bandValues[bandIndex] = selected_btn;
    Serial.print("Selected Band ");
    Serial.print(bandIndex + 1);
    Serial.print(": ");
    Serial.println(btn_names[selected_btn]);
    bandIndex++;

    print_bands();

    // Check if we have all bands selected
    if (bandIndex == 4)
    {
      calculate_resistor_value();
      bandIndex = 0; // Reset for the next calculation
    }
  }
}

void calculate_resistor_value()
{
  long resistance = 0;
  int multiplier = 0;
  char val_str[40] = {0};
  char resistance_str[40] = {0};

  // Calculate resistance based on selected bands
    resistance = (bandValues[0] * 10 + bandValues[1]);
    multiplier = bandValues[2];

  // Apply multiplier
  for (int i = 0; i < multiplier; i++)
  {
    resistance *= 10;
  }

  // Determine the appropriate unit
  const char *unit = "ohms";
  float displayValue = resistance;

  if (resistance >= 1000000)
  {
    displayValue = resistance / 1000000.0;
    unit = "Mohms";
  }
  else if (resistance >= 1000)
  {
    displayValue = resistance / 1000.0;
    unit = "kohms";
  }

  dtostrf(displayValue, 1, 1, val_str);
  sprintf(resistance_str, "%s %s", val_str, unit);

  // Print the calculated resistance with appropriate unit
  Serial.print("Calculated Resistance: ");
  Serial.println(resistance_str);

  lcd.setCursor(0, 1);
  lcd.print(resistance_str);
}

void print_bands()
{
  char bands_sel_desc[40] = {0};
  char band_str[4][5] = {0};
  size_t i = 0;

  // Fill all bands with question mark
  for (i = 0; i < 4; i++)
  {
    band_str[i][0] = '?';
    band_str[i][1] = '?';
  }

  // Override set bands with its values
  for (i = 0; i < bandIndex; i++)
  {
    sprintf(band_str[i], "%02d", bandValues[i]);
  }
  
  sprintf(bands_sel_desc, "0:%s,1:%s,2:%s,3:%s", band_str[0], band_str[1], band_str[2], band_str[3]);

  lcd.setCursor(0, 0);
  lcd.print(bands_sel_desc);
}