#ifndef RESISTOR_4_BANDS_CALC_H
#define RESISTOR_4_BANDS_CALC_H

#include "calculator_app.h"

class Resistor4BandsCalc : public CalculatorApp {
public:
    Resistor4BandsCalc(LiquidCrystal *lcd_instance);
    void screenMenu(uint8_t menu_indx) override;
    void screenApp(VirtualButton pressed_btn) override;

protected:
    void resetState() override;
    
private:
    void printBands();
    void calculateResistorValue();
    int bandValues[4];
    int bandIndex;
};

#endif // RESISTOR_4_BANDS_CALC_H