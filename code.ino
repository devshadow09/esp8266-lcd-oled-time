#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
LiquidCrystal_I2C lcd(0x27, 16, 2);

Timezone India;

unsigned long previousMillis = 0;
const long interval = 300000;
const long animationDuration = 30000;

unsigned long animationStartMillis = 0;
bool showingAnimation = false;

int x = 0;
int y = 16;
int xDirection = 1;
int yDirection = 1;

void setup() {
  Serial.begin(115200);
  WiFi.begin("Suresh Reddy", "sureshreddy@123");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  lcd.init();
  lcd.backlight();

  waitForSync();
  India.setLocation("Asia/Kolkata");

  Serial.println("Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();

  if (showingAnimation) {
    if (currentMillis - animationStartMillis >= animationDuration) {
      showingAnimation = false;
      previousMillis = currentMillis;
      display.clearDisplay();
      display.display();
    } else {
      displayAnimation();
    }
  } else {
    if (currentMillis - previousMillis >= interval) {
      showingAnimation = true;
      animationStartMillis = currentMillis;
    } else {
      displayDateTime();
    }
  }

  displayGreeting();

  delay(100);
}

void displayDateTime() {
  String dateTime = India.dateTime("l");
  String day = India.dateTime("d-M-y");

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(dateTime);
  display.setTextSize(1);
  display.setCursor(0, 18);
  display.println(day);

  display.display();
}

void displayGreeting() {
  lcd.clear();

  String lcdTime = India.dateTime("H:i:s");
  int hour = India.hour();
  String greeting;

  if (hour > 4 && hour < 12) {
    greeting = "Good Morning";
  } else if (hour >= 12 && hour < 16) {
    greeting = "Good Afternoon";
  } else if (hour >= 16 && hour < 20) {
    greeting = "Good Evening";
  } else {
    greeting = "Good Night";
  }

  lcd.setCursor(0, 0);
  lcd.print(lcdTime);
  lcd.setCursor(0, 1);
  lcd.print(greeting);
}

void displayAnimation() {
  display.clearDisplay();

  display.fillCircle(x, y, 3, SSD1306_WHITE);

  x += xDirection;
  y += yDirection;

  if (x <= 0 || x >= SCREEN_WIDTH - 1) xDirection = -xDirection;
  if (y <= 0 || y >= SCREEN_HEIGHT - 1) yDirection = -yDirection;

  display.display();
}
