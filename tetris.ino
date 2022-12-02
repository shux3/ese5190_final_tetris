/*
 * This is the Arduino UNO version of Tetris
 * Final Project for ESE5190
 * 
 * Author: Shu Xu
 * last edited: Dec 2nd, 2022
 */

// library
#include "LedControl.h"
#include "binary.h"

/* here we have
 *  VCC to 5V power pin
 *  GND to ground pin
 *  Datain to pin 12
 *  chipselected to pin 11
 *  CLK to pin 10
 *  using 1 LED Matrix
 */

LedControl tetris = LedControl(12, 10, 11, 1);

/*  joystick configures
 *  X to A0, Y to A1, SW to pin2
 */
int VRx = A0;
int VRy = A1;
int SW = 2;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

// delay time between drops
unsigned long droptime=500;

// new block coming-in flag
int new_block = 1;

// clear one block flag
boolean clear_block = true;

// live starting block position
int live_x;
int live_y;

// block type
int type;
int block;

// Game over lose face
byte lose_face[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100};

void gameover(){
  // Display lose face
  tetris.setColumn(0,7,lose_face[0]);
  tetris.setColumn(0,6,lose_face[1]);
  tetris.setColumn(0,5,lose_face[2]);
  tetris.setColumn(0,4,lose_face[3]);
  tetris.setColumn(0,3,lose_face[4]);
  tetris.setColumn(0,2,lose_face[5]);
  tetris.setColumn(0,1,lose_face[6]);
  tetris.setColumn(0,0,lose_face[7]);
}

void setup() {
  // max7219 oeprates
  tetris.shutdown(0,false);
  // Set brightness to 8
  tetris.setIntensity(0,8);
  // Clear the display
  tetris.clearDisplay(0);
  Serial.begin(115200);

  // joystick set up
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
}

// ------------------------------------------- block function -------------------------------------------
/* block:
 *  0 -- I
 *  1 -- J
 *  2 -- L
 *  3 -- O
 *  4 -- S
 *  5 -- Z
 *  6 -- T
*/
void sel_block(int block, int type, int x, int y, boolean write){
  
  switch(block){
    case 0:
      I_block(type, x, y, write);
      break;

    case 1:
      J_block(type, x, y, write);
      break;

    case 2:
      L_block(type, x, y, write);
      break;

    case 3:
      O_block(x, y, write);
      break;

    case 4:
      S_block(type, x, y, write);
      break;

    case 5:
      Z_block(type, x, y, write);
      break;
      
    case 6:
      T_block(type, x, y, write);
      break;
  }
}
/* I block: two cases 
 *  case 0 - horizontal
 *  case 1 - vertical 
 *  x, y represents the starting point of block 
 */
void I_block(int type, int x, int y, boolean write){
  switch(type){
    // horizontal I block
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+2, y, write);
      tetris.setLed(0, x+3, y, write);
      break;
    // vertical I block
    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x, y-3, write);
      break;
  } 
}

/* J block: four cases 
 *  case 0 - vertical left
 *  case 1 - horizontal up
 *  case 2 - vertical right
 *  case 3 - horizontal down
 */
void J_block(int type, int x, int y, boolean write){
  switch(type){
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x-1, y-2, write);
      break;

    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x+1, y-1, write);
      tetris.setLed(0, x+2, y-1, write);
      break;

    case 2:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x+1, y, write);
      break;

    case 3:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+2, y, write);
      tetris.setLed(0, x+2, y-1, write);
      break;
    
  }
}

/* L block: four cases 
 *  case 0 - vertical right
 *  case 1 - horizontal down
 *  case 2 - vertical left
 *  case 3 - horizontal up
 */
void L_block(int type, int x, int y, boolean write){
  switch(type){
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x+1, y-2, write);
      break;

    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+2, y, write);
      break;

    case 2:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+1, y-1, write);
      tetris.setLed(0, x+1, y-2, write);
      break;

    case 3:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x-1, y-1, write);
      tetris.setLed(0, x-2, y-1, write);
      break;
  }
}

// O block
void O_block(int x, int y, boolean write){
  tetris.setLed(0, x, y, write);
  tetris.setLed(0, x, y-1, write);
  tetris.setLed(0, x+1, y, write);
  tetris.setLed(0, x+1, y-1, write);
}

/* S block: two cases 
 *  case 0 - horizontal
 *  case 1 - vertical
 */
void S_block(int type, int x, int y, boolean write){
  switch(type){
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x-1, y-1, write);
      break;

    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x+1, y-1, write);
      tetris.setLed(0, x+1, y-2, write);
      break;
  }
}

