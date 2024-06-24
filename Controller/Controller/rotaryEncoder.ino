// Media and volume variables
int next = 0;
int prev = 0;
int previousStateCLK;
int currentStateCLK;

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
  pinMode(inputCLK, INPUT);
  pinMode(inputDT, INPUT);
  pinMode(inputSW, INPUT_PULLUP);

  digitalWrite(inputSW, HIGH);
  previousStateCLK = digitalRead(inputCLK);
}

void rotaryEncoderUpdate() {
  // Read the current state of inputCLK
  currentStateCLK = digitalRead(inputCLK);
  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (currentStateCLK != previousStateCLK) {
    // If the inputDT state is different than the inputCLK state then
    // the encoder is rotating counterclockwise
    // Serial.print("DT :");
    // Serial.print(digitalRead(inputDT));
    // Serial.print("| CLK :");
    // Serial.println(currentStateCLK);


    if (digitalRead(inputDT) != currentStateCLK) {
      rotateRight();
    } else {
      rotateLeft();
    }
    // Update previousStateCLK with the current state
    previousStateCLK = currentStateCLK;
    // Sends desired volume to serial monitor
    updateDisplay();
    Serial.println(volume);
  }
  // Checks what actions are performed
  int b = checkButton();
  if (b == 1) singleClick();
  if (b == 2) doubleClick();
}