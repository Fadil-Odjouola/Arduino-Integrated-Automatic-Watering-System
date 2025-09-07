/*********
  Complete project details at https://randomnerdtutorials.com
  
  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution. 
*********/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define ACTION_BUTTON D2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };





class MSTRsensor {
  int pin;
  int value;
  int u_threshold;
  int l_threshold;
  String status;
  
public:
  MSTRsensor(int p) : pin(p){
    pinMode(pin, INPUT);
    updateValue();
    Serial.println("New Moisture Sensor at pin - " + String(pin));
  }
  int read(){
    value = analogRead(pin);
    return value;
  }
  
  void setThreshold(int l, int u){
    if (l>u){
      Serial.println("Moisture Sensor parameters must be lowerthreshold, upperthreshold");
      return;
      }
    l_threshold = l;
    u_threshold = u;
  }
  bool isInRange(){
    if(value < l_threshold) {
      Serial.println("Moisture sensor is TOO DRY");
      return false;
    }
    if(value > u_threshold) {
      Serial.println("Moisture sensor is TOO MOIST");
      return false;
    }
    return true;
  }
  void updateStatus(){
    if(value < l_threshold) {
        status = "TOO DRY";
    }
    elseif(value > u_threshold) {
        status = "TOO MOIST";
    } else{ status = "OK"; }
  }
  void printStats(){
    String message = 
    "Pin - " + String(pin) + "\n" + 
    "Moisture - " + String(value) + "\n" + 
    "Dry Threshold - " + String(l_threshold) + "\n" + 
    "Moist Threshold - " + String(u_threshold) + "\n" + 
    "Status - " + status;
    Serial.println(message);
  }
};





  

void textBubble(int16_t x, int16_t y, int len, char text[], int16_t scale, int16_t color, int16_t bg, int16_t outlineColor /*, bool alignCenter, bool fillRect*/) {
  int16_t i;

  int charSizex = 5 *  scale;
  int charSizey = 7 * scale;

  int lowerBoundx = x;                 // Start of the text bubble border along x axis
  int upperBoundx = len + x;           // End of the text bubble border along x axis
  int upperBoundy = y;                 // Start of the text bubble border along y axis
  int lowerBoundy = display.height();  /* End of the text bubble border along y axis, display.height is used a placeholder and if left unchanged will
                                          render the bottom of the border at the bottom of the screen */
                                          
  // Space between text inside and the border
  int paddingx = 5; 
  int paddingy = 5; 
  
  int innerLBX = lowerBoundx + paddingx; // Just like upperBoundx, lowerBoundy, etc. Except for the text itself
  int innerUBX = upperBoundx - paddingx; 
  int innerUBY = upperBoundy + paddingy;
  
  int textBoxSpace = innerUBX - innerLBX;     // Space the text has availible in a given layer
  int charPad = 1;                            // Spacing inbetween each character in series
  int charSpace = 2;                          // Spacing inbetween each layer of characters
  int totCharSpace = charSpace + charSizey;   // Pixel size per increment for character height + padding inbetween layers
  int totCharLen = charSizex + charPad;       // Pixel size per increment for character length + padding inbetween characters

  int iy = -1; // Count for how many layer there are
  int ay = 1; // Count for if there's an impartial layer

  // Loop for drawing text onto screen
  display.clearDisplay();
  for(i=0; i<strlen(text); i++){
    ay = 1; // When writing character in a layer, the partial layer count is set to 1
    
    // Check if pixel sized increment overlaps the border, add 1 to layer count and sets partial layer count to zero
    if((((i*totCharLen)%(textBoxSpace)) < totCharLen)){
      iy++;
      ay = 0;
    }

    // Draw chars from the start of the test coordinate plus the pixel sized increment coordinates. Same for Y except pixel size increment coordinate is calculated using layer count
    // Ofcourse, each increment runs thrugh each character in the text array, each one draws that character at its index
    display.drawChar(innerLBX + ((i*totCharLen)%(textBoxSpace)) , innerUBY + (totCharSpace*iy), text[i], color, bg, scale);
    display.display();
    delay(100);
  }

  // The bottom y coordinate of the text bubble border is the total layers multiplied by the pixel size per layer plus padding and starting y coordinate
  lowerBoundy = ((ay+iy)*totCharSpace) + (paddingy*2) + y;

  // width and height of the text bubble border is the coordinate of the upper part minus the coordinate of the lower part
  int bubbleWidth = upperBoundx - lowerBoundx;  
  int bubbleHeight = lowerBoundy - upperBoundy;

  // a round rectangle aka text bubble border is the x,y coordinates(top left corner) and the calculated width and height
  display.drawRoundRect(x, y, bubbleWidth, bubbleHeight, 5, outlineColor);
  display.display();
  delay(3000);
  
}


void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Attach a digital innterupt pin that calls a screen invert function ISR
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), invertScreen, RISING);
  
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  
  // Clear the buffer
  display.clearDisplay();

  testmenu();

  textBubble(2,2,100, "Hello World, what function do you want to run today?", 1, WHITE, BLACK, WHITE);
  
  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  
  */
}

void loop() {
  // Draws the main menu
  display.
}

char message[] = "Hello World! Starting program...";



void testmenu() {
  int16_t i;
  Serial.println(display.height());
  Serial.println(display.width());
  display.clearDisplay();
  for(i=0; i<display.height(); i++){
    
    display.drawLine(0,i, display.width()-1, i, WHITE);
    display.display();
    if(i%2 == 1){
      display.invertDisplay(true);
      
    } else{
      display.invertDisplay(false);
    }
  }
  display.invertDisplay(false);
  display.clearDisplay();
  display.drawRoundRect(10, 10, display.width()-20, 30, 5, WHITE);
  display.display();
  delay(3000);
  int y = -1;
  char text[] = "Hello World! Starting program...";
  for(i=0; i<strlen(text); i++){
    if((((i*6)%(display.width()-30)) < 6)){
      y++;
    }
    display.drawChar(15+((6*i)%(display.width()-30)), 15+(10*y), text[i], WHITE, BLACK, 1);
    display.display();
    delay(400);
  }
  
  delay(3000);
  
}

void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {d
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}
