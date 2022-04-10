
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

struct PS5_val{
  bool circle_button = false,
       r1_shot_spd   = false,
       l1_shot_spd   = false;

  uint8_t r_stick_x = 0,
          r_stick_y = 0,
          l_stick_x = 0,
          l_stick_y = 0,
          r2_val    = 0,
          l2_val    = 0;
          
  uint8_t tx_buff[8] = {0};

  void to_buff();
};

PS5_val ps;

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

  if(ps.circle_button ^ PS5.getButtonPress(CIRCLE)){
    ps.circle_button^= true;
  }
  if(ps.r1_shot_spd ^ PS5.getButtonPress(R1)){
    ps.r1_shot_spd ^= true;
  }
  if(ps.l1_shot_spd ^ PS5.getButtonPress(L1)){
    ps.l1_shot_spd ^= true; 
  }

  ps.r_stick_x = PS5.getAnalogHat(RightHatX);
  ps.r_stick_y = PS5.getAnalogHat(RightHatY);
  ps.l_stick_x = PS5.getAnalogHat(LeftHatX);
  ps.l_stick_y = PS5.getAnalogHat(LeftHatY);

  ps.r2_val = PS5.getAnalogButton(R2);
  ps.l2_val = PS5.getAnalogButton(L2);
  

  if(ps.circle_button){
    PS5.setRumbleOn(RumbleHigh);
    //ただの意地悪
  }

  ps.to_buff();

  Serial.write(ps.tx_buff, sizeof(ps.tx_buff));
}

void PS5_val::to_buff(){
  this->tx_buff[BUTTON] = this->circle_button + (this->r1_shot_spd << 1) + (this->l1_shot_spd << 2);
  this->tx_buff[R_STICK_X] = this->r_stick_x;
  this->tx_buff[R_STICK_Y] = this->r_stick_y;
  this->tx_buff[L_STICK_X] = this->l_stick_x;
  this->tx_buff[L_STICK_Y] = this->l_stick_y;
  this->tx_buff[R2_VAL]    = this->r2_val;
  this->tx_buff[l2_VAL]    = this->l2_val;
}
