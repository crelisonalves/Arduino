#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
 
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
 
 
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
 
 
void setup()   {
  Serial.begin(9600);
 
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done
 
  display.display();
  
 
  // Clear the buffer.
  display.clearDisplay();
 
  // text display tests

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(35,18);  // display.setCursor(x,y-canto inferior do texto)
  display.print("Kelu & Bel");
  display.display();
  delay(1500);

 
}
 
 
void loop() {

    delay(1500);
  display.clearDisplay();
    display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,12);  // display.setCursor(x,y)
  display.print("Kelu");
  display.display();
  display.clearDisplay();
  display.clearDisplay();  
  delay(1500);
  display.clearDisplay();
    display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(50,12);  // display.setCursor(x,y)
  display.print("Bel");
  display.display();
  display.clearDisplay();
  display.clearDisplay();
    delay(1500);
 
 
}
 
