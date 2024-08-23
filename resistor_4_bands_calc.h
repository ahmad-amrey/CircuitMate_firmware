#ifndef RESISTOR_4_BANDS_CALC_H
#define RESISTOR_4_BANDS_CALC_H

#include "calculator_app.h"

class Resistor4BandsCalc : public CalculatorApp {
public:
    Resistor4BandsCalc(LiquidCrystal *lcd_instance);
    char* get_app_name(void) override;
    void screenApp(VirtualButton pressed_btn, bool is_pressed) override;
    void resetState() override;
    
private:
    void printBands();
    void calculateResistorValue();
    int bandValues[4];
    int bandIndex;
};

#endif // RESISTOR_4_BANDS_CALC_H