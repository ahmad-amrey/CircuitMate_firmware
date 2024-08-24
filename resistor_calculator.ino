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
#include "resistor_5_bands_calc.h"
#include "resistor_smd_3_digits_calc.h"

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

const unsigned long led_toggle_delay_ms = 500; // LED toggle delay
const unsigned long DEBOUNCE_DELAY = 50;       // Debounce delay
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Initlize apps
Resistor4BandsCalc res_4_calc(&lcd);
Resistor5BandsCalc res_5_calc(&lcd);
ResistorSMD3DigitsCalc res_smd_3_calc(&lcd);
CalculatorApp *selected_app = NULL;

// Variables to store state
static bool in_menu = false;

void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(analogPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Set inital app
  selected_app = &res_4_calc;

  // Print the initial app name
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(selected_app->get_app_name());
  delay(1500);

  // Call the app with none button (pressed) to ask it to draw inital app page
  selected_app->screenApp(VIRT_BUTTON_NONE, true);
}

void loop()
{
  int curr_btn_reading;
  static int last_btn_reading = LOW;
  static int debounced_btn_state;
  static unsigned long last_debounce_time = 0;
  static unsigned long next_led_toggle_time = 0;
  static VirtualButton last_knob_position = VIRT_BUTTON_NONE;
  VirtualButton curr_knob_position = VIRT_BUTTON_NONE;
  bool btn_is_pressed = false;

  // Handle LED toggling
  if (millis() > next_led_toggle_time)
  {
    next_led_toggle_time = millis() + led_toggle_delay_ms;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // Read and debounce button
  curr_btn_reading = digitalRead(buttonPin);
  if (curr_btn_reading != last_btn_reading)
  {
    last_debounce_time = millis();
  }

  last_btn_reading = curr_btn_reading;

  if ((millis() - last_debounce_time) > DEBOUNCE_DELAY)
  {
    if (curr_btn_reading != debounced_btn_state)
    {
      debounced_btn_state = curr_btn_reading;
      if (debounced_btn_state == LOW)
      {
        btn_is_pressed = true;
      }
    }
  }

  // Read which position knob is on
  curr_knob_position = get_knob_position();

  // Refreash LCD with menu or app
  if (in_menu)
  {
    refresh_screen_with_menu(curr_knob_position, btn_is_pressed);
  }
  else
  {
    refresh_screen_with_app(curr_knob_position, btn_is_pressed);
  }
}

VirtualButton get_knob_position()
{
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

  int analogValue = 0;
  VirtualButton knob_on = VIRT_BUTTON_0_BLACK;

  analogValue = analogRead(analogPin);

  // Determine which button is selected based on the analog value
  for (int i = 0; i < VIRT_BUTTON_NONE; i++)
  {
    if (analogValue < analog_v_btn_mapping[i + 1])
    {
      knob_on = (VirtualButton)i;
      break;
    }
  }

  return knob_on;
}

void refresh_screen_with_menu(VirtualButton selected_btn, bool is_pressed)
{
  static const uint8_t MAX_APPS = 3;
  uint8_t app_idx = UINT8_MAX;
  static CalculatorApp *last_selected_app = (CalculatorApp *)SIZE_MAX;
  static char lcd_nav_bar[17] = {0};

  switch (selected_btn)
  {
  case VIRT_BUTTON_0_BLACK:
    selected_app = &res_4_calc;
    app_idx = selected_btn + 1;
    break;
  case VIRT_BUTTON_1_BROWN:
    selected_app = &res_5_calc;
    app_idx = selected_btn + 1;
    break;
  case VIRT_BUTTON_2_RED:
    selected_app = &res_smd_3_calc;
    app_idx = selected_btn + 1;
    break;

  default:
    selected_app = NULL;
    app_idx = UINT8_MAX;
    break;
  }

  // Check if the app has changed, refreash if so
  if (selected_app != last_selected_app)
  {
    last_selected_app = selected_app;

    lcd.clear();

    // Handle empty app slots differently
    if (selected_app != NULL)
    {
      lcd.setCursor(0, 0);
      lcd.print(selected_app->get_app_name());
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("No App");
    }

    memset(lcd_nav_bar, 0, sizeof(lcd_nav_bar));

    // Process navigation bar - Numbers
    if (app_idx > MAX_APPS)
    {
      sprintf(lcd_nav_bar, "      ?/%d       ", MAX_APPS);
    }
    else if (app_idx <= MAX_APPS)
    {
      sprintf(lcd_nav_bar, "      %d/%d       ", app_idx, MAX_APPS);
    }

    // Process navigation bar - Arrows
    if (app_idx == 1)
    {
      lcd_nav_bar[0] = ' ';
    }
    else
    {
      lcd_nav_bar[0] = '<';
    }

    if (app_idx >= MAX_APPS)
    {
      lcd_nav_bar[15] = ' ';
    }
    else
    {
      lcd_nav_bar[15] = '>';
    }

    lcd.setCursor(0, 1);
    lcd.print(lcd_nav_bar);
  }

  // Reset function state and exit menu mode
  if (is_pressed)
  {
    last_selected_app = (CalculatorApp *)SIZE_MAX;
    // If we have invalid app, just propt the user to select valid on and return
    if (selected_app == NULL)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please select a");
      lcd.setCursor(0, 1);
      lcd.print("valid app");
      delay(1500);
    }
    else
    {
      in_menu = false;
      // Call the app with none button (pressed) to ask it to draw inital app page
      selected_app->screenApp(VIRT_BUTTON_NONE, true);
    }
  }
}

void refresh_screen_with_app(VirtualButton selected_btn, bool is_pressed)
{
  switch (selected_btn)
  {
  case VIRT_BUTTON_0_BLACK:
  case VIRT_BUTTON_1_BROWN:
  case VIRT_BUTTON_2_RED:
  case VIRT_BUTTON_3_ORANGE:
  case VIRT_BUTTON_4_YELLOW:
  case VIRT_BUTTON_5_GREEN:
  case VIRT_BUTTON_6_BLUE:
  case VIRT_BUTTON_7_VIOLET:
  case VIRT_BUTTON_8_GREY:
  case VIRT_BUTTON_9_WHITE:
  case VIRT_BUTTON_GOLD:
  case VIRT_BUTTON_SILVER:
  case VIRT_BUTTON_CANCEL:
    selected_app->screenApp(selected_btn, is_pressed);
    break;
  case VIRT_BUTTON_MENU:
    if (is_pressed)
    {
      selected_app->resetState();
      in_menu = true;
    }
    break;

  default:
    // Just ignore it
    break;
  }
}
