//////////////////////////////////////////////////////////////////////////////
// Arduino button tutorial 1.
//
// Demonstrates:
// - detection of pressing state
//
// Push-button must be connected as follows:
//              __,__
//   Pin2 ------o   o------ GND
//
// (C) 2011 By P. Bauermeister
// This example code is in the public domain.
//
//////////////////////////////////////////////////////////////////////////////

// Adapt these to your board and application timings:

#define BUTTON_PIN        8  // Button
#define DELAY            20  // Delay per loop in ms

//////////////////////////////////////////////////////////////////////////////

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // pull-up
  Serial.begin(9600);
}

boolean handle_button()
{
  int button_pressed = !digitalRead(BUTTON_PIN); // pin low -> pressed
  return button_pressed;
}

void loop()
{
  // handle button
  boolean button_pressed = handle_button();

  // do other things
  Serial.print(button_pressed ? "^" : ".");

  // add newline sometimes
  static int counter = 0;
  if ((++counter & 0x3f) == 0)
    Serial.println();

  delay(DELAY);
}
