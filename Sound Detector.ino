#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long time = 0;
long oldTime = 0;
long counter = 0;
long avg;
long min = -1;
long max = -1;

void setup() {
  Serial.begin(9600);

  //initializing to 0x3C address
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  delay(2000);             // wait for initializing
  display.clearDisplay();  // clear display
  oldTime = millis();
}

void loop() {
  counter++;
  time = millis();

  if(min == -1)
    min = analogRead(A0);
  else if (min > analogRead(A0))
    min = analogRead(A0);

  if(max == -1)
    max = analogRead(A0);
  else if (max < analogRead(A0))
    max = analogRead(A0);
  avg += analogRead(A0);

  display.clearDisplay();
  display.drawRect(0, 0, 64, 32, WHITE);    // Main rectangle
  display.drawRect(0, 32, 42, 32, WHITE);   // Average rectangle
  display.drawRect(43, 32, 42, 32, WHITE);  // Min rectangle
  display.drawRect(86, 32, 42, 32, WHITE);  // Max rectangle

  //Top Box
  display.setTextSize(1);           // text size
  display.setTextColor(WHITE);      // text color
  display.setCursor(2, 2);          // position to display
  display.println(analogRead(A0));  // text to display
  display.setCursor(2, 15);
  display.println("Cur. Sound");  // text to display

  //Flowting hole
  display.setCursor(65, 2);  // position to display
  display.print((time - oldTime) / 60000);  // text to display
  display.print(" min");
  display.setCursor(65, 15);
  display.println("Uptime");  // text to display

  //Box 1
  display.setCursor(2, 34);               // position to display
  display.println(avg / counter);  // text to display
  display.setCursor(2, 45);
  display.println("Avg.");  // text to display

  //Box 2
  display.setCursor(45, 34);  // position to display
  display.println(min);  // text to display
  display.setCursor(45, 45);
  display.println("Min");  // text to display

  //Box 3
  display.setCursor(88, 34);  // position to display
  display.println(max);  // text to display
  display.setCursor(88, 45);
  display.println("Max");  // text to display

  display.display();
}