/* Z block: two cases 
 *  case 0 - horizontal
 *  case 1 - vertical
 */
void Z_block(int type, int x, int y, boolean write){
  switch(type){
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+1, y-1, write);
      tetris.setLed(0, x+2, y-1, write);
      break;

    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x-1, y-1, write);
      tetris.setLed(0, x-1, y-2, write);
      break;
  }
}

/* T block: four cases 
 *  case 0 - up
 *  case 1 - right
 *  case 2 - dpwn
 *  case 3 - left
 */
void T_block(int type, int x, int y, boolean write){
  switch(type){
    case 0:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x-1, y-1, write);
      tetris.setLed(0, x+1, y-1, write);
      break;

    case 1:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x+1, y-1, write);
      break;

    case 2:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x+1, y, write);
      tetris.setLed(0, x+2, y, write);
      tetris.setLed(0, x+1, y-1, write);
      break;

    case 3:
      tetris.setLed(0, x, y, write);
      tetris.setLed(0, x, y-1, write);
      tetris.setLed(0, x, y-2, write);
      tetris.setLed(0, x-1, y-1, write);
      break;
  }
}

// ----------------------------------------- movement function -----------------------------------------

/* block:
 *  0 -- I
 *  1 -- J
 *  2 -- L
 *  3 -- O
 *  4 -- S
 *  5 -- Z
 *  6 -- T
*/
int move_down(int block, int type, int x, int y){
  switch(block){
    // I block
    case 0:
      switch(type){
        case 0:
          if (y-1 < 0) {
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x+2, y, false);
          tetris.setLed(0, x+3, y, false);
          
          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x+1, y-1, true);
          tetris.setLed(0, x+2, y-1, true);
          tetris.setLed(0, x+3, y-1, true);
          break;
          
        case 1:
          if (y - 4 < 0) {
            return 1;
          }
          tetris.setLed(0, x, y, false);
    
          tetris.setLed(0, x, y-4, true);
          break;
      }
      break;
      
    // J block
    case 1:
      switch(type){
        case 0:
          if (y - 3 < 0) {
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x-1, y-2, false);
          
          tetris.setLed(0, x, y-3, true);
          tetris.setLed(0, x-1, y-3, true);
          break;

        case 1:
          if (y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y-1, false);
          tetris.setLed(0, x+2, y-1, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x+1, y-2, true);
          tetris.setLed(0, x+2, y-2, true);
          break;
          
        case 2:
          if (y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);

          tetris.setLed(0, x, y-3, true);
          tetris.setLed(0, x+1, y-1, true);
          break;
        
        case 3:
          if ( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x+2, y, false);

          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x+1, y-1, true);
          tetris.setLed(0, x+2, y-2, true);
          break;
      }
      break;

    // L block
    case 2:
      switch(type){
        case 0:
          if ( y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y-2, false);
      
          tetris.setLed(0, x, y-3, true);
          tetris.setLed(0, x+1, y-3, true);
          break;

        case 1:
          if (y - 2 < 0) {
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x+2, y, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x+1, y-1, true);
          tetris.setLed(0, x+2, y-1, true);
          break;

        case 2:
          if (y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          
          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x+1, y-3, true);
          break;

        case 3:
          if ( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x-1, y-1, false);
          tetris.setLed(0, x-2, y-1, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x-1, y-2, true);
          tetris.setLed(0, x-2, y-2, true);
          break;
      }
      break;

    // O block
    case 3:
      if ( y - 2 < 0){
        return 1;
      }
      tetris.setLed(0, x, y, false);
      tetris.setLed(0, x+1, y, false);

      tetris.setLed(0, x, y-2, true);
      tetris.setLed(0, x+1, y-2, true);
      break;

    // S block
    case 4:
      switch(type){
        case 0:
          if( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x-1, y-1, false);

          tetris.setLed(0, x+1, y-1, true);
          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x-1, y-2, true);
          break;

        case 1:
          if ( y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y-1, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x+1, y-3, true);
          break;
      }
      break;

    // Z block
    case 5:
      switch(type){
        case 0:
          if ( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x+2, y-1, false);
  
          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x+1, y-2, true);
          tetris.setLed(0, x+2, y-2, true);
          break;

        case 1:
          if ( y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x-1, y-1, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x-1, y-3, true);
          break;
      }
      break;

    // T block
    case 6:
      switch(type){
        case 0:
          if ( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x-1, y-1, false);
          tetris.setLed(0, x+1, y-1, false);

          tetris.setLed(0, x, y-2, true);
          tetris.setLed(0, x-1, y-2, true);
          tetris.setLed(0, x+1, y-2, true);
          break;

        case 1:
          if ( y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y-1, false);

          tetris.setLed(0, x, y-3, true);
          tetris.setLed(0, x+1, y-2, true);
          break;

        case 2:
          if ( y - 2 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x+1, y, false);
          tetris.setLed(0, x+2, y, false);
          
          tetris.setLed(0, x, y-1, true);
          tetris.setLed(0, x+2, y-1, true);
          tetris.setLed(0, x+1, y-2, true);
          break;

        case 3:
          if ( y - 3 < 0){
            return 1;
          }
          tetris.setLed(0, x, y, false);
          tetris.setLed(0, x-1, y-1, false);
          
          tetris.setLed(0, x, y-3, true);
          tetris.setLed(0, x-1, y-2, true);
          break;
      }
      break;
  }
  return 0;
}


