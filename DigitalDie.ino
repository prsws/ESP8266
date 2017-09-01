/*
DigitalDie.io - Discrete LED electronic dice for ESP8266 NodeMCU
 
Copyright (c) 2017 Jose F. Reyes-Santana <jose.reyes@prswservices.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// definitions
// change the values for a different pinout - make sure they're nodemcu compliant!
                                  // GPIO    APP    (NodeMCU)
static const uint8_t PIN_ARRAY[8] = { 5,  // BUTTON (D1)
                                      16, // LED1 (D0)
                                      15, // LED2 (D8)
                                      14, // LED3 (D5)
                                      13, // LED4 (D7)
                                      12, // LED5 (D6)
                                      10, // LED6 (SD3)
                                      17  // RNDGEN (A0)
                                    };
                                    
static const uint8_t BUTTON = PIN_ARRAY[0];
static const uint8_t LED1 = PIN_ARRAY[1];
static const uint8_t LED2 = PIN_ARRAY[2];
static const uint8_t LED3 = PIN_ARRAY[3];
static const uint8_t LED4 = PIN_ARRAY[4];
static const uint8_t LED5 = PIN_ARRAY[5];
static const uint8_t LED6 = PIN_ARRAY[6];
static const uint8_t RNDGEN = PIN_ARRAY[7];

static const uint8_t ALL_OFF = 0;
static const int ANIMATION_DELAY = 150;
static const int EXIT_DELAY = 4000;

// Holds the current button state.
volatile int button_state = HIGH;
// The delay threshold for debounce checking in microseconds
const int DEBOUNCE_DELAY = 2000;

// setup code runs once upon every boot
void setup()
{
  // init usb
  Serial.begin(115200);
  
  // init pins
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(RNDGEN, INPUT);

  // debounce button: attach an interrupt to the pin, assign the onChange function as a handler and trigger on changes (LOW or HIGH).
  attachInterrupt(digitalPinToInterrupt(BUTTON), pinChange, CHANGE);
  
  // seed random generator using ADC with no input connected
  randomSeed(analogRead(RNDGEN));

  // turn output pins off
  display_die_face(ALL_OFF);

  Serial.println("Setup done.");
}

// main loop runs continuosly after setup
void loop()
{
  if (button_state == LOW) {
    Serial.println("Rolling dice.");
    run_animation();
    
    int winner = random(1,7);
    display_die_face(winner);
    Serial.print("Winner is: ");
    Serial.println(winner);    
    delay(EXIT_DELAY);
    display_die_face(ALL_OFF);
  }
  else {
    Serial.println("Waiting for dice roll.");
  }
  delay(500);
}

// Gets called by the interrupt.
void pinChange() {
  // Get the pin value.
  int pin_value = digitalRead(BUTTON);

  // debounce
  if(pin_value != button_state) {
    delayMicroseconds(DEBOUNCE_DELAY);  // cannot use delay() nor millis() inside ISR
    pin_value = digitalRead(BUTTON);
  }
  // persist the reading as the state.
  button_state = pin_value;
}


// run dice animation
void run_animation()
{
  int i, j;
  // shake in hand
  for (j = 1; j <= 2; j++) {
    for (i = 1; i <= 6; i++) {
      walk_led(i);
      delay(ANIMATION_DELAY);
    }
    walk_led(ALL_OFF);
    for (i = 6; i >= 1; i--) {
      walk_led(i);
      delay(ANIMATION_DELAY);
    }
  }
  // tumble
  for (i = 1; i <= 6; i++) {
    int tumbler = random(1,7);
    display_die_face(tumbler);
    delay(ANIMATION_DELAY * 2);
    display_die_face(ALL_OFF);
    delay(ANIMATION_DELAY * 2);
  }
  delay(ANIMATION_DELAY * 2);
}

// display die face (0=all off, 1-6=LED's on)
void display_die_face(int value)
{
  switch (value) {
    
    case 1:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 2:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 3:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 4:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 5:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, LOW);
      break;

    case 6:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      break;

    case 0:
    default: // turn output pins off
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;
  }
}

// display single walking LED (pos 1-6)
void walk_led(int position)
{
  switch (position) {
    
    case 1:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 2:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 3:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 4:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;

    case 5:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, LOW);
      break;

    case 6:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, HIGH);
      break;

    case 0:
    default: // turn output pins off
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      break;
  }
}

