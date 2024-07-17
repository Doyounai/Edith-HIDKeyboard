#include <ButtonDebounce.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h> // library required for IIC communication

#define inputCLK 2
#define inputDT 3
#define inputSW 6

// Button timing variables
#define debounce 20
#define double_click_gap 250
#define holdTime 1000

int volume = 20;

void buttonSetup();
void buttonUpdate();

void rotaryEncoderSetup();
void rotaryEncoderUpdate();

void displaySetup();
void displayUpdate();

void setup() {
  buttonSetup();
  rotaryEncoderSetup();
  displaySetup();

  Serial.begin(9600);
}

void loop() {
  buttonUpdate();
  rotaryEncoderUpdate();
  displayUpdate();
}
