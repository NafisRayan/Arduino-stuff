// BTS7960 -> Arduino MEGA

//BTS7960 Motor Driver Carrier part:

// MotorRight_R_EN - always high(5V) // Pin to control the clockwise direction of Right Motor
// MotorRight_L_EN - always high(5V) // Pin to control the counterclockwise direction of Right Motor 

// MotorLeft_R_EN - always high(5V) // Pin to control the clockwise direction of Left Motor
// MotorLeft_L_EN - always high(5V) // Pin to control the counterclockwise direction of Left Motor

const int Rpwm1 = 6; // pwm output - motor A
const int Lpwm1 = 7; // pwm output - motor B
const int Rpwm2 = 2; // pwm output - motor A
const int Lpwm2 = 3; // pwm output - motor B
long pwmLvalue = 255;
long pwmRvalue = 255;
byte pwmChannel;

//Monster part
//here 1(motor) 2 3 4 5 6(motor) are different actuators and motors 1 2 goes for monster driver 1 and so on

const int Rpwm1M =4;
const int Lpwm1M =5;
const int Rpwm2M =8;
const int Lpwm2M =9;
const int Rpwm3M =10;
const int Lpwm3M =11;
const int Rpwm4M =12;
const int Lpwm4M =13;

//Arduino Mega don't have any more pins I guess :")
const int Rpwm5M =14;
const int Lpwm5M =15;
const int Rpwm6M =16;
const int Lpwm6M =17;

#include <math.h>

const char startOfNumberDelimiter = '<';
const char endOfNumberDelimiter = '>';
int robotControlState;
int last_mspeed;

void setup(){
  /*
  For Arduino Mega 2560
  Serial1 RX - pin 19
  Serial1 TX - pin 18
  */
  Serial.begin(9600);//Default Bluetooth Baudrate for HC-05
  
  //Setup PWM pins as Outputs
  pinMode(Rpwm1, OUTPUT);
  pinMode(Lpwm1, OUTPUT);
  pinMode(Rpwm2, OUTPUT);
  pinMode(Lpwm2, OUTPUT);
  
  stop_Robot();
}// void setup()

void loop(){
  //int i = 0;  
  if (Serial.available()) {
    processInput();
  }
}// void loop()

void processInput (){
  static long receivedNumber = 0;
  static boolean negative = false;
  byte c = Serial.read ();

  switch (c){
  case endOfNumberDelimiter:
    if (negative)
      SetPWM(- receivedNumber, pwmChannel);
    else
      SetPWM(receivedNumber, pwmChannel);

    // fall through to start a new number
  case startOfNumberDelimiter:
    receivedNumber = 0;
    negative = false;
    pwmChannel = 0;
    break;

  case 'F': // Go FORWARD
    go_Forward(255);
    Serial.println("forward");
    break;

  case 'B': // Go BACK
    go_Backwad(255);
    break;

  case 'R':
    turn_Right(255);
    break;

  case 'L':
    turn_Left(255);
    break;

  case 'I': // Top Right
    move_RightForward(255);
    break; 

  case 'G': // Top Left
    move_LeftForward(255);
    break;  

  case 'J': // Bottom Right
    move_RightBackward(255);
    break; 

  case 'H': // Bottom Left
    move_LeftBackward(255);
    break;  

  case 'S':
    stop_Robot();
    break;

  case 'x':
    pwmChannel = 1; // Rpwm1
    break;
  case 'y': // Lpwm1
    pwmChannel = 2;
    break;
  
  case '1': // Go FORWARD
    horizontal_Circular_Rotation_Right(255);
    Serial.println("horizontal_Circular_Rotation_Right");
    break;
  
  case '2': // Go BACK
    horizontal_Circular_Rotation_Left(255);
    Serial.println("horizontal_Circular_Rotation_Left");  
    break;
  
  case '3':
    front_Back_Actuator_Front(255);
    Serial.println("front_Back_Actuator_Front");
    break;
  
  case '4':
    front_Back_Actuator_Back(255);
    Serial.println("front_Back_Actuator_Back");
    break;
  
  case '5': // Top Right
    up_Down_Actuator_Up(255);
    Serial.println("up_Down_Actuator_Up");
    break; 
  
  case '6': // Top Left
    up_Down_Actuator_Down(255);
    Serial.println("up_Down_Actuator_Down");
    break;  
  
  case '7': // Bottom Right
    wrist_Actuator_down(255);
    Serial.println("wrist_Actuator_down");
    break; 
  
  case '8': // Bottom Left
    wrist_Actuator_up(255);
    Serial.println("wrist_Actuator_up");
    break;  
  
  case '9':
    claw_Actuator_open(255);
    Serial.println("claw_Actuator_open");
    break;
  
  case '0':
    claw_Actuator_close(255);
    Serial.println("claw_Actuator_close");
    break;


  } // end of switch
} // void processInput ()

