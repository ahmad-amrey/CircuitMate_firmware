#ifndef RESISTOR_4_BANDS_CALC_H
#define RESISTOR_4_BANDS_CALC_H

#include "calculator_app.h" // Include the base class

class resistor_4_bands_calc : public Calculator_app {
public:
    resistor_4_bands_calc(LiquidCrystal *lcd_instance);
    void screen_menu(uint8_t menu_indx) override;
    void screen_app(v_buttons_t pressed_btn) override;

protected:
    void reset_state() override;
    
private:
    void print_bands();
    void calculate_resistor_value();
    int bandValues[4];
    int bandIndex;
};

#endif // RESISTOR_4_BANDS_CALC_H