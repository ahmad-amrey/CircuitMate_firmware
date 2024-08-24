#ifndef GAME_GUESS_THE_NUMBER_H
#define GAME_GUESS_THE_NUMBER_H

#include "calculator_app.h"

class GameGuessTheNumber : public CalculatorApp
{
public:
    GameGuessTheNumber(LiquidCrystal *lcd_instance);
    char *get_app_name(void) override;
    void screenApp(VirtualButton pressed_btn, bool is_pressed) override;
    void resetState() override;

private:
    void resetTry();
    void generateRandomNumber();
    void checkGuess();
    void updateGuess();
    int targetNumber;      // The number to guess
    uint8_t currentGuessTens;  // The player's current tens digit
    uint8_t currentGuessUnits; // The player's current units digi
    bool selectingTens;    // Flag to indicate which digit is being selected
};

#endif // GAME_GUESS_THE_NUMBER_H