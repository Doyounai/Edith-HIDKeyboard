#include <Arduino.h>
#include <RotaryEncoder.h>

RotaryEncoder *encoder = nullptr;

void checkPosition() {
  encoder->tick();  // just call tick() to check the state.
}

// Media and volume variables
// int next = 0;
// int prev = 0;
// int previousStateCLK;
// int currentStateCLK;

// Other button variables
boolean buttonVal = HIGH;           // reads value from button
boolean buttonLast = HIGH;          // buffered value of the button's previous state
boolean DCwaiting = false;          // whether we're waiting for a double click (down)
boolean DConUp = false;             // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;            // whether it's OK to do a single click
long downTime = -1;                 // time the button was pressed down
long upTime = -1;                   // time the button was released
boolean ignoreUp = false;           // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;          // when held, whether to wait for the up event
boolean holdEventPast = false;      // whether or not the hold event happened already
boolean longHoldEventPast = false;  // whether or not the long hold event happened already

volatile byte aFlag = 0;       // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0;       // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0;  //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0;   //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;     //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

void updateDisplay();

void singleClick() {
  Serial.println("playpause");
}

// Mutes the audio
void doubleClick() {
  Serial.println("mute");
}

// This function detects if the button is pressed once, twice or if you're holding it
int checkButton() {
  int event = 0;
  buttonVal = digitalRead(inputSW);
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce) {
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    if ((millis() - upTime) < double_click_gap && DConUp == false && DCwaiting == true) DConUp = true;
    else DConUp = false;
    DCwaiting = false;
  } else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce) {
    if (not ignoreUp) {
      upTime = millis();
      if (DConUp == false) DCwaiting = true;
      else {
        event = 2;
        DConUp = false;
        DCwaiting = false;
        singleOK = false;
      }
    }
  }
  if (buttonVal == HIGH && (millis() - upTime) >= double_click_gap && DCwaiting == true && DConUp == false && singleOK == true) {
    event = 1;
    DCwaiting = false;
  }
  if (buttonVal == LOW && (millis() - downTime) >= 1000) {
    if ((millis() - downTime) >= holdTime) {
      if (not longHoldEventPast) {
        waitForUp = true;
        ignoreUp = true;
        DConUp = false;
        DCwaiting = false;
        event = 3;
        longHoldEventPast = true;
      }
    }
  }
  buttonLast = buttonVal;
  return event;
}

// If rotated CCW it will decrease the volume(mode 1) or play previous song(mode 2)
void rotateLeft() {
  if (volume > 0) {
    volume = volume - 1;
  }
}

// If rotated CW it will increase the volume(mode 1) or play next song(mode 2)
void rotateRight() {
  if (volume < 100) {
    volume = volume + 1;
  }
}

void rotaryEncoderSetup() {
  encoder = new RotaryEncoder(inputCLK, inputDT, RotaryEncoder::LatchMode::TWO03);
  pinMode(inputSW, INPUT_PULLUP);

  digitalWrite(inputSW, HIGH);
  // previousStateCLK = digitalRead(inputCLK);

  attachInterrupt(digitalPinToInterrupt(inputCLK), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(inputDT), checkPosition, CHANGE);
}

static int pos = 0;

void rotaryEncoderUpdate() {
  encoder->tick();
  int newPos = encoder->getPosition();
  if (pos != newPos) {
    // Serial.print("pos:");
    // Serial.print(newPos);
    // Serial.print(" dir:");
    // Serial.println((int)(encoder->getDirection()));

    if ((int)(encoder->getDirection()) == -1) {
      rotateRight();
    } else {
      rotateLeft();
    }

    pos = newPos;
    Serial.println(volume);
  }

  int b = checkButton();
  if (b == 1) singleClick();
  if (b == 2) doubleClick();
}