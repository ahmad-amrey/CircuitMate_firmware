#include "calculator_app.h"

CalculatorApp::CalculatorApp(LiquidCrystal *lcd_instance)
{
        this->lcd = lcd_instance;
}

char* CalculatorApp::get_app_name(void)
{
        return "Not implemented";
}

void CalculatorApp::screenApp(VirtualButton pressed_btn, bool is_pressed)
{
        NOT_IMPLEMENTED;
}

void CalculatorApp::resetState()
{
        NOT_IMPLEMENTED;
}
