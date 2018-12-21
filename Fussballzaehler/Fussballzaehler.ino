#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#define sclk 24
#define mosi 23
#define cs   5
#define rst  6
#define dc   9
#define leftbt 10
#define rightbt 11
#define okbt 12

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

int maxpoints = 10;
int absmaxpoints = 99;
int absminpoints = 1;
int teamA = 0;
int teamB = 0;
bool victory = 0;

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

void setup(void) {

  pinMode(leftbt, INPUT_PULLUP);
  pinMode(rightbt, INPUT_PULLUP);
  pinMode(okbt, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.print("hello!");
  display.begin();

  Serial.println("init");
  uint16_t time = millis();
  display.fillScreen(BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  lcdTestPattern();
  delay(500);
}

void loop() {
  display.fillScreen(BLACK);
  display.setCursor(5, 20);
  display.setTextColor(RED);
  display.setTextSize(3);
  display.print("LEVIN");
  delay(1000);
  drawMaxPoints(1);
  
  while (digitalRead(okbt) == HIGH) {
    if (digitalRead(leftbt) == LOW) {
      if (maxpoints == absminpoints) {
        maxpoints = absmaxpoints;
      } else {
        maxpoints -= 1;
      }
      drawMaxPoints(1);
      delay(300);
    }
    if (digitalRead(rightbt) == LOW) {
      if (maxpoints == absmaxpoints) {
        maxpoints = absminpoints;
      } else {
        maxpoints += 1;
      }
      drawMaxPoints(1);
      delay(300);
    }
  }
  drawMaxPoints(0);
  delay(1000);

  //Start game
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setTextSize(1);
  display.setCursor(3, 0);
  display.print("Start in");
  display.setCursor(30, 55);
  display.print("Sekunden");
  display.setTextSize(3);
  Serial.println("Countdown");
  for (int i = 5; i > 0; i--) {
    display.setTextColor(YELLOW);
    display.setCursor(30, 20);
    display.print(i);
    delay(1000);
    display.setTextColor(BLACK);
    display.setCursor(30, 20);
    display.print(i);
  }
  display.setCursor(20, 20);
  display.setTextColor(RED);
  display.print("LOS");
  delay(100);
  display.setCursor(20, 20);
  display.setTextColor(BLUE);
  display.print("LOS");
  delay(100);
  display.setCursor(20, 20);
  display.setTextColor(RED);
  display.print("LOS");
  delay(100);
  display.setCursor(20, 20);
  display.setTextColor(BLUE);
  display.print("LOS");
  delay(500);

  //Spielstand
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setTextSize(1);
  display.setCursor(18, 0);
  display.print("Spielstand");
  display.setTextSize(3);
  display.setTextColor(GREEN);
  display.setCursor(5, 20);
  display.print(teamA);
  display.setCursor(96 - 18 - 5, 20);
  display.print(teamB);
  display.setCursor(40, 20);
  display.print(":");

  while (victory == 0) {
    if (digitalRead(leftbt) == LOW) {
      teamA += 1;
      if (teamA == maxpoints) {
        victory = 1;
        drawScore("A");
      } else {
        drawScore("none");
      }
      delay(500);
    }
    if (digitalRead(rightbt) == LOW) {
      teamB += 1;
      if (teamB == maxpoints) {
        victory = 1;
        drawScore("B");
      } else {
        drawScore("none");
      }
      delay(500);
    }
  }
  display.setTextColor(BLUE);
  display.setTextWrap(true);
  display.setTextSize(1);
  display.setCursor(30, 55);
  display.print("Neustart?");
  while (digitalRead(okbt) == HIGH) {
    maxpoints = 10;
    absmaxpoints = 20;
    absminpoints = 1;
    teamA = 0;
    teamB = 0;
    victory = 0;
  }
}
void drawScore(String team) {
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setTextSize(1);
  display.setCursor(18, 0);
  if (victory) {
    if (team == "A") {
      display.print("Sieg Team A");
    }
    if (team == "B") {
      display.print("Sieg Team B");
    }
  } else {
    display.print("Spielstand");
  }
  display.setTextSize(3);
  if (team != "A") {
    display.setTextColor(GREEN);
  } else if (team == "A") {
    display.setTextColor(YELLOW);
  }
  display.setCursor(5, 20);
  display.print(teamA);
  if (team != "B") {
    display.setTextColor(GREEN);
  } else if (team == "B") {
    display.setTextColor(YELLOW);
  }
  if (teamB > 9) {
    display.setCursor(96 - 36 - 5, 20);
  } else {
    display.setCursor(96 - 18 - 5, 20);
  }
  display.print(teamB);
  display.setTextColor(GREEN);
  display.setCursor(40, 20);
  display.print(":");
}
void drawMaxPoints(int arrows)
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setTextSize(1);
  display.setCursor(3, 0);
  display.print("Wir spielen auf");
  display.setCursor(30, 55);
  display.setTextColor(BLUE);
  display.print("Punkte");
  display.setTextColor(GREEN);
  display.setTextSize(3);
  if (arrows) {
    display.fillTriangle(0, 32, 9, 27, 9, 37, GREEN);
    display.fillTriangle(96, 32, 87, 27, 87, 37, GREEN);
  }
  display.setCursor(30, 20);
  display.print(maxpoints);
}

/**************************************************************************/
/*!
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint8_t w, h;
  display.setAddrWindow(0, 0, 96, 64);

  for (h = 0; h < 64; h++)
  {
    for (w = 0; w < 96; w++)
    {
      if (w > 83) {
        display.writePixel(WHITE);
      }
      else if (w > 71) {
        display.writePixel(BLUE);
      }
      else if (w > 59) {
        display.writePixel(GREEN);
      }
      else if (w > 47) {
        display.writePixel(CYAN);
      }
      else if (w > 35) {
        display.writePixel(RED);
      }
      else if (w > 23) {
        display.writePixel(MAGENTA);
      }
      else if (w > 11) {
        display.writePixel(YELLOW);
      }
      else {
        display.writePixel(BLACK);
      }
    }
  }
  display.endWrite();
}
