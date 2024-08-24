#include "resistor_5_bands_calc.h"

Resistor5BandsCalc::Resistor5BandsCalc(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

char *Resistor5BandsCalc::get_app_name(void)
{
    return "Resistor 5 bands";
}

void Resistor5BandsCalc::screenApp(VirtualButton pressed_btn, bool is_pressed)
{
    if (!is_pressed)
    {
        // Nothing to do here, just return
        return;
    }

    if ((pressed_btn == VIRT_BUTTON_CANCEL) ||
        (pressed_btn == VIRT_BUTTON_NONE))
    {
        resetState();
        Serial.println("Cancel requested");
        printBands();
        return;
    }

    if (!is_valid_btn(pressed_btn, bandIndex))
    {
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

bool Resistor5BandsCalc::is_valid_btn(VirtualButton pressed_btn, uint8_t indx)
{
    bool valid = false;

    switch (indx)
    {
    case 0:
    case 1:
    case 2:
        if ((pressed_btn >= VIRT_BUTTON_0_BLACK) && (pressed_btn <= VIRT_BUTTON_9_WHITE))
        {
            valid = true;
        }
        break;

    case 3:
        if ((pressed_btn >= VIRT_BUTTON_0_BLACK) && (pressed_btn <= VIRT_BUTTON_SILVER))
        {
            valid = true;
        }
        break;
    case 4:
        switch (pressed_btn)
        {
        case VIRT_BUTTON_1_BROWN:
        case VIRT_BUTTON_2_RED:
        case VIRT_BUTTON_5_GREEN:
        case VIRT_BUTTON_6_BLUE:
        case VIRT_BUTTON_7_VIOLET:
        case VIRT_BUTTON_8_GREY:
        case VIRT_BUTTON_GOLD:
        case VIRT_BUTTON_SILVER:
            valid = true;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return valid;
}

void Resistor5BandsCalc::printBands()
{
    char digits_str[6] = {0};
    size_t i = 0;

    // Fill all digits with question mark
    for (i = 0; i < 5; i++)
    {
        digits_str[i] = '?';
    }

    // Override digits with its values
    for (i = 0; i < bandIndex; i++)
    {
        if ((bandValues[i] == VIRT_BUTTON_GOLD))
        {
            digits_str[i] = 'G';
        }
        else if ((bandValues[i] == VIRT_BUTTON_SILVER))
        {
            digits_str[i] = 'S';
        }
        else
        {
            digits_str[i] = '0' + bandValues[i];
        }
    }

    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(digits_str);
}

void Resistor5BandsCalc::calculateResistorValue()
{
    long resistance = 0;
    float multiplier = 0;
    float displayValue = resistance;
    const char *unit = "";
    const char *tolerance_str = "";
    bool valid_tolerance = false;
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

    // make ohms characters:
    const static uint8_t ohms_char[8] = {
        0b00000,
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b11011,
        0b00000};

    // Calculate resistance based on selected bands
    resistance = ((bandValues[0] * 100) + (bandValues[1] * 10) + (bandValues[2] * 1));

    // Find multiplier
    switch (bandValues[3])
    {
    // 1s
    case VIRT_BUTTON_1_BROWN:
    case VIRT_BUTTON_4_YELLOW:
    case VIRT_BUTTON_7_VIOLET:
    case VIRT_BUTTON_SILVER:
        multiplier = 0.01;
        break;
    // 10s
    case VIRT_BUTTON_2_RED:
    case VIRT_BUTTON_5_GREEN:
    case VIRT_BUTTON_8_GREY:
    case VIRT_BUTTON_GOLD:
        multiplier = 0.1;
        break;
    // 100s
    case VIRT_BUTTON_0_BLACK:
    case VIRT_BUTTON_3_ORANGE:
    case VIRT_BUTTON_6_BLUE:
    case VIRT_BUTTON_9_WHITE:
        multiplier = 1.0;
        break;
    default:
        // Not supposed to be here
        return;
    }

    // Determine the appropriate unit
    switch (bandValues[3])
    {

    case VIRT_BUTTON_SILVER:
    case VIRT_BUTTON_GOLD:
    case VIRT_BUTTON_0_BLACK:
        unit = " ";
        break;
    case VIRT_BUTTON_1_BROWN:
    case VIRT_BUTTON_2_RED:
    case VIRT_BUTTON_3_ORANGE:
        unit = " k";
        break;
    case VIRT_BUTTON_4_YELLOW:
    case VIRT_BUTTON_5_GREEN:
    case VIRT_BUTTON_6_BLUE:
        unit = " M";
        break;
    case VIRT_BUTTON_7_VIOLET:
    case VIRT_BUTTON_8_GREY:
    case VIRT_BUTTON_9_WHITE:
        unit = " G";
        break;

    default:
        // Not supposed to be here
        return;
    }

    // Determine the tolerance
    switch (bandValues[4])
    {
    case VIRT_BUTTON_1_BROWN:
        tolerance_str = "1%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_2_RED:
        tolerance_str = "2%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_5_GREEN:
        tolerance_str = "0.5%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_6_BLUE:
        tolerance_str = "0.25%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_7_VIOLET:
        tolerance_str = "0.1%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_8_GREY:
        tolerance_str = "0.05%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_GOLD:
        tolerance_str = "5%";
        valid_tolerance = true;
        break;
    case VIRT_BUTTON_SILVER:
        tolerance_str = "10%";
        valid_tolerance = true;
        break;
    default:
        // Invalid tolerance selected, just ignore
        break;
    }

    displayValue = resistance * multiplier;

    dtostrf(displayValue, 1, 2, displayValueStr);
    sprintf(resistance_str_serial, "%s%s ±%s", displayValueStr, unit, tolerance_str);
    sprintf(resistance_str_lcd, "%s%s", displayValueStr, unit);

    // Print the calculated resistance with appropriate unit
    Serial.print("Calculated Resistance: ");
    Serial.println(resistance_str_serial);

    // create a new character
    lcd->createChar(0, (uint8_t *)plus_minues_char);
    lcd->createChar(1, (uint8_t *)ohms_char);

    lcd->setCursor(0, 1);
    lcd->print(resistance_str_lcd);
    lcd->write(byte(1));
    if (valid_tolerance)
    {
        // Print plus minues charachter and tolerance value
        lcd->write(' ');
        lcd->write(byte(0));
        lcd->print(tolerance_str);
    }
}
