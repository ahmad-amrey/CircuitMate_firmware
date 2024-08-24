#include "resistor_smd_4_digits_calc.h"

ResistorSMD4DigitsCalc::ResistorSMD4DigitsCalc(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

char *ResistorSMD4DigitsCalc::get_app_name(void)
{
    return "Res SMD 4 digits";
}

void ResistorSMD4DigitsCalc::screenApp(VirtualButton pressed_btn, bool is_pressed)
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
    if (digitIndex >= 4)
    {
        calculateResistorValue();
        digitIndex = 0;                                // Reset for the next calculation
        memset(digitsValues, 0, sizeof(digitsValues)); // Initialize the array
    }
}

void ResistorSMD4DigitsCalc::resetState()
{
    digitIndex = 0;
    memset(digitsValues, 0, sizeof(digitsValues)); // Initialize the array
}

bool ResistorSMD4DigitsCalc::is_valid_btn(VirtualButton pressed_btn, uint8_t indx)
{
    bool valid = false;
    VirtualButton max_allowed = VIRT_BUTTON_0_BLACK;

    switch (indx)
    {
    case 0:
    case 3:
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
    // R is acepted here (Unless idx 1 is already R)
    case 2:
        max_allowed = VIRT_BUTTON_SILVER;
        if ((digitsValues[1] == VIRT_BUTTON_GOLD) || (digitsValues[1] == VIRT_BUTTON_SILVER))
        {
            max_allowed = VIRT_BUTTON_9_WHITE;
        }
        if ((pressed_btn >= VIRT_BUTTON_0_BLACK) && (pressed_btn <= max_allowed))
        {
            valid = true;
        }
        break;

    default:
        break;
    }

    return valid;
}

void ResistorSMD4DigitsCalc::printDigits()
{
    char digits_str[5] = {0};
    size_t i = 0;

    // Fill all digits with question mark
    for (i = 0; i < 4; i++)
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

void ResistorSMD4DigitsCalc::calculateResistorValue()
{
    long resistance = 0;
    float multiplier = 0;
    float displayValue = resistance;
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

    // R is on digit 1
    if ((digitsValues[1] == VIRT_BUTTON_GOLD) || (digitsValues[1] == VIRT_BUTTON_SILVER))
    {
        resistance = ((digitsValues[0] * 100) + (digitsValues[2] * 10) + (digitsValues[3] * 1));
        multiplier = 0.01;
        unit = " ";
    }
    // R is on digit 2
    else if ((digitsValues[2] == VIRT_BUTTON_GOLD) || (digitsValues[2] == VIRT_BUTTON_SILVER))
    {
        resistance = ((digitsValues[0] * 100) + (digitsValues[1] * 10) + (digitsValues[3] * 1));
        multiplier = 0.1;
        unit = " ";
    }
    else
    {
        // Calculate resistance based on selected bands
        resistance = ((digitsValues[0] * 100) + (digitsValues[1] * 10) + (digitsValues[2] * 1));

        // Find multiplier
        switch (digitsValues[3])
        {
        // 1s
        case VIRT_BUTTON_1_BROWN:
        case VIRT_BUTTON_4_YELLOW:
        case VIRT_BUTTON_7_VIOLET:
            multiplier = 0.01;
            break;
        // 10s
        case VIRT_BUTTON_2_RED:
        case VIRT_BUTTON_5_GREEN:
        case VIRT_BUTTON_8_GREY:
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
        switch (digitsValues[3])
        {

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
    }

    displayValue = resistance * multiplier;

    dtostrf(displayValue, 1, 2, displayValueStr);
    sprintf(resistance_str_lcd, "%s%s", displayValueStr, unit);

    // create a new character
    lcd->createChar(0, (uint8_t *)plus_minues_char);
    lcd->createChar(1, (uint8_t *)ohms_char);

    lcd->setCursor(0, 1);
    lcd->print(resistance_str_lcd);
    lcd->write(byte(1));
}
