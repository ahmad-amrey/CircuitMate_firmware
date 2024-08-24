#ifndef RESISTOR_5_BANDS_CALC_H
#define RESISTOR_5_BANDS_CALC_H

#include "calculator_app.h"

class Resistor5BandsCalc : public CalculatorApp {
public:
    Resistor5BandsCalc(LiquidCrystal *lcd_instance);
    char* get_app_name(void) override;
    void screenApp(VirtualButton pressed_btn, bool is_pressed) override;
    void resetState() override;
    
private:
    bool is_valid_btn(VirtualButton pressed_btn, uint8_t indx);
    void printBands();
    void calculateResistorValue();
    int bandValues[5];
    int bandIndex;
};

#endif // RESISTOR_5_BANDS_CALC_H