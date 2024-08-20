#ifndef __CALCULATOR_APP_H__
#define __CALCULATOR_APP_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

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
  VIRT_BUTTON_NONE,
  VIRT_BUTTON_MAX
} v_buttons_t;

static const char *btn_names[VIRT_BUTTON_MAX] = {
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

class Calculator_app {
public:
    Calculator_app(LiquidCrystal *lcd_instance);
    virtual void screen_menu(uint8_t menu_indx) = 0;
    virtual void screen_app(v_buttons_t pressed_btn) = 0; // Pure virtual function for displaying data

protected:
    virtual void reset_state() = 0;
    LiquidCrystal *lcd; // Pointer to the LCD object
};

#endif // __CALCULATOR_APP_H__