/* block:
 *  0 -- I
 *  1 -- J
 *  2 -- L
 *  3 -- O
 *  4 -- S
 *  5 -- Z
 *  6 -- T
*/
int move_right(int block, int type, int x, int y){
  switch(block){
    case 0:
      switch(type){
        case 0:
          if ( x + 4 > 7){
            return 2;
          }
          break;

        case 1:
          if ( x + 1 > 7){
            return 2;
          }
          break;
      }
      I_block(type, x, y, false);
      I_block(type, x+1, y, true);
      break;
      
    case 1:
      switch(type){
        case 0:
          if (x + 1 > 7){
            return 2;
          }
          break;

        case 1:
          if (x + 3 > 7){
            return 2;
          }
          break;
          
        case 2:
          if (x + 2 > 7){
            return 2;
          }
          break;

        case 3:
          if (x + 3 > 7){
            return 2;
          }
          break;
      }
      J_block(type, x, y, false);
      J_block(type, x+1, y, true);
      break;

    case 2:
      switch(type){
        case 0:
          if (x + 2 > 7){
            return 2;
          }
          break;

        case 1:
          if (x + 3 > 7){
            return 2;
          }
          break;
          
        case 2:
          if (x + 2 > 7){
            return 2;
          }
          break;

        case 3:
          if (x + 3 > 7){
            return 2;
          }
          break;
      }
      L_block(type, x, y, false);
      L_block(type, x+1, y, true);
      break;

    case 3:
      if (x + 2 > 7){
        return 2;
      }
      O_block(x, y, false);
      O_block(x+1, y, true);
      break;

    case 4:
      switch(type){
        case 0:
          if (x + 2 > 7){
            return 1;
          }
          break;

        case 1:
          if (x + 2 > 7){
            return 1;
          }
          break;
      }
      S_block(type, x, y, false);
      S_block(type, x+1, y, true);
      break;

    case 5:
      switch(type){
        case 0:
          if (x + 3 > 7){
            return 1;
          }
          break;

        case 1:
          if (x + 1 > 7){
            return 1;
          }
          break;
      }
      Z_block(type, x, y, false);
      Z_block(type, x+1, y, true);
      break;

    case 6:
      switch(type){
        case 0:
          if (x + 2 > 7){
            return 1;
          }
          break;

        case 1:
          if (x + 2 > 7){
            return 1;
          }
          break;
          
        case 2:
          if (x + 3 > 7){
            return 1;
          }
          break;

        case 3:
          if (x + 1 > 7){
            return 1;
          }
          break;
      }
      T_block(type, x, y, false);
      T_block(type, x+1, y, true);
      break;
  }
  return 0;
}

