#include "game_guess_the_number.h"

GameGuessTheNumber::GameGuessTheNumber(LiquidCrystal *lcd_instance) : CalculatorApp(lcd_instance)
{
    // No additional initialization needed, all done in the Screen class
    resetState();
}

char *GameGuessTheNumber::get_app_name(void)
{
    return "Guess num Game";
}

void GameGuessTheNumber::screenApp(VirtualButton pressed_btn, bool is_pressed)
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
        return;
    }

    // Use knob positions to select digits 0-9
    if (pressed_btn >= VIRT_BUTTON_0_BLACK && pressed_btn <= VIRT_BUTTON_9_WHITE)
    {
        if (selectingTens)
        {
            currentGuessTens = (uint8_t)pressed_btn; // Convert to 0-9
            updateGuess();
            selectingTens = false;
        }
        else
        {
            currentGuessUnits = (uint8_t)pressed_btn; // Convert to 0-9
            updateGuess();
            checkGuess();
        }
    }
}

void GameGuessTheNumber::resetTry()
{
    currentGuessTens = UINT8_MAX; // Start at MAX
    currentGuessUnits = UINT8_MAX;
    selectingTens = true; // Start with the tens digit
    updateGuess();
}

void GameGuessTheNumber::resetState()
{
    lcd->clear();
    resetTry();
    generateRandomNumber();
}

void GameGuessTheNumber::generateRandomNumber()
{
    randomSeed(analogRead(0));     // Seed the random generator with noise from an unused analog pin
    targetNumber = random(0, 100); // Random number between 00 and 99
}

void GameGuessTheNumber::checkGuess()
{
    int guess = (currentGuessTens * 10) + (currentGuessUnits * 1);
    const char *too_low_str = "Too Low!        ";
    const char *too_high_str = "Too High!       ";
    const char *correct_str = "Correct!        ";
    const char *res_str = NULL;
    bool new_game = false;

    if (guess < targetNumber)
    {
        res_str = too_low_str;
        new_game = false;
    }
    else if (guess > targetNumber)
    {
        res_str = too_high_str;
        new_game = false;
    }
    else
    {
        res_str = correct_str;
        new_game = true;
    }

    lcd->setCursor(0, 1);
    lcd->print(res_str); // Print the result
    delay(2000);         // Pause for 2 seconds to show result

    lcd->setCursor(0, 1);
    lcd->print("            "); // Clear the second line after delay
    if (new_game)
    {
        resetState(); // Start a new game
    }
    else
    {
        resetTry(); // Prepare for next round
    }
}

void GameGuessTheNumber::updateGuess()
{
    char guess_str[17] = "Guess: ??       ";

    if (currentGuessTens != UINT8_MAX)
    {
        guess_str[7] = '0' + currentGuessTens;
    }
    if (currentGuessUnits != UINT8_MAX)
    {
        guess_str[8] = '0' + currentGuessUnits;
    }

    lcd->setCursor(0, 0);
    lcd->print(guess_str); // Clear the rest of the line
}
