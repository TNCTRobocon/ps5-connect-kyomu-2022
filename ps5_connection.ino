
#include<PS5BT.h>
#include<usbhub.h>
#include<SPI.h>

USB usb;
BTD btd(&usb);

PS5BT PS5(&btd, PAIR);

#define ERROR_TX 1

enum transmit_def{
  BUTTON = 0,
  R_STICK_X,
  R_STICK_Y,
  L_STICK_X,
  L_STICK_Y,
  R2_VAL,
  l2_VAL,
};

enum button_def{
  CIRCLE_BUTTON = 0x1,
  R1_BUTTON     = 0x2,
  L1_BUTTON     = 0X4
};

struct PS5_input{
  uint8_t button = 0;

  struct axis{
    uint8_t x = 0, 
            y = 0;
  }r_stick, l_stick;
  
  uint8_t r2_val    = 0,
          l2_val    = 0;
}ps;

uint8_t tx_buff[8] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(usb.Init() == -1){
    Serial.write(ERROR_TX);
    while(1);
  }
}

void loop() {
  usb.Task();

  if(PS5.getButtonClick(PS)){
    Serial.write(ERROR_TX);
  }

  if((ps.button&CIRCLE_BUTTON) ^ PS5.getButtonPress(CIRCLE)){
    ps.button^= CIRCLE_BUTTON;
  }
  if((ps.button&R1_BUTTON) ^ PS5.getButtonPress(R1)<<1){
    ps.button ^= R1_BUTTON;
  }
  if((ps.button&L1_BUTTON) ^ PS5.getButtonPress(L1)<<2){
    ps.button ^= L1_BUTTON; 
  }

  ps.r_stick.x = PS5.getAnalogHat(RightHatX);
  ps.r_stick.y = PS5.getAnalogHat(RightHatY);
  ps.l_stick.x = PS5.getAnalogHat(LeftHatX);
  ps.l_stick.y = PS5.getAnalogHat(LeftHatY);

  ps.r2_val = PS5.getAnalogButton(R2);
  ps.l2_val = PS5.getAnalogButton(L2);
  

  if(ps.button&CIRCLE_BUTTON){
    PS5.setRumbleOn(RumbleHigh);
    //ただの意地悪
  }
  
  Serial.write((uint8_t*)&ps, sizeof(PS5_input));

  delay(1);
}
