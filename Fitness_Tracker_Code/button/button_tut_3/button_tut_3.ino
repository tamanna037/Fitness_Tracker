//////////////////////////////////////////////////////////////////////////////
// Arduino button tutorial 3.
//
// Demonstrates:
// - detection of releasing edge and deglitching
// - state variables for iterative update call
// - distinction of short press and long press
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

#define LONGPRESS_LEN    25  // Min nr of loops for a long press
#define DELAY            20  // Delay per loop in ms

//////////////////////////////////////////////////////////////////////////////

enum { EV_NONE=0, EV_SHORTPRESS, EV_LONGPRESS };

boolean button_was_pressed; // previous state
int button_pressed_counter; // press running duration

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // pull-up
  Serial.begin(9600);
  button_was_pressed = false;
  button_pressed_counter = 0;
}

int handle_button()
{
  int event;
  int button_now_pressed = !digitalRead(BUTTON_PIN); // pin low -> pressed

  if (!button_now_pressed && button_was_pressed) {
    //Serial.print(button_pressed_counter);
    Serial.print(button_pressed_counter);
    if (button_pressed_counter < LONGPRESS_LEN){
       event = EV_SHORTPRESS;
       
       Serial.print("hi");
    }
    else{
      event = EV_LONGPRESS;
             Serial.print("hello");

    }
    button_pressed_counter = 0;
  }
  else
    event = EV_NONE;
  
  if (button_now_pressed)
    ++button_pressed_counter;
    
//  if(button_pressed_counter !=0) Serial.print(button_pressed_counter);
  button_was_pressed = button_now_pressed;
  return event;
}

void loop()
{
  // handle button
  int event = handle_button();
//         Serial.print(event);


  // do other things
  switch (event) {
    case EV_NONE:
      Serial.print(".");
      break;
    case EV_SHORTPRESS:
      Serial.print("S");
      break;
    case EV_LONGPRESS:
      Serial.print("L");
      break;
  }

  // add newline sometimes
  static int counter = 0;
  if ((++counter & 0x3f) == 0)
    Serial.println();

  delay(DELAY);
}