void stop_Robot(){ // robotControlState = 0
  if(robotControlState!=0){
    analogWrite(Rpwm1, 0);
    analogWrite(Lpwm1, 0);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm2, 0);
    robotControlState = 0;
  }
}// void stopRobot()

void turn_Right(int mspeed){ // robotControlState = 1
  if(robotControlState!=1 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, 0);
    analogWrite(Lpwm1, mspeed);
    analogWrite(Rpwm2, mspeed);
    analogWrite(Lpwm2, 0);
    robotControlState=1;
    last_mspeed=mspeed;
  }
}// void turn_Right(int mspeed)

void turn_Left(int mspeed){ // robotControlState = 2
  if(robotControlState!=2 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, mspeed);
    analogWrite(Lpwm1, 0);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm2, mspeed);
    robotControlState=2;
    last_mspeed=mspeed;
  }
}// void turn_Left(int mspeed)

void go_Forward(int mspeed){ // robotControlState = 3
  if(robotControlState!=3 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, mspeed);
    analogWrite(Lpwm1, 0);
    analogWrite(Rpwm2, mspeed);
    analogWrite(Lpwm2, 0);
    robotControlState=3;
    last_mspeed=mspeed;
  }
}// void goForward(int mspeed)

void go_Backwad(int mspeed){ // robotControlState = 4
  if(robotControlState!=4 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, 0);
    analogWrite(Lpwm1, mspeed);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm2, mspeed);
    robotControlState=4;
    last_mspeed=mspeed;
  }
}// void goBackwad(int mspeed)

void move_RightForward(int mspeed){ // robotControlState = 5
  if(robotControlState!=5 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, mspeed*0.4);
    analogWrite(Lpwm1, 0);
    analogWrite(Rpwm2, mspeed);
    analogWrite(Lpwm2, 0);
    robotControlState=5;
    last_mspeed=mspeed;
  }
}// void move_RightForward(int mspeed)

void move_LeftForward(int mspeed){ // robotControlState = 6
  if(robotControlState!=6 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, mspeed);
    analogWrite(Lpwm1, 0);
    analogWrite(Rpwm2, mspeed*0.4);
    analogWrite(Lpwm2, 0);
    robotControlState=6;
    last_mspeed=mspeed;  
  }
}// move_LeftForward(int mspeed)

void move_RightBackward(int mspeed){ // robotControlState = 7
  if(robotControlState!=7 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, 0);
    analogWrite(Lpwm1, mspeed*0.4);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm2, mspeed);
    robotControlState=7;
    last_mspeed=mspeed;  
  }
}// void move_RightBackward(int mspeed)

void move_LeftBackward(int mspeed){ // robotControlState = 8
  if(robotControlState!=8 || last_mspeed!=mspeed){
    analogWrite(Rpwm1, 0);
    analogWrite(Lpwm1, mspeed);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm2, mspeed*0.4);
    robotControlState=8; 
    last_mspeed=mspeed; 
  }
}// void move_LeftBackward(int mspeed)

void stopRobot(int delay_ms){
  analogWrite(Rpwm1, 0);
  analogWrite(Lpwm1, 0);
  analogWrite(Rpwm2, 0);
  analogWrite(Lpwm2, 0);
  delay(delay_ms);
}// void stopRobot(int delay_ms)

