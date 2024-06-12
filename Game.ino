// Author: Carter Nelson
// MIT License (https://opensource.org/licenses/MIT)
//used for extra lives feature

#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Wire.h>
#include <SPI.h>
#define ROLL_THRESHOLD      30          // Total acceleration threshold for roll detect
#define TAP_THRESHOLD       10          // Tap detect threshold
#define DICE_COLOR          0xEA6292    // Dice digits color

const uint8_t spYOU_WIN[]           PROGMEM = {0x26, 0x2B, 0x82, 0x54, 0x76, 0x4B, 0xA7, 0x76, 0x04, 0x14, 0xB6, 0xBA, 0x5D, 0xBA, 0x9E, 0x48, 0x30, 0xEA, 0xD6, 0x69, 0x6A, 0x73, 0x01, 0xF7, 0xB7, 0xA7, 0x6A, 0xAD, 0x0C, 0x0C, 0xFB, 0x8F, 0x2C, 0x5A, 0x55, 0x2D, 0xFD, 0x52, 0x62, 0x6E, 0xBA, 0x06, 0xA5, 0x59, 0x8E, 0x72, 0x1C, 0x13, 0x97, 0x31, 0xF7, 0x61, 0x2C, 0x4D, 0x5B, 0x6E, 0x64, 0x8E, 0x19, 0x66, 0x3D, 0x55, 0x16, 0x9E, 0x56, 0xD8, 0xF5, 0xD4, 0x35, 0xA6, 0x6A, 0xC8, 0x9A, 0xDB, 0xDC, 0xF6, 0xB6, 0xB7, 0x4D, 0x9D, 0xB4, 0x66, 0xEE, 0xDE, 0xD8, 0xB7, 0xAA, 0xA5, 0xC1, 0xC2, 0xB2, 0x22, 0xAB, 0x86, 0x7A, 0x2F, 0x8D, 0x91, 0x2D, 0x6A, 0x94, 0x23, 0x3D, 0xDB, 0x8E, 0xA8, 0xB0, 0xCB, 0x4E, 0x2B, 0x39, 0x0F, 0x00, 0x00};
const uint8_t spTRY_AGAIN[]         PROGMEM = {0x0E, 0x28, 0xA1, 0xA2, 0x02, 0x0E, 0x28, 0x6D, 0x6C, 0xC4, 0xB6, 0xAB, 0xA5, 0x8B, 0xDB, 0x15, 0xEB, 0xEE, 0x5E, 0xCE, 0x6B, 0x4E, 0xE2, 0xED, 0x84, 0xB9, 0x26, 0xBE, 0xD1, 0x0D, 0x57, 0x10, 0x8B, 0x9B, 0xEB, 0x44, 0xD9, 0xC1, 0x0E, 0x57, 0x1C, 0x8B, 0x99, 0xDB, 0x46, 0x39, 0x69, 0x8E, 0xA6, 0xEE, 0x35, 0x7B, 0x64, 0x35, 0x26, 0xF2, 0x44, 0xA7, 0x94, 0xE7, 0x24, 0x60, 0x1B, 0x15, 0x55, 0xCE, 0x45, 0x66, 0x35, 0x3B, 0x29, 0x45, 0xD1, 0xCC, 0x6E, 0xE1, 0x66, 0x64, 0xD5, 0x39, 0x53, 0x74, 0x9B, 0x13, 0xE7, 0xE0, 0xAC, 0x55, 0xB3, 0x4F, 0x5C, 0x5D, 0x98, 0x45, 0xCC, 0xBE, 0xD1, 0x8D, 0x6E, 0x72, 0xF3, 0x5B, 0xAD, 0x36, 0x46, 0x4F, 0x95, 0x8C, 0x32, 0xFB, 0xDE, 0x87, 0xDE, 0x66, 0x35, 0xD7, 0x70, 0xE5, 0x06, 0x4F, 0x53, 0xCC, 0xBC, 0x95, 0x1A, 0xA4, 0xD5, 0x08, 0x19, 0xC7, 0x0F};
const uint8_t spLEFT[]              PROGMEM = {0xAD, 0x0E, 0xB0, 0x96, 0x2B, 0x5A, 0x8C, 0xC2, 0x93, 0x49, 0xAE, 0x5C, 0x78, 0x72, 0x1F, 0x23, 0xC5, 0xBE, 0xF6, 0xCD, 0x6F, 0x71, 0xEA, 0x58, 0xA2, 0xC4, 0x3A, 0xF1, 0x6D, 0x5B, 0xE7, 0x63, 0x04, 0x7B, 0x4C, 0x8A, 0x23, 0x03, 0xA2, 0x35, 0xD1, 0x80, 0x02, 0x86, 0xF7, 0x00, 0x00, 0x70, 0x40, 0x17, 0xE6, 0x08, 0x60, 0x29, 0xFD, 0x01, 0x00, 0x00};


