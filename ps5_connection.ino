#include<PS5BT.h>
#include<usbhub.h>

USB usb;
BTD btd(&usb);

PS5BT PS5(&btd, PAIR);

#define ERROR_TX 1

enum button{
  CIRCLE_SHOT  = 0x01,
  R1_SHOT_SPD  = 0x02,
  L1_SHOT_SPD  = 0x04,
};

enum transmit_def{
  BUTTON = 0,
  R_STICK_X,
  R_STICK_Y,
  L_STICK_X,
  L_STICK_Y,
  R2_VAL,
  l2_VAL,
};

uint8_t tx_buff[8] = {0};

void setup() {
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

  if(tx_buff[BUTTON] ^ PS5.getButtonPress(CIRCLE)){
    tx_buff[BUTTON] ^= CIRCLE_SHOT;
  }
  if(tx_buff[BUTTON] ^ (PS5.getButtonPress(R1) << 1)){
    tx_buff[BUTTON] ^= R1_SHOT_SPD;
  }
  if(tx_buff[BUTTON] ^ (PS5.getButtonPress(L1) << 2)){
    tx_buff[BUTTON] ^= L1_SHOT_SPD; 
  }

  tx_buff[R_STICK_X] = PS5.getAnalogHat(RightHatX);
  tx_buff[R_STICK_Y] = PS5.getAnalogHat(RightHatY);
  tx_buff[L_STICK_X] = PS5.getAnalogHat(LeftHatX);
  tx_buff[L_STICK_Y] = PS5.getAnalogHat(LeftHatY);

  tx_buff[R2_VAL] = PS5.getAnalogButton(R2);
  tx_buff[L2_VAL] = PS5.getAnalogButton(L2);
  

  if(tx_buff[BUTTON] & CIRCLE_SHOT){
    PS5.setRumbleOn(RumbleHigh);
    //ただの意地悪
  }

  Serial.write(tx_buff, sizeof(tx_buff));

  
}
