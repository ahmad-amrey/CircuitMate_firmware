#include "capacitor_3_digits_calc.h"
/**
 * This calculator should work for both of SMD and ceramic DIP capacitors
 * (The only difference I noticed is the use of 'R' so just ignore it)
 */

Capacitor3DigitsCalc::Capacitor3DigitsCalc(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

char *Capacitor3DigitsCalc::get_app_name(void)
{
    return "Cap 3 digits";
}

void Capacitor3DigitsCalc::screenApp(VirtualButton pressed_btn, bool is_pressed)
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
        printDigits();
        return;
    }

    if (!is_valid_btn(pressed_btn, digitIndex))
    {
        return;
    }

    digitsValues[digitIndex] = pressed_btn;
    digitIndex++;

    printDigits();

    // Check if we have all digits selected
    if (digitIndex >= 3)
    {
        calculateCapacitorValue();
        digitIndex = 0;                                // Reset for the next calculation
        memset(digitsValues, 0, sizeof(digitsValues)); // Initialize the array
    }
}

void Capacitor3DigitsCalc::resetState()
{
    digitIndex = 0;
    memset(digitsValues, 0, sizeof(digitsValues)); // Initialize the array
}

bool Capacitor3DigitsCalc::is_valid_btn(VirtualButton pressed_btn, uint8_t indx)
{
    bool valid = false;

    switch (indx)
    {
    case 0:
    case 2:
        if ((pressed_btn >= VIRT_BUTTON_0_BLACK) && (pressed_btn <= VIRT_BUTTON_9_WHITE))
        {
            valid = true;
        }
        break;

    // R is acepted here
    case 1:
        if ((pressed_btn >= VIRT_BUTTON_0_BLACK) && (pressed_btn <= VIRT_BUTTON_SILVER))
        {
            valid = true;
        }
        break;

    default:
        break;
    }

    return valid;
}

void Capacitor3DigitsCalc::printDigits()
{
    char digits_str[4] = {0};
    size_t i = 0;

    // Fill all digits with question mark
    for (i = 0; i < 3; i++)
    {
        digits_str[i] = '?';
    }

    // Override digits with its values
    for (i = 0; i < digitIndex; i++)
    {
        if ((digitsValues[i] == VIRT_BUTTON_GOLD) || (digitsValues[i] == VIRT_BUTTON_SILVER))
        {
            digits_str[i] = 'R';
        }
        else
        {
            digits_str[i] = '0' + digitsValues[i];
        }
    }

    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(digits_str);
}

void Capacitor3DigitsCalc::calculateCapacitorValue()
{
    long capacitance = 0;
    float multiplier = 0;
    float displayValue = capacitance;
    const char *unit = "";
    char displayValueStr[40] = {0};
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

    // Handle the case were there is an 'R' characther
    if ((digitsValues[1] == VIRT_BUTTON_GOLD) || (digitsValues[1] == VIRT_BUTTON_SILVER))
    {
        capacitance = ((digitsValues[0] * 10) + digitsValues[2]);
        multiplier = 0.1;
        unit = " pF";
    }
    else
    {
        capacitance = ((digitsValues[0] * 10) + digitsValues[1]);
        // Find multiplier
        switch (digitsValues[2])
        {
        // 1s
        case VIRT_BUTTON_2_RED:
        case VIRT_BUTTON_5_GREEN:
        case VIRT_BUTTON_8_GREY:
            multiplier = 0.1;
            break;
        // 10s
        case VIRT_BUTTON_0_BLACK:
        case VIRT_BUTTON_3_ORANGE:
        case VIRT_BUTTON_6_BLUE:
        case VIRT_BUTTON_9_WHITE:
            multiplier = 1.0;
            break;
        // 100s
        case VIRT_BUTTON_1_BROWN:
        case VIRT_BUTTON_4_YELLOW:
        case VIRT_BUTTON_7_VIOLET:
            multiplier = 10.0;
            break;
        default:
            // Not supposed to be here
            return;
        }

        // Determine the appropriate unit
        switch (digitsValues[2])
        {
        case VIRT_BUTTON_0_BLACK:
        case VIRT_BUTTON_1_BROWN:
            unit = " pF";
            break;
        case VIRT_BUTTON_2_RED:
        case VIRT_BUTTON_3_ORANGE:
        case VIRT_BUTTON_4_YELLOW:
            unit = " nF";
            break;
        case VIRT_BUTTON_5_GREEN:
        case VIRT_BUTTON_6_BLUE:
        case VIRT_BUTTON_7_VIOLET:
            unit = " uF";
            break;
        case VIRT_BUTTON_8_GREY:
        case VIRT_BUTTON_9_WHITE:
            unit = " mF";
            break;

        default:
            // Not supposed to be here
            return;
        }
    }

    displayValue = capacitance * multiplier;

    dtostrf(displayValue, 1, 2, displayValueStr);
    sprintf(resistance_str_lcd, "%s%s", displayValueStr, unit);

    lcd->setCursor(0, 1);
    lcd->print(resistance_str_lcd);
}