uint8_t rollNumber;
float X, Y, Z, totalAccel;
float midi[127];
int A_four = 440; // a is 440 hz...
volatile bool right = false;
volatile bool left = false;
volatile bool modeselect = false;
const int maxModes = 10;
int mode = 1;
int maxlives = 3;
int lives = maxlives;
int down[10] = {60, 61, 63, 61, 60, 60, 61, 63, 65, 67};
int Blues[28] = {55, 55, 67, 67, 67, 67, 53, 53, 65, 65, 65, 65, 51, 51, 63, 63, 63, 63, 50, 50, 62, 62, 62, 62, 67, 65, 63, 62};
int bpm[28] = {300, 300, 150, 150, 150, 150, 300, 300, 150, 150, 150, 150, 300, 300, 150, 150, 150, 150, 300, 300, 150, 150, 150, 150, 600, 600, 600, 600};

int G = 0;
int B = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  CircuitPlayground.begin();
  generateMIDI();
  if (digitalRead(7) == HIGH) {
    modeselect = true;
    CircuitPlayground.begin();
    CircuitPlayground.setBrightness(100);
    CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
    CircuitPlayground.setAccelTap(2, TAP_THRESHOLD);
    // Seed the random function with light sensor value
    randomSeed(CircuitPlayground.lightSensor());
  }
  attachInterrupt(digitalPinToInterrupt(7), M, CHANGE);
  attachInterrupt(digitalPinToInterrupt(5), R, RISING);
  attachInterrupt(digitalPinToInterrupt(4), L, RISING);
}