/* block:
 *  0 -- I
 *  1 -- J
 *  2 -- L
 *  3 -- O
 *  4 -- S
 *  5 -- Z
 *  6 -- T
*/
int move_left(int block, int type, int x, int y){
  switch(block){
    case 0:
      switch(type){
        case 0:
          if ( x - 1 < 0){
            return 2;
          }
          break;

        case 1:
          if ( x - 1 < 0){
            return 2;
          }
          break;
      }
      I_block(type, x, y, false);
      I_block(type, x-1, y, true);
      break;
      
    case 1:
      switch(type){
        case 0:
          if (x - 2 < 0){
            return 2;
          }
          break;

        case 1:
          if (x - 1 < 0){
            return 2;
          }
          break;
          
        case 2:
          if (x - 1< 0){
            return 2;
          }
          break;

        case 3:
          if (x - 1 < 0){
            return 2;
          }
          break;
      }
      J_block(type, x, y, false);
      J_block(type, x-1, y, true);
      break;

    case 2:
      switch(type){
        case 0:
          if (x - 1 < 0){
            return 2;
          }
          break;

        case 1:
          if (x - 1 < 0){
            return 2;
          }
          break;
          
        case 2:
          if (x - 1 < 0){
            return 2;
          }
          break;

        case 3:
          if (x - 3 < 0){
            return 2;
          }
          break;
      }
      L_block(type, x, y, false);
      L_block(type, x-1, y, true);
      break;

    case 3:
      if (x - 1 < 0){
        return 2;
      }
      O_block(x, y, false);
      O_block(x-1, y, true);
      break;

    case 4:
      switch(type){
        case 0:
          if (x - 2 < 0){
            return 2;
          }
          break;

        case 1:
          if (x - 1 < 0){
            return 2;
          }
          break;
      }
      S_block(type, x, y, false);
      S_block(type, x-1, y, true);
      break;

    case 5:
      switch(type){
        case 0:
          if (x - 1 < 0){
            return 2;
          }
          break;

        case 1:
          if (x - 2 < 0){
            return 2;
          }
          break;
      }
      Z_block(type, x, y, false);
      Z_block(type, x-1, y, true);
      break;

    case 6:
      switch(type){
        case 0:
          if (x - 2 < 0){
            return 2;
          }
          break;

        case 1:
          if (x - 1 < 0){
            return 2;
          }
          break;
          
        case 2:
          if (x - 1 < 0){
            return 2;
          }
          break;

        case 3:
          if (x - 2 < 0){
            return 2;
          }
          break;
      }
      T_block(type, x, y, false);
      T_block(type, x-1, y, true);
      break;
  }
  return 0;
}

// ---------------------------------------------- main ----------------------------------------------
void loop(){
  // Joystick readings
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);
  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.println(SW_state);

  // game starts here
  if (SW_state == 1 | new_block == 1) {
    if (new_block == 1){
      tetris.clearDisplay(0);
      // decide new block type
      block = random(0,7);
      switch(block) {
        case 0:
          type = random(0,2);     
          switch(type){
            case 0:
              live_x = 2;
              live_y = 7;
              break;
            case 1:
              live_x = 3;
              live_y = 7;
              break;
          }
          break;

        case 1:
          type = random(0,4);
          switch(type){
            case 0:
              live_x = 4;
              live_y = 7;
              break;
            case 1:
              live_x = 3;
              live_y = 7;
              break;
            case 2:
              live_x = 3;
              live_y = 7;
              break;
            case 3:
              live_x = 3;
              live_y = 7;
              break;
          }
          break;

        case 2:
          type = random(0,4);
          switch(type){
            case 0:
              live_x = 4;
              live_y = 7;
              break;
            case 1:
              live_x = 3;
              live_y = 7;
              break;
            case 2:
              live_x = 3;
              live_y = 7;
              break;
            case 3:
              live_x = 5;
              live_y = 7;
              break;
          }
          break;

        case 3:
          type = 0;
          live_x = 3;
          live_y = 7;
          break;

        case 4:
          type = random(0,2);
          switch(type){
            case 0:
              live_x = 4;
              live_y = 7;
              break;
            case 1:
              live_x = 3;
              live_y = 7;
              break;
          }
          break;

        case 5:
          type = random(0,2);
          switch(type){
            case 0:
              live_x = 3;
              live_y = 7;
              break;
            case 1:
              live_x = 4;
              live_y = 7;
              break;
          }
          break;

        case 6:
          type = random(0,4);
          switch(type){
            case 0:
              live_x = 4;
              live_y = 7;
              break;
            case 1:
              live_x = 3;
              live_y = 7;
              break;
            case 2:
              live_x = 3;
              live_y = 7;
              break;
            case 3:
              live_x = 4;
              live_y = 7;
              break;
          }
          break;     
      }   
      sel_block(block, type, live_x, live_y, true);
      new_block = 0;
    } else if (mapY < -100){
      delay(droptime);
      Serial.println(String("block: ") + block + String(", type: ") + type);
      new_block = move_down(block, type, live_x, live_y);
      live_y = live_y - 1;
    } else if (mapX > 100){
      delay(droptime);
      new_block = move_right(block, type, live_x, live_y);
      if (new_block == 2) {
        new_block = 0;
      } else {
        live_x = live_x + 1;
      }
    } else if (mapX < -100){
      delay(droptime);
      new_block = move_left(block, type, live_x, live_y);
      if (new_block == 2) {
        new_block = 0;
      } else {
        live_x = live_x - 1;
      }
    }
  } else {
    gameover();
    delay(droptime);
    delay(droptime);
    tetris.clearDisplay(0);
    new_block = 1;
  }
}
