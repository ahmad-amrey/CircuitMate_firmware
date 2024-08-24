
# CircuitMate Firmware

## Description

CircuitMate is a portable, handheld multifunctional electronic tool designed to assist you in your electronics lab. This device is capable of performing various calculations for resistors, capacitors, and SMD components. It also features a built-in guessing game for added entertainment during your electronics projects.

The 3D files and related details can be found on this [Thingiverse page](https://www.thingiverse.com/thing:6741874).

## Features

- **Resistor Calculators:**
  - DIP resistor calculator (4-Band and 5-Band)
  - SMD resistor calculator (3-digit and 4-digit)
- **Capacitor Calculator:**
  - Supports 3-digit ceramic and SMD capacitors
- **Games:**
  - Number guessing game
- **User Interface:**
  - LCD display for output
  - Potentiometer and push button for input

## Knob Calibration Instructions

The potentiometer (knob) in CircuitMate may require calibration to ensure it accurately registers its position across all available steps. Typically, this needs to be done once after printing the knob label file (available on the [Thingiverse page](https://www.thingiverse.com/thing:6741874)).

### Enabling Calibration Mode

To enable calibration mode:

1. Uncomment the following macro:
   ```cpp
    #define BUILD_FOR_CALIBRATION
   ```
2. Upload the Arduino code.
3. The device will now be in calibration mode. As you rotate the knob, the device will print the corresponding analog values to the Serial monitor.
4. Open the Serial Monitor in the Arduino IDE (or your preferred serial interface) to view the output.
5. The values will change as the knob moves through its positions. These values represent the boundaries between different virtual buttons mapped to the knob's positions.
6. Identify the analog value at the beginning region of each button's range. This value will correspond to the start of a new button's region.
7. Update the `analog_v_btn_mapping[]` array in the firmware code with these values. The array should contain the analog values in increasing order, each representing the threshold for a button's position.
8. After updating the array, disable calibration mode by commenting out or removing the line:
   ```cpp
   // #define BUILD_FOR_CALIBRATION
   ```
## CircuitMate Apps

CircuitMate operates on an app-based architecture, where each calculator or game is implemented as an "app." These apps inherit from the `CalculatorApp` base class, which provides a standard interface for interacting with the device’s hardware and user inputs.

### Changing the Default App

Whenever CircuitMate starts, it loads the app defined in:
// Set initial app
selected_app = &res_4_calc;

If you want another app to start at boot-up, simply change this to the app you need.

### How to Implement a New App

1. **Create a New Class**: 
   - Copy the closest existing app to the one you want to build (e.g., `resistor_4_bands_calc.cpp` and `resistor_4_bands_calc.h`).
   - Implement the necessary functions, such as `screenApp`, `resetState`, and `get_app_name`.
   - Create an instance under `// Initialize apps` in `CircuitMate_calc.ino`.
   - In the function `refresh_screen_with_menu()`, update the following:
     - Adjust `MAX_APPS` to include your new app.
     - Assign a button for it inside the `switch (selected_btn)` block.