void horizontal_Circular_Rotation_Right(int mspeed){ // robotControlState = 1
  if(robotControlState!=1 || last_mspeed!=mspeed){
    analogWrite(Rpwm1M, mspeed);
    analogWrite(Lpwm1M, 0);

    robotControlState=1;
    last_mspeed=mspeed;
  }
}

void horizontal_Circular_Rotation_Left(int mspeed){ // robotControlState = 2
  if(robotControlState!=2 || last_mspeed!=mspeed){
    analogWrite(Rpwm1M, 0);
    analogWrite(Lpwm1M, mspeed);

    robotControlState=2;
    last_mspeed=mspeed;
  }
}

void front_Back_Actuator_Front(int mspeed){ // robotControlState = 3
  if(robotControlState!=3 || last_mspeed!=mspeed){
    analogWrite(Rpwm2M, mspeed);
    analogWrite(Lpwm2M, 0);

    robotControlState=3;
    last_mspeed=mspeed;
  }
}

void front_Back_Actuator_Back(int mspeed){ // robotControlState = 4
  if(robotControlState!=4 || last_mspeed!=mspeed){
    analogWrite(Rpwm2M, 0);
    analogWrite(Lpwm2M, mspeed);

    robotControlState=4;
    last_mspeed=mspeed;
  }
}
void up_Down_Actuator_Up(int mspeed){ // robotControlState = 5
  if(robotControlState!=5 || last_mspeed!=mspeed){
    analogWrite(Rpwm3M, mspeed);
    analogWrite(Lpwm3M, 0);

    robotControlState=5;
    last_mspeed=mspeed;
  }
}
void up_Down_Actuator_Down(int mspeed){ // robotControlState = 6
  if(robotControlState!=6 || last_mspeed!=mspeed){
    analogWrite(Rpwm3M, 0);
    analogWrite(Lpwm3M, mspeed);

    robotControlState=6;
    last_mspeed=mspeed;
  }
}

void wrist_Actuator_down(int mspeed){
//robotControlState = 7
  if(robotControlState != 7 || last_mspeed!=mspeed){
    analogWrite(Rpwm4M, mspeed);
    analogWrite(Lpwm4M, 0);

    robotControlState=7;
    last_mspeed=mspeed;
  }
}


void wrist_Actuator_up(int mspeed){
//robotControlState = 8
  if(robotControlState != 8 || last_mspeed!=mspeed){
    analogWrite(Rpwm4M,0);
    analogWrite(Lpwm4M, mspeed);

    robotControlState=8;
    last_mspeed=mspeed;
  }
}

void claw_Actuator_open(int mspeed){
//robotControlState = 9
  if(robotControlState != 9 || last_mspeed!=mspeed){
    analogWrite(Rpwm5M,mspeed);
    analogWrite(Lpwm5M, 0);
  analogWrite(Rpwm6M,mspeed);
    analogWrite(Lpwm6M, 0);

    robotControlState=9;
    last_mspeed=mspeed;
  }
}


void claw_Actuator_close(int mspeed){
//robotControlState = 10
  if(robotControlState != 10 || last_mspeed!=mspeed){
    analogWrite(Rpwm5M,0);
    analogWrite(Lpwm5M, mspeed);
  analogWrite(Rpwm6M,0);
    analogWrite(Lpwm6M, mspeed);

    robotControlState=10;
    last_mspeed=mspeed;
  }
}

void SetPWM(const long pwm_num, byte pwm_channel){
  if(pwm_channel==1){ // DRIVE MOTOR
    analogWrite(Rpwm1, 0);
    analogWrite(Rpwm2, 0);
    analogWrite(Lpwm1, pwm_num);
    analogWrite(Lpwm2, pwm_num);
    pwmRvalue = pwm_num;
  }
  else if(pwm_channel==2){ // STEERING MOTOR
    analogWrite(Lpwm1, 0);
    analogWrite(Lpwm2, 0);
    analogWrite(Rpwm1, pwm_num);
    analogWrite(Rpwm2, pwm_num);
    pwmLvalue = pwm_num;
  }
}// void SetPWM (const long pwm_num, byte pwm_channel)   
