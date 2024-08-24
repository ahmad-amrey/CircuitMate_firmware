#ifndef CAPACITOR_3_DIGITS_CALC_H
#define CAPACITOR_3_DIGITS_CALC_H

#include "calculator_app.h"

class Capacitor3DigitsCalc : public CalculatorApp {
public:
    Capacitor3DigitsCalc(LiquidCrystal *lcd_instance);
    char* get_app_name(void) override;
    void screenApp(VirtualButton pressed_btn, bool is_pressed) override;
    void resetState() override;
    
private:
    bool is_valid_btn(VirtualButton pressed_btn, uint8_t indx);
    void printDigits();
    void calculateCapacitorValue();
    uint8_t digitsValues[3];
    int digitIndex;
};

#endif // CAPACITOR_3_DIGITS_CALC_H