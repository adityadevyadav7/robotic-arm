#include <Servo.h>
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;
const int BASE_PIN = 3;
const int SHOULDER_PIN = 5;
const int ELBOW_PIN = 6;
const int WRIST_PIN = 9;
const int GRIPPER_PIN = 10;
int baseCurrent = 90;
int baseTarget = 90;

int shoulderCurrent = 90;
int shoulderTarget = 90;

int elbowCurrent = 90;
int elbowTarget = 90;

int wristCurrent = 90;
int wristTarget = 90;

int gripperCurrent = 60;
int gripperTarget = 60;

int minAngle[5] = { 10, 10, 20, 10, 19 };
int maxAngle[5] = { 170, 170, 170, 170, 60 };

int stepDelay=15;
void setup() {
  Serial.begin(9600);

  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  wrist.attach(WRIST_PIN);
  gripper.attach(GRIPPER_PIN);

  base.write(baseCurrent);
  shoulder.write(shoulderCurrent);
  elbow.write(elbowCurrent);
  wrist.write(wristCurrent);
  gripper.write(gripperCurrent);
}
void home(){
  baseTarget=90;
  shoulderTarget=90;
  elbowTarget=90;
  wristTarget=90;
  gripperTarget=60;
  
}
void stop(){
  baseTarget=baseCurrent;
  shoulderTarget=shoulderCurrent;
  elbowTarget=elbowCurrent;
  wristTarget=wristCurrent;
  gripperTarget=gripperCurrent;
}

void moveServo(){
  if (baseCurrent!=baseTarget){
    if(baseCurrent < baseTarget){
    baseCurrent++;}
    else if(baseCurrent > baseTarget){
    baseCurrent--;}
    base.write(baseCurrent);
  }
  if (shoulderCurrent!=shoulderTarget){
    if(shoulderCurrent < shoulderTarget){
    shoulderCurrent++;}
    else if(shoulderCurrent > shoulderTarget){
    shoulderCurrent--;}
    shoulder.write(shoulderCurrent);
  }
  if (elbowCurrent!=elbowTarget){
    if(elbowCurrent < elbowTarget){
    elbowCurrent++;}
    else if(elbowCurrent > elbowTarget){
    elbowCurrent--;}
    elbow.write(elbowCurrent);
  }
  if (wristCurrent!=wristTarget){
    if(wristCurrent < wristTarget){
    wristCurrent++;}
    else if(wristCurrent > wristTarget){
    wristCurrent--;}
    wrist.write(wristCurrent);
  }
  if (gripperCurrent!=gripperTarget){
    if(gripperCurrent < gripperTarget){
    gripperCurrent++;}
    else if(gripperCurrent > gripperTarget){
    gripperCurrent--;}
    gripper.write(gripperCurrent);
  }
  delay(stepDelay);
}
void Base(int angle) {
  if (angle<minAngle[0]||angle>maxAngle[0]){
    return;
  }
  baseTarget=angle;
  
}

void Shoulder(int angle) {
  if (angle<minAngle[1]||angle>maxAngle[1]){
    return;
  }
  shoulderTarget=angle;
  
}

void Elbow(int angle) {
  if (angle<minAngle[2]||angle>maxAngle[2]){
    return;
  }
  elbowTarget=angle;
  
}

void Wrist(int angle) {
  if (angle<minAngle[3]||angle>maxAngle[3]){
    return;
  }
  wristTarget=angle;
  
}

void Gripper(int angle) {
  if (angle<minAngle[4]||angle>maxAngle[4]){
    return;
  }
  gripperTarget=angle;
  
}

void readCommand() {
  if (!Serial.available())
    return;

  String cmd = Serial.readStringUntil('\n');
  if (cmd == "HOME"){
    home();
    return;
  }
  if (cmd == "STOP"){
    stop();
    return;
  }
  if (cmd == "STATUS"){
    sendStatus();
    return;
  }
  if (cmd.length()<2||cmd.length()>4){
    return;
  }
  char first = cmd[0];
  int angle =0;
  for(int i=1;i<cmd.length();i++){
    if(cmd[i]<'0' || cmd[i]>'9'){
        return;}
  }
  for (int i = 1; i < cmd.length(); i++){
    angle = angle * 10 + (cmd[i] - '0');
  }

  switch (first) {
    case 'B':
      Base(angle);
      return;
    case 'S' :
      Shoulder(angle);
      return;
    case 'E':
      Elbow(angle);
      return;
    case 'W':
      Wrist(angle);
      return;
    case 'G':
      Gripper(angle);
      return;
    case 'V':
      stepDelay = angle;
      return;
  }
}
void sendStatus(){
  Serial.print("P");
  Serial.print(baseCurrent);
  Serial.print(",");
  Serial.print(shoulderCurrent);
  Serial.print(",");
  Serial.print(elbowCurrent);
  Serial.print(",");
  Serial.print(wristCurrent);
  Serial.print(",");
  Serial.print(gripperCurrent);

}

void loop(){
  readCommand();

  moveServo();

}