void loop() {

  if (modeselect == true) {
    for (int i = 0; i < 360; i++) {
      if (modeselect == false)break;
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(mode - 1, ((sin(i * PI / 180)) * 127) + 128, (sin((i - 120)*PI / 180)) * 127 + 128, (sin((i - 240)*PI / 180)) * 127 + 128);
      delay(25);
      X = 0;
      Y = 0;
      Z = 0;
      for (int i = 0; i < 10; i++) {
        X += CircuitPlayground.motionX();
        Y += CircuitPlayground.motionY();
        Z += CircuitPlayground.motionZ();
        delay(1);
      }
      X /= 10;
      Y /= 10;
      Z /= 10;

      totalAccel = sqrt(X * X + Y * Y + Z * Z);

      // Check for rolling
      if (totalAccel > ROLL_THRESHOLD) {
        if (maxlives < 10) {
          maxlives++;
        } else {
          maxlives = 1;
        }
        CircuitPlayground.clearPixels();
        delay(500);
        for (int i = 0; i < maxlives - 1; i++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        CircuitPlayground.playTone(midi[70], 250);
        CircuitPlayground.clearPixels();
        for (int i = 0; i < maxlives; i++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        CircuitPlayground.playTone(midi[70], 250);
        CircuitPlayground.clearPixels();
        for (int i = 0; i < maxlives - 1; i++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        CircuitPlayground.playTone(midi[70], 250);
        delay(500);
        CircuitPlayground.clearPixels();
        for (int i = 0; i < maxlives; i++) {
          CircuitPlayground.setPixelColor(i, 255, 0, 0);
        }
        CircuitPlayground.playTone(midi[75], 250);
        delay(1000);
      }
    }
  } else {
    // pregame
    lives = maxlives;
    Serial.println(lives);

    switch (mode) {
      case 1:
        for (int a = 0; a < 10; a++) {
          if (lives <= 0 || modeselect == true) break;
          diff1();
        }
        //post game
        if (lives > 0) {
          Xgonna();
        } else {
          OSU();
        }
        CircuitPlayground.clearPixels();
        break;
      case 2:
        for (int a = 0; a < 10; a++) {
          if (lives <= 0 || modeselect == true) break;
          diff2();
        }
        //post game
        if (lives > 0) {
          Xgonna();
        } else {
          for (int i = 10 ; i >= 0; i--) {
            if (modeselect == true) break;
            for (int h = 0; h < i; h++) {
              if (modeselect == true) break;
              CircuitPlayground.setPixelColor(h, 255, 0, 0);
              CircuitPlayground.setPixelColor(h - 1, 0, 0, 0);
              delay(100);
            }
          }
        }
        CircuitPlayground.clearPixels();
        break;
      case 3:
        for (int a = 0; a < 10; a++) {
          if (lives <= 0 || modeselect == true) break;
          diff3();
        }
        //post game
        if (lives > 0) {
          Xgonna();
        } else {
          OSU();
        }
        CircuitPlayground.clearPixels();
        break;
      case 4:
        for (int a = 0; a < 10; a++) {
          if (lives <= 0 || modeselect == true) break;
          diff4();
        }
        //post game
        if (lives > 0) {
          Xgonna();
        } else {
          OSU();
        }
        CircuitPlayground.clearPixels();
        break;
      case 5:
        for (int a = 0; a < 10; a++) {
          if (lives <= 0 || modeselect == true) break;
          diff5();
        }
        //post game
        if (lives > 0) {
          Xgonna();
        } else {
          OSU();
        }
        CircuitPlayground.clearPixels();
        break;
      case 6:
       while(lives>0){
        diff1();
        points++;
      }
      Serial.print("Score = ");
      Serial.println(points);
      if (points > 2*maxlives) {
          Xgonna();
        } else {
          OSU();
        }
        break;
      case 7:
        while(lives>0){
        diff2();
        points++;
      }
      Serial.print("Score = ");
      Serial.println(points);
      if (points > 2*maxlives) {
          Xgonna();
        } else {
          OSU();
        }
        break;
      case 8:
        while(lives>0){
        diff3();
        points++;
      }
      Serial.print("Score = ");
      Serial.println(points);
      if (points > 2*maxlives) {
          Xgonna();
        } else {
          OSU();
        }
        break;
      case 9:
        while(lives>0){
        diff4();
        points++;
      }
      Serial.print("Score = ");
      Serial.println(points);
      if (points > 2*maxlives) {
          Xgonna();
        } else {
          OSU();
        }
        break;
      case 10:
        while(lives>0){
        diff5();
        points++;
      }
      Serial.print("Score = ");
      Serial.println(points);
      if (points > 2*maxlives) {
          Xgonna();
        } else {
          OSU();
        }
        break;
    }
  }
}






void diff1() {

  // if (lives <= 0) break;

  right = false;
  left = false;
  int light = random(2);
  int G;
  if (light == 1) {
    G = 255;
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, G, 255 - G);
      if (left == false && right == false) {
        delay(100);
      }
    }
    if (right == false && left == true) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  } else {
    G = 0;
    for (int i = 9; i >= 0; i--) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, G, 255 - G);
      if (left == false && right == false) {
        delay(100);
      }
    }
    if (right == true && left == false) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
    Serial.println(lives);
  }
}
void diff2() {
  right = false;
  left = false;
  int light = random(2);
  int G;
  if (light == 1) {
    G = 255;
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
      if (left == false && right == false) {
        delay(100);
      }
    }
    if (right == false && left == true) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  } else {
    G = 0;
    for (int i = 9; i >= 0; i--) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
      if (left == false && right == false) {
        delay(100);
      }
    }
    if (right == true && left == false) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  }
  Serial.println(lives);

}
void diff3() {
  right = false;
  left = false;
  int light = random(2);
  int G;
  if (light == 1) {
    G = 255;
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
      if (left == false && right == false) {
        delay(50);
      }
    }
    if (right == false && left == true) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  } else {
    G = 0;
    for (int i = 9; i >= 0; i--) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 255, 255);
      if (left == false && right == false) {
        delay(50);
      }
    }
    if (right == true && left == false) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  }
  Serial.println(lives);
}
void diff4() {
  right = false;
  left = false;
  int light = random(2);
  int R = random(256);
  int G = random(256);
  int B = random(256);
  if (light == 1) {
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, R, G, B);
      if (left == false && right == false) {
        delay(50);
      }
    }
    if (right == false && left == true) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  } else {
    for (int i = 9; i >= 0; i--) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, R, G, B);
      if (left == false && right == false) {
        delay(50);
      }
    }
    if (right == true && left == false) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  }
  Serial.println(lives);
}

