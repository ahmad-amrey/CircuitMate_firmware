#ifndef RESISTOR_SMD_4_DIGITS_CALC_H
#define RESISTOR_SMD_4_DIGITS_CALC_H

#include "calculator_app.h"

class ResistorSMD4DigitsCalc : public CalculatorApp {
public:
    ResistorSMD4DigitsCalc(LiquidCrystal *lcd_instance);
    char* get_app_name(void) override;
    void screenApp(VirtualButton pressed_btn, bool is_pressed) override;
    void resetState() override;
    
private:
    bool is_valid_btn(VirtualButton pressed_btn, uint8_t indx);
    void printDigits();
    void calculateResistorValue();
    uint8_t digitsValues[4];
    int digitIndex;
};

#endif // RESISTOR_SMD_4_DIGITS_CALC_H