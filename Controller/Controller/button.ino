ButtonDebounce sw1B(7, 50);
ButtonDebounce sw2B(8, 50);
ButtonDebounce sw3B(4, 50);
ButtonDebounce sw4B(5, 50);

void sw1Change(int state) {
  if (state == 0) {
    Serial.println("sw1");
  }
}

void sw2Change(int state) {
  if (state == 0) {
    Serial.println("sw2");
  }
}

void sw3Change(int state) {
  if (state == 0) {
    Serial.println("sw3");
  }
}

void sw4Change(int state) {
  if (state == 0) {
    Serial.println("sw4");
  }
}

void buttonSetup() {
  sw1B.setCallback(sw1Change);
  sw2B.setCallback(sw2Change);
  sw3B.setCallback(sw3Change);
  sw4B.setCallback(sw4Change);
}

void buttonUpdate() {
  sw1B.update();
  sw2B.update();
  sw3B.update();
  sw4B.update();
}