void diff5() {
  right = false;
  left = false;

  int light = random(2);
  int color = random(2);

  if (color == 0) {
    int B = 0;
    int G = 255;
  } else {
    int B = 255;
    int G = 0;
  }

  if (light == 0) {

    for (int i = 0; i < 10; i++) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 250, 0);
      delay(25);
    }
    if (right == false && left == true) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }


  } else {

    for (int i = 9; i >= 0; i--) {
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 0, 250, 0);
      delay(25);
    }
    if (right == true && left == false) {
      //keep going
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 200, 200, 200);
        delay(10);
      }
      CircuitPlayground.clearPixels();
    } else {
      heartLoss();
      lives--;
    }
  }
  Serial.println(lives);
}
void heartLoss() {
  CircuitPlayground.clearPixels();
  delay(500);
  for (int i = 0; i < lives; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
  CircuitPlayground.playTone(midi[70], 250);
  CircuitPlayground.clearPixels();
  for (int i = 0; i < lives - 1; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
  CircuitPlayground.playTone(midi[70], 250);
  CircuitPlayground.clearPixels();
  for (int i = 0; i < lives; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
  CircuitPlayground.playTone(midi[70], 250);
  delay(500);
  CircuitPlayground.clearPixels();
  for (int i = 0; i < lives - 1; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }
  CircuitPlayground.playTone(midi[63], 250);
  delay(1000);
 // if (lives != 1) {
    CircuitPlayground.speaker.say(spTRY_AGAIN);
  //  CircuitPlayground.speaker.end();
  //  CircuitPlayground.begin();
    generateMIDI();
 //   CircuitPlayground.speaker.end();
 //   delay(500);
 // }
}
void OSU() {
  for (int i = 0; i < sizeof(down) / sizeof(int); i++)
  {
    if (modeselect == true) break;
    CircuitPlayground.setPixelColor(i, 255, 70, 0);
    if (i == 0 || i == 1 || i == 4 || i == 5 || i == 6 || i == 9) {
      CircuitPlayground.playTone(midi[down[i]], 400);
    } else if (i == 2 || i == 3 || i == 7 || i == 8) {
      CircuitPlayground.playTone(midi[down[i]], 200);
    }
  }
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 50, 50, 50);
  }
  //CircuitPlayground.speaker.say(spTRY_AGAIN);
  //CircuitPlayground.speaker.end();
  //      delay(500);
}
void Xgonna() {
  for (int i = 0; i < sizeof(Blues) / sizeof(int); i++)
  {
    int R = random(10)*5;
    int G = random(10)*5;
    int B = random(10)*5;
    for (int h = 0; h < 10; h++) {
      CircuitPlayground.setPixelColor(h, R, G, B);
    }
    if (modeselect == true) break;
    CircuitPlayground.playTone(midi[Blues[i]], bpm[i]);
  }
}
void R() {
  if (modeselect == true) {
    if (mode < maxModes) {
      mode++;
    } else {
      mode = 1;
    }
  } else {
    right = true;
  }
}
void L() {
  if (modeselect == true) {
    if (mode > 1) {
      mode--;
    } else {
      mode = maxModes;
    }
  } else {
    left = true;
  }
}
void M() {
  if (digitalRead(7) == HIGH) {
    modeselect = true;
  } else {
    modeselect = false;
  }
}
void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}
