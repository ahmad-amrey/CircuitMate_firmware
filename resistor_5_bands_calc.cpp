#include "resistor_5_bands_calc.h"

Resistor5BandsCalc::Resistor5BandsCalc(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

char* Resistor5BandsCalc::get_app_name(void)
{
        return "Resistor 5 bands";
}

void Resistor5BandsCalc::screenApp(VirtualButton pressed_btn, bool is_pressed)
{
    if(!is_pressed)
    {
        // Nothing to do here, just return
        return;
    }

    if (pressed_btn == VIRT_BUTTON_CANCEL)
    {
        resetState();
        Serial.println("Cancel requested");
        printBands();
        return;
    }

    if (pressed_btn == VIRT_BUTTON_NONE)
    {
        resetState();
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
    if (bandIndex == 5)
    {
        calculateResistorValue();
        bandIndex = 0;                             // Reset for the next calculation
        memset(bandValues, 0, sizeof(bandValues)); // Initialize the array
    }
}

void Resistor5BandsCalc::resetState()
{
    bandIndex = 0;
    memset(bandValues, 0, sizeof(bandValues)); // Initialize the array
}

void Resistor5BandsCalc::printBands()
{
    char bands_sel_desc[40] = {0};
    char band_str[5][5] = {0};
    size_t i = 0;

    // Fill all bands with question mark
    for (i = 0; i < 5; i++)
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

void Resistor5BandsCalc::calculateResistorValue()
{
    long resistance = 0;
    float multiplier = 0;
    float displayValue = resistance;
    const char *unit = "ohms";
    const char *tolerance_str = "";
    char displayValueStr[40] = {0};
    char resistance_str_serial[40] = {0};
    char resistance_str_lcd[40] = {0};

    // make plus minues characters:
    const static uint8_t plus_minues_char[8] = {
        0b00100,
        0b00100,
        0b11111,
        0b00100,
        0b00100,
        0b00000,
        0b11111,
        0b00000};

    // Calculate resistance based on selected bands
    resistance = ((bandValues[0] * 10) + bandValues[1]);

    // Find multiplier
    switch (bandValues[2])
    {
    case VIRT_BUTTON_BLACK:
    case VIRT_BUTTON_ORANGE:
    case VIRT_BUTTON_BLUE:
    case VIRT_BUTTON_WHITE:
        multiplier = 1.0;
        break;
    case VIRT_BUTTON_BROWN:
    case VIRT_BUTTON_YELLOW:
    case VIRT_BUTTON_VIOLET:
    case VIRT_BUTTON_SILVER:
        multiplier = 10.0;
        break;
    case VIRT_BUTTON_RED:
    case VIRT_BUTTON_GREEN:
    case VIRT_BUTTON_GREY:
    case VIRT_BUTTON_GOLD:
        multiplier = 0.1;
        break;

    default:
        // Not supposed to be here
        return;
    }

    // Determine the appropriate unit
    switch (bandValues[2])
    {
    case VIRT_BUTTON_BLACK:
    case VIRT_BUTTON_BROWN:
    case VIRT_BUTTON_GOLD:
        unit = " ohms";
        break;
    case VIRT_BUTTON_RED:
    case VIRT_BUTTON_ORANGE:
    case VIRT_BUTTON_YELLOW:
        unit = "k ohms";
        break;
    case VIRT_BUTTON_GREEN:
    case VIRT_BUTTON_BLUE:
    case VIRT_BUTTON_VIOLET:
        unit = "M ohms";
        break;
    case VIRT_BUTTON_GREY:
    case VIRT_BUTTON_WHITE:
        unit = "G ohms";
        break;
    case VIRT_BUTTON_SILVER:
        unit = " m";
        break;

    default:
        // Not supposed to be here
        return;
    }

    // Determine the tolerance
    switch (bandValues[3])
    {
    case VIRT_BUTTON_BROWN:
        tolerance_str = "1%";
        break;
    case VIRT_BUTTON_RED:
        tolerance_str = "2%";
        break;
    case VIRT_BUTTON_GREEN:
        tolerance_str = "0.5%";
        break;
    case VIRT_BUTTON_BLUE:
        tolerance_str = "0.25%";
        break;
    case VIRT_BUTTON_VIOLET:
        tolerance_str = "0.1%";
        break;
    case VIRT_BUTTON_GREY:
        tolerance_str = "0.05%";
        break;
    case VIRT_BUTTON_GOLD:
        tolerance_str = "5%";
        break;
    case VIRT_BUTTON_SILVER:
        tolerance_str = "10%";
        break;
    default:
        // Invalid tolerance selected, just ignore
        break;
    }

    displayValue = resistance * multiplier;

    dtostrf(displayValue, 1, 1, displayValueStr);
    sprintf(resistance_str_serial, "%s%s ±%s", displayValueStr, unit, tolerance_str);
    sprintf(resistance_str_lcd, "%s%s ", displayValueStr, unit);

    // Print the calculated resistance with appropriate unit
    Serial.print("Calculated Resistance: ");
    Serial.println(resistance_str_serial);

    // create a new character
    lcd->createChar(0, (uint8_t *)plus_minues_char);

    lcd->setCursor(0, 1);
    lcd->print(resistance_str_lcd);
    // Print plus minues charachter
    lcd->write(byte(0));
    lcd->print(tolerance_str);
}
