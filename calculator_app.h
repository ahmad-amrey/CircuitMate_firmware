#ifndef __CALCULATOR_APP_H__
#define __CALCULATOR_APP_H__

#include <Arduino.h>
#include <LiquidCrystal.h>

#define NOT_IMPLEMENTED                     do {char print_buff[100];sprintf(print_buff, "%s Not implemented", __PRETTY_FUNCTION__);Serial.println(print_buff);} while (0)


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
} VirtualButton;

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

class CalculatorApp {
public:
    CalculatorApp(LiquidCrystal *lcd_instance);
    virtual void screenMenu(uint8_t menu_indx);
    virtual void screenApp(VirtualButton pressed_btn); // Pure virtual function for displaying data

protected:
    virtual void resetState();
    LiquidCrystal *lcd; // Pointer to the LCD object
};

#endif // __CALCULATOR_APP_H__
