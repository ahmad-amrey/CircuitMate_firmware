#include "resistor_4_bands_calc.h"

Resistor4BandsCalc::Resistor4BandsCalc(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

void Resistor4BandsCalc::screenMenu(uint8_t menu_indx)
{
    NOT_IMPLEMENTED;
}

void Resistor4BandsCalc::screenApp(VirtualButton pressed_btn)
{

    if (pressed_btn == VIRT_BUTTON_CANCEL)
    {
        resetState();
        Serial.println("Cancel requested");
        printBands();
        return;
    }

    bandValues[bandIndex] = pressed_btn;
    Serial.print("Selected Band ");
    Serial.print(bandIndex + 1);
    Serial.print(": ");
    Serial.println(btn_names[pressed_btn]);
    bandIndex++;

    printBands();

    // Check if we have all bands selected
    if (bandIndex == 4)
    {
        calculateResistorValue();
        bandIndex = 0;                             // Reset for the next calculation
        memset(bandValues, 0, sizeof(bandValues)); // Initialize the array
    }
}

void Resistor4BandsCalc::resetState()
{
    bandIndex = 0;
    memset(bandValues, 0, sizeof(bandValues)); // Initialize the array
}

void Resistor4BandsCalc::printBands()
{
    char bands_sel_desc[40] = {0};
    char band_str[4][5] = {0};
    size_t i = 0;

    // Fill all bands with question mark
    for (i = 0; i < 4; i++)
    {
        band_str[i][0] = '?';
        band_str[i][1] = '?';
    }

    // Override set bands with its values
    for (i = 0; i < bandIndex; i++)
    {
        sprintf(band_str[i], "%02d", bandValues[i]);
    }

    sprintf(bands_sel_desc, "%s %s %s %s", band_str[0], band_str[1], band_str[2], band_str[3]);

    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(bands_sel_desc);
}

void Resistor4BandsCalc::calculateResistorValue()
{
    long resistance = 0;
    int multiplier = 0;
    char val_str[40] = {0};
    char resistance_str[40] = {0};

    // Calculate resistance based on selected bands
    resistance = (bandValues[0] * 10 + bandValues[1]);
    multiplier = bandValues[2];

    // Apply multiplier
    for (int i = 0; i < multiplier; i++)
    {
        resistance *= 10;
    }

    // Determine the appropriate unit
    const char *unit = "ohms";
    float displayValue = resistance;

    if (resistance >= 1000000)
    {
        displayValue = resistance / 1000000.0;
        unit = "Mohms";
    }
    else if (resistance >= 1000)
    {
        displayValue = resistance / 1000.0;
        unit = "kohms";
    }

    dtostrf(displayValue, 1, 1, val_str);
    sprintf(resistance_str, "%s %s", val_str, unit);

    // Print the calculated resistance with appropriate unit
    Serial.print("Calculated Resistance: ");
    Serial.println(resistance_str);

    lcd->setCursor(0, 1);
    lcd->print(resistance_str);
}
