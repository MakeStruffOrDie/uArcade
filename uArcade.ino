//               ___                            __    
//       __  __ /   |   _____ _____ ____ _ ____/ /___ 
//      / / / // /| |  / ___// ___// __ `// __  // _ \
//     / /_/ // ___ | / /   / /__ / /_/ // /_/ //  __/
//     \__,_//_/  |_|/_/    \___/ \__,_/ \__,_/ \___/ 
//
//              Created by Yair Kapach 2015
//        For uArcade build details, please visit: 
//         http://www.youtube.com/user/microstrat
//           http://makestuffordie.blogspot.com/
//
//  You may do whatever you want with this code, as long as:
//    a. share it and keep it FREE. 
//    b. keep the original credits.

#include <U8glib.h>

static const uint32_t splash [] U8G_PROGMEM = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7936,0,0,0,536362880,520095680,127,25171968,0x7FF8FFE0,0x3FC007C0,127,25171968,0x7FF87FE0,0x3FE00FE0,127,6316032,0xF8F871F0,0x3FF00FE0,7,6316032,0xF8F831F0,838344672,7,33552384,0xF0F811F0,7872480,7,33552384,0xF0F801F0,3677936,7,0x79F9E00,0xF0F807F0,3940080,119,0x79F9E00,0xF8F81FF0,3947640,127,536870784,0xFCF83FF0,3947640,127,536870784,0x7FF8FFE0,3947640,127,436205952,0x7FF9FF80,3963000,7,436205952,536475648,7895164,7,427825536,0xF9F000,821592060,7,427825536,0xF9F000,0x3FF87FFC,7,7987200,0xF9F000,0x3FF0FFFC,127,7987200,0xF9F1E0,0x3FE0F03E,127,0,0xF8F9F0,0x3FC0F03E,127,0,0xF8FFF0,520220734,1,0,0xF87FF0,4159,3145728,0,0xF03FC0,0,8128496,0,384,0xF8080000,8265715,0,0,0xFC2FF004,8265715,0,0xC1F00000,0xF83FF00F,2571507,0,0xC0F007CC,947908623,474224,0x801E0000,0xC0F807CF,947712031,473200,0x801F07C0,0xE0FC0F8F,955281439,2047088,0x807F0FC0,0xE0FC1F8F,955314239,8339056,0x80FF0FC0,0xE0781F8F,0xB9E0F03F,0xFE1FF3,0x81FF0FC0,0xF07C1F0F,0xF9E0F03D,0xFC0FF3,0x81FF0FC0,0xF07E3F0F,0xF9E0F03D,0xE00FF3,0x81FF0FC0,0xF03E3F0F,0xF9E0F078,0xE007F3,0x7FF0FC0,0xF83E3E0F,971042936,0xE20F70,0xFFF0FC0,0xF83E3E0F,954265720,0xF60F70,536809408,0x781F7E1F,955314416,8330864,0x3F9F0FC0,0x7C1F7C1F,955314416,4136048,0x3F9F0FC0,0xFC1FF81F,0xF878F0FF,3947635,0x3F1F0FC0,0xFC0FF81F,0xF87CF1FF,14451,0xFE1F0FC0,0xFC0FF01F,0xF83FF1FF,51,0xFC1F0FC0,0xFE0FF01F,941617632,0,0xF81F0FC0,0x3E0FF01F,0xFF1E0,0,0xF01F0FC0,0x3E07F01F,5088,0,0xF01F0FC0,0x3E07E01F,0,0,0xE03F0FC0,520347679,0,0,0xC03F0FC0,245791,16777216,524288,0x803F0FC0,31,16777216,786432,4132800,15,41943040,524288,4132800,0,0x82866000,25889906,462784,0,0xC4444000,38572617,1984,0,0x87C44000,63478792,0,0,0x88244000,4755976,0,0,0x882E4000,39621192,0,0,0xDC758000,25902129,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const uint16_t monsters[] U8G_PROGMEM = {0x0600,0x0F00,0x1F80,0x36C0,0x3FC0,0x0900,0x1680,0x2940,0x0600,0x0F00,0x1F80,0x36C0,0x3FC0,0x0900,0x1080,0x0900,0x2080,0x9120,0xBFA0,0xEEE0,0x7FC0,0x3F80,0x2080,0x4040,0x2080,0x1100,0x3F80,0x6EC0,0xFFE0,0xBFA0,0xA0A0,0x1B00,0x0E00,0x7FC0,0xFFE0,0xCE60,0xFFE0,0x3F80,0x6EC0,0xC060,0x0E00,0x7FC0,0xFFE0,0xCE60,0xFFE0,0x3F80,0x6EC0,0x3180};
  
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, d/c A0 = 9
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // I2C: SCL=A5,SDA=A4

const int screenW = 128, screenH = 64;
enum GameState { GAME_NORMAL, GAME_WIN, GAME_OVER } g_gameState;

