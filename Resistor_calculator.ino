/*
 * File: Resistor_calculator.ino
 * Author: Ahmad AlAmrey (ahmad.amrey@gmail.com)
 * Date: August 18, 2024
 * Description: This code reads an analog value from pin A0 
 *              whenever a pulled-up digital input on pin D0 is pressed (active low).
 */

// Define pin numbers
const int analogPin = A0;  // Pin connected to the analog sensor
const int buttonPin = 2;   // Pin connected to the button (active low)

// define other constants
typedef enum {
  VIRT_BUTTON_BLACK = 0,
  VIRT_BUTTON_BROWN,
  VIRT_BUTTON_RED,
  VIRT_BUTTON_ORANGE,
  VIRT_BUTTON_YELLOW,
  VIRT_BUTTON_GREEN,
  VIRT_BUTTON_BLUE,
  VIRT_BUTTON_VIOLET,
  VIRT_BUTTON_GREY,
  VIRT_BUTTON_WHITE,
  VIRT_BUTTON_GOLD,
  VIRT_BUTTON_SILVER,
  VIRT_BUTTON_OPTION,
  VIRT_BUTTON_CANCEL,
  VIRT_BUTTON_MAX
} v_buttons_t;

/*
 * Each virtual button has a minimum threshold value
 * e.g. Black = 0, Brown = 6 => if the potentionmeter read value is between 0 and
 * less than 6, it reads black, if it is 6 and less than the red threshold, then it is red
 */
const uint16_t analog_v_btn_mapping[VIRT_BUTTON_MAX] = {
  0,
  6,
  85,
  174,
  260,
  340,
  420,
  490,
  570,
  645,
  720,
  805,
  900,
  975,
};

const char *btn_names[VIRT_BUTTON_MAX] = {
  "BLACK",
  "BROWN",
  "RED",
  "ORANGE",
  "YELLOW",
  "GREEN",
  "BLUE",
  "VIOLET",
  "GREY",
  "WHITE",
  "GOLD",
  "SILVER",
  "OPTION",
  "CANCEL"
};

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
const unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Variable to store the analog value
int analogValue = 0;

void setup() {
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);

  // Set the button pin as an input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize the analog pin as an input (optional, as it's default)
  pinMode(analogPin, INPUT);
}

void loop() {
  static int lastButtonState = LOW;  // the previous reading from the input pin
  static int buttonState;            // the current reading from the input pin
  static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled


  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        process_btn_press();
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}


void process_btn_press() {
  v_buttons_t selected_btn = VIRT_BUTTON_BLACK;
  // Read the analog value from pin A0
  analogValue = analogRead(analogPin);

  if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_BROWN])
  {
    selected_btn = VIRT_BUTTON_BLACK;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_RED])
  {
    selected_btn = VIRT_BUTTON_BROWN;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_ORANGE])
  {
    selected_btn = VIRT_BUTTON_RED;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_YELLOW])
  {
    selected_btn = VIRT_BUTTON_ORANGE;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_GREEN])
  {
    selected_btn = VIRT_BUTTON_YELLOW;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_BLUE])
  {
    selected_btn = VIRT_BUTTON_GREEN;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_VIOLET])
  {
    selected_btn = VIRT_BUTTON_BLUE;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_GREY])
  {
    selected_btn = VIRT_BUTTON_VIOLET;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_WHITE])
  {
    selected_btn = VIRT_BUTTON_GREY;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_GOLD])
  {
    selected_btn = VIRT_BUTTON_WHITE;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_SILVER])
  {
    selected_btn = VIRT_BUTTON_GOLD;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_OPTION])
  {
    selected_btn = VIRT_BUTTON_SILVER;
  } 
  else if(analogValue < analog_v_btn_mapping[VIRT_BUTTON_CANCEL])
  {
    selected_btn = VIRT_BUTTON_OPTION;
  } 
  else
  {
    selected_btn = VIRT_BUTTON_CANCEL;
  }

  // Print the analog value to the serial monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(", Button is:");
  Serial.println(btn_names[selected_btn]);
}