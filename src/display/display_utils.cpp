#include <Wire.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_SSD1306.h> // Hardware-specific library
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "connections/wifi_utils.h"
#include "display_utils.h"

WiFiUDP udp;

// GMT+7 = 25200 seconds
NTPClient timeClient(udp, "pool.ntp.org", 25200, 60000);

WifiUtils wifi;

// SDA PIN and SCL PIN definitions for the I2C bus
#define SDA_PIN 18
#define SCL_PIN 19

// DEFINITIONS
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The "-1" means we don't use a reset pin.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupOLED();
void drawTopLine();
void drawTime();
void drawMeasurements();
String getTwoDigits(int number);
String getPmAm(int hours);
void initDisplay();
void renderOLED();

void setupOLED()
{
  // Clear the buffer (the screen starts with the Adafruit logo by default)
  display.clearDisplay();
  display.setRotation(1); // Rotate display if needed

  // SETTING UP TEXT
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.setCursor(1, 8);

  drawTopLine();
  drawTime();
  drawMeasurements();
  // RENDER (Crucial Step!)
  // Adafruit libraries use a "buffer". Nothing appears until you call display()
  display.display();
}

void drawTopLine()
{
  display.drawLine(0, 7, 31, 7, 1);
  display.drawLine(1, 4, 4, 1, 1);
  display.drawLine(7, 4, 10, 1, 1);
  display.drawLine(21, 4, 24, 1, 1);
  display.drawLine(14, 4, 17, 1, 1);
  display.drawLine(27, 4, 30, 1, 1);
}

void drawTime()
{
  // draw AM/PM
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.setCursor(1, 12);
  display.setTextSize(1);
  display.print(getPmAm(timeClient.getHours()));
  // draw hours
  display.setTextSize(2);
  display.setCursor(5, 24);
  display.print(getTwoDigits(timeClient.getHours()));

  // draw minutes
  display.setTextSize(2);
  display.setCursor(5, 46);
  display.print(getTwoDigits(timeClient.getMinutes()));

  // draw seconds
  display.setTextSize(2);
  display.setCursor(5, 68);
  display.print(getTwoDigits(timeClient.getSeconds()));
}

String getTwoDigits(int number)
{
  if (number < 10)
  {
    return "0" + String(number);
  }
  return String(number);
}

String getPmAm(int hours)
{
  if (hours >= 12)
  {
    return "PM";
  }
  return "AM";
}

void drawMeasurements()
{
  display.drawLine(0, 90, 31, 90, 1);

  // draw celsius
  display.drawCircle(20, 96, 1, 1);
  display.setTextSize(1);
  display.setCursor(23, 96);
  display.print("C");
  display.setCursor(4, 96);
  display.print("26");

  // draw humidity
  display.setCursor(4, 108);
  display.print("24 %");
}

void DisplayUtils::initDisplay() {
 wifi.connect();
    // Initialize I2C with custom SDA and SCL pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // Address 0x3C is standard for 128x64 OLEDs
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}

bool timeSet = false;
void DisplayUtils::renderDisplay()
{
  // Clear the buffer (the screen starts with the Adafruit logo by default)
   if (wifi.isConnected() && !timeSet)
  {
    timeSet = true;
    timeClient.begin();
  }

  if (timeSet)
  {
    // draw seconds
    // Update time from internet
    timeClient.update();
    setupOLED();
    Serial.println("Current time: " + timeClient.getFormattedTime());
  }

  delay(1000);
}