// encoder debounce was taken from: http://playground.arduino.cc/Main/RotaryEncoders
enum EncoderPins {  ENCODER_PIN_RIGHT = 2, ENCODER_PIN_LEFT = 3, ENCODER_PIN_FIRE = 12};

volatile unsigned char encoderPos = 127;  // a counter for the dial
unsigned int lastReportedPos = 1;         // change management
static boolean rotating = false;          // debounce management
int lastPress = HIGH;
int debounce = 20;
int lastTime = 0;
int state = LOW;

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

bool checkBtn() {
  rotating = true;  // reset the debouncer

  if (lastReportedPos != encoderPos) {
    lastReportedPos = encoderPos;
  }
  int reading = digitalRead(ENCODER_PIN_FIRE);

  if (reading != lastPress) {
    lastTime = millis(); // reset the debouncing timer
  }

  if ((millis() - lastTime) > debounce) {
    if (reading != state) {
      state = reading;
      if (state == LOW) {
        return true;
      }
    }
  }
  lastPress = reading;
  return false;
}

void doEncoderA() { // Interrupt on A changing state
  // debounce
  if (rotating)
    delay(1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if (digitalRead(ENCODER_PIN_RIGHT) != A_set) {  // debounce once more
    A_set = !A_set;
    // adjust counter + if A leads B
    if (A_set && !B_set && encoderPos != 0xff) {
      encoderPos++;
    }
    rotating = false;  // no more debouncing until loop() hits again
  }
}

void doEncoderB() { // Interrupt on B changing state, same as A above
  if (rotating)
    delay(1);
  if (digitalRead(ENCODER_PIN_LEFT) != B_set) {
    B_set = !B_set;
    if (B_set && !A_set && encoderPos != 0) {  //  adjust counter - 1 if B leads A
      encoderPos--;
    }
    rotating = false;
  }
}

void setupEncoder() {
  pinMode(ENCODER_PIN_RIGHT, INPUT);
  pinMode(ENCODER_PIN_LEFT, INPUT);
  pinMode(ENCODER_PIN_FIRE, INPUT);
  digitalWrite(ENCODER_PIN_RIGHT, HIGH);
  digitalWrite(ENCODER_PIN_LEFT, HIGH);
  digitalWrite(ENCODER_PIN_FIRE, HIGH);
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
}

class Lasers {
public:
  enum { LASER_H = 3, MAX_LASERS = 4, INACTIVE_LASER = 0xff };

  Lasers() { reset(); }

  void reset() { 
    memset(_y, INACTIVE_LASER, MAX_LASERS);
    //_lastStep = millis();
  }

  void add(uint8_t x, uint8_t y) {
    for (int i = 0; i < MAX_LASERS; i++) {
      if (_y[i] == INACTIVE_LASER) {
        _y[i] = y;
        _x[i] = x;
        break;
      }
    }
  }

  void draw() {
    for (int i = 0; i < MAX_LASERS; i++) {
      uint8_t y = _y[i];
      if (y != INACTIVE_LASER) {
        u8g.drawVLine(_x[i], _y[i], LASER_H);
      }
    }
  }

  void step() {
//    if (millis() - _lastStep < 50)
//      return;
//    
//    _lastStep = millis();

    for (int i = 0; i < MAX_LASERS; i++) {
      uint8_t y = _y[i];
      if (y != INACTIVE_LASER) {
        if (y < 2) {
          y = INACTIVE_LASER;
        } else {
          y -= 2;
        }
      }
      _y[i] = y;
    }
  }

  uint8_t _y[MAX_LASERS];
  uint8_t _x[MAX_LASERS];
//  unsigned long _lastStep;
};

Lasers lasers;

class PlayerShip {
public:
  enum { SHIP_RAD = 6, SHIP_W = SHIP_RAD * 2 + 1, SHIP_H = 6, SHIP_Y = screenH - 1 - SHIP_H };

  PlayerShip() : _x(screenW / 2) {}
  
  void draw() {
    u8g.drawHLine(_x - 1, SHIP_Y, 3);
    u8g.drawHLine(_x - 1, SHIP_Y + 1, 3);
    u8g.drawHLine(_x - (SHIP_RAD - 1), SHIP_Y + 2, SHIP_W - 2);
    u8g.drawHLine(_x - SHIP_RAD, SHIP_Y + 3, SHIP_W);
    u8g.drawHLine(_x - SHIP_RAD, SHIP_Y + 4, SHIP_W);
    u8g.drawHLine(_x - SHIP_RAD, SHIP_Y + 5, SHIP_W);
  }
  
  void step() {
    const bool fire = checkBtn();
    _x = screenW / 2 + (encoderPos - 127) * 4;

    if (_x - SHIP_RAD < 0) _x = SHIP_RAD;
    if (_x + SHIP_RAD >= screenW) _x = screenW - SHIP_RAD - 1;
    if (fire) lasers.add(_x, SHIP_Y - Lasers::LASER_H);
  }
  int _x;
};

class Monsters {
public:
  enum { SPEED = 300 };
  enum { BLANK_IDX = 6 };
  enum { MAX_COLS = 8, MAX_ROWS = 4, MON_WIDTH = 16, MON_HEIGHT = 8, MON_HMARGIN = 1 };

  Monsters(const int x, const int y, const int width, const int height, const uint8_t *data) {
    _x = x;
    _y = y;
    _left = 0;
    _right = width * MON_WIDTH;
    _bottom = height * (MON_HEIGHT + MON_HMARGIN);
    _dx = 1;
    _width = width;
    _height = height;
    _currSprite = 0;
    _lastUpdate = millis();
    memcpy(_data, data, MAX_COLS * MAX_ROWS);
  }

  void step() {
    if (millis() - _lastUpdate >= SPEED) {
      _lastUpdate = millis();
      _currSprite ^= 1;
      _x += _dx;
      if (_x + _left < 0) {
        _x = -_left;
        _dx = -_dx;
        _y++;
      }
      if (_x + _right >= screenW) {
        _x = screenW - _right - 1;
        _dx = -_dx;
        _y++;
      }
    }
    if (_y + _bottom >= PlayerShip::SHIP_Y) {
      //_y = 0;
      g_gameState = GAME_OVER;
    }
  }
  void draw() {
    for (int i = 0; i < _height; i++) {
      for (int j = 0; j < _width; j++) {
        const int idx = _data[j + i * MAX_COLS] + _currSprite;
        if (idx < BLANK_IDX) {
          u8g.drawXBMP(_x + j * MON_WIDTH, _y + i * (MON_HEIGHT + MON_HMARGIN), MON_WIDTH, MON_HEIGHT, (unsigned char*) (monsters + idx * MON_WIDTH / 2));
        }
      }
    }
  }
  void hitTest() {
    bool hit = false;

    for (int i = 0; i < Lasers::MAX_LASERS; i++) {
      // quick fallbacks
      uint8_t y = lasers._y[i];
      if ((y == Lasers::INACTIVE_LASER) || (y + Lasers::LASER_H < _y) || (y >= _y + _bottom))
        continue;

      uint8_t x = lasers._x[i];
      if ((x < _x + _left) || (x >= _x + _right))
        continue;
      
      if (((x - _x) & 0xf) > 12) // hack
        continue;

      int col = (x - _x) / (MON_WIDTH); //& 0xf; // hack mod(ship width)
      int row = (y - _y) / (MON_HEIGHT + MON_HMARGIN);

      if (_data[col + row * MAX_COLS] >= BLANK_IDX)
        continue;

      _data[col + row * MAX_COLS] = BLANK_IDX;
      lasers._y[i] = 0xff;
      hit = true;
    }

    if (!hit) {
      return;
    }

    int c0 = _width - 1, c1 = 0;
    int r1 = 0;

    for (int i = 0; i < _height; i++) {
      for (int j = 0; j < _width; j++) {
        if (_data[j + i * MAX_COLS] < BLANK_IDX) {
          c0 = min(c0, j);
          c1 = max(c1, j);
          r1 = max(r1, i);
        }
      }
    }
    if (c0 > c1) {
      g_gameState = GAME_WIN;
      return;
    }
    _left = c0 * MON_WIDTH;
    _right = (c1 + 1) * MON_WIDTH;
    _bottom = (r1 + 1) * (MON_HEIGHT + MON_HMARGIN);
  }

  unsigned long _lastUpdate;
  uint8_t _data[MAX_COLS * MAX_ROWS];
  int _width;
  int _height;
  int _x;
  int _y;
  int _left;
  int _right;
  int _bottom;
  int _dx;
  uint8_t _currSprite;
};

const uint8_t level_0[] = { 0, 0, 0, 0, 0, 0, 0xff, 0xff,
                            2, 2, 2, 2, 2, 2, 0xff, 0xff,
                            4, 4, 4, 4, 4, 4, 0xff, 0xff, 
                            4, 4, 4, 4, 4, 4, 0xff, 0xff};

Monsters mon(0, 0, 6, 4, level_0);
PlayerShip player;

void setup(void) {
  u8g.setDefaultForegroundColor();
  u8g.setFont(u8g_font_unifont);
  setupEncoder();

  g_gameState = GAME_NORMAL;

  do {
    u8g.firstPage();
    do {
      u8g.drawXBMP(0, 0, screenW, screenH, (const uint8_t*) splash);
    } while (u8g.nextPage());
  } while (digitalRead(ENCODER_PIN_FIRE) == HIGH);
}

void loop(void) {
  if (g_gameState == GAME_NORMAL) {
    u8g.firstPage();
    do {
      mon.draw();
      player.draw();
      lasers.draw();
    } while (u8g.nextPage());

    player.step();
    mon.step();
    lasers.step();
    mon.hitTest();
  } else {
    u8g.firstPage();
    do {
      if (g_gameState == GAME_OVER) {
        u8g.drawStr(26, 33, "Game Over");
      } else {
        u8g.drawStr(26, 33, "Press RST");
      }
    } while (u8g.nextPage());
  }
}
