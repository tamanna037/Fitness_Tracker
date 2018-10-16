//////////////////////////////////////////////////////////////////////////////
// Arduino button tutorial 2.
//
// Demonstrates:
// - detection of pressing edge and deglitching
// - state variable for iterative update call
//
// Push-button must be connected as follows:
//              __,__
//   Pin2 ------o   o------ GND
//
// (C) 2011 By P. Bauermeister
// This example code is in the public domain.
//////////////////////////////////////////////////////////////////////////////

// Adapt these to your board and application timings:

#define BUTTON_PIN        0  // Button
#define DELAY            20  // Delay per loop in ms

//////////////////////////////////////////////////////////////////////////////

boolean button_was_pressed; // previous state

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // pull-up
  Serial.begin(9600);
  button_was_pressed = false;
}

boolean handle_button()
{
  boolean event;
  int button_now_pressed = !digitalRead(BUTTON_PIN); // pin low -> pressed

  event = button_now_pressed && !button_was_pressed;
  button_was_pressed = button_now_pressed;
  return event;
}

void loop()
{
  // handle button
  boolean raising_edge = handle_button();

  // do other things
  Serial.print(raising_edge ? "^" : ".");

  // add newline sometimes
  static int counter = 0;
  if ((++counter & 0x3f) == 0)
    Serial.println();

  delay(DELAY);
}
