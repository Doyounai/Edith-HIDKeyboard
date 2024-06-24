#include <String.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int x = 0;
int y = 0;

int timeTextX = 55;
int timeTextY = 15;
int timeTextWidth = 0;
int timeTextHeight = 0;
float timeTextSize = 3.5;

int dateTextY = 42;
int dateTextWidth = 0;
int dateTextHeight = 0;
float dateTextSize = 1.7;

String read = "";

int min = 0;
int second = 0;

void drawTimeText() {
  display.setTextSize(timeTextSize);                              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);                            // Draw white text
  display.setCursor(timeTextX - (timeTextWidth / 2), timeTextY);  // Start at top-left corner

  min = (millis() / 1000) / 60;
  second = millis() / 1000;

  // Create a String object with enough space for 4 characters (including null terminator)
  String formattedString = "";

  // Use sprintf to format the number with leading zero (maximum width 2)
  sprintf(formattedString.c_str(), "%02d:%02d", min, second);

  display.println(formattedString.c_str());
}

void drawDateText() {
  display.setTextSize(dateTextSize);                              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);                            // Draw white text
  display.setCursor(timeTextX - (dateTextWidth / 2), dateTextY);  // Start at top-left corner
  // display.println(F("24 June 2024"));
  if (read != "") {
    display.println(read);
  } else {
    display.println("-- -- ----");
  }
}

void drawVolumeBar() {
  float mvolume = volume;
  mvolume = mvolume / 100;

  display.fillRect(112, 63 - (62 * mvolume), 15, (62 * mvolume), 1);
  display.drawRect(111, 0, 17, 64, 1);
}

void updateDisplay() {
  display.clearDisplay();
  drawTimeText();
  drawDateText();
  drawVolumeBar();
  display.display();
}

void displaySetup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.clearDisplay();

  display.setTextSize(timeTextSize);
  display.getTextBounds("xx:xx", 0, 0, &x, &y, &timeTextWidth, &timeTextHeight);

  display.setTextSize(dateTextSize);
  display.getTextBounds("xx xxxxxx xxxx", 0, 0, &x, &y, &dateTextWidth, &dateTextHeight);

  updateDisplay();
}

void displayUpdate() {
  // updateDisplay();
}