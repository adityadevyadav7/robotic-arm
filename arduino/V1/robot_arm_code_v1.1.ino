#include <Servo.h>
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;
const uint8_t BASE_PIN = 3;
const uint8_t SHOULDER_PIN = 5;
const uint8_t ELBOW_PIN = 6;
const uint8_t WRIST_PIN = 9;
const uint8_t GRIPPER_PIN = 10;
uint8_t baseCurrent = 90;
uint8_t baseTarget = 90;

uint8_t shoulderCurrent = 90;
uint8_t shoulderTarget = 90;

uint8_t elbowCurrent = 90;
uint8_t elbowTarget = 90;

uint8_t wristCurrent = 90;
uint8_t wristTarget = 90;

uint8_t gripperCurrent = 60;
uint8_t gripperTarget = 60;

uint8_t minAngle[5] = { 10, 10, 20, 10, 19 };
uint8_t maxAngle[5] = { 170, 170, 170, 170, 60 };

int stepDelay=15;

const uint8_t maxPoses=30;
uint8_t poseCount=0;

bool playing;
uint8_t playIndex = 0;

struct Pose {
    uint8_t base;
    uint8_t shoulder;
    uint8_t elbow;
    uint8_t wrist;
    uint8_t gripper;
};

struct Pose savedpose[maxPoses];

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
void savePose(){
  if (poseCount<maxPoses){
    savedpose[poseCount].base=baseCurrent;
    savedpose[poseCount].shoulder=shoulderCurrent;
    savedpose[poseCount].elbow=elbowCurrent;
    savedpose[poseCount].wrist=wristCurrent;
    savedpose[poseCount].gripper=gripperCurrent;
    poseCount++;
  }
  else{
  Serial.println("MEMORY FULL");
  }
  Serial.println("Pose Saved");
}
void undo(){
  if (poseCount>0){
    poseCount--;
    playing = false;
  }
}
bool atTarget(){
  if (baseTarget==baseCurrent && shoulderTarget==shoulderCurrent && elbowTarget==elbowCurrent 
    && wristTarget==wristCurrent && gripperTarget==gripperCurrent){
    return true;
  }
  return false;
}

void play() {
  if (playIndex >= poseCount) {
    playing = false;
    playIndex = 0;
    Serial.println("Successfully played all poses.");
    return;
  }
  baseTarget = savedpose[playIndex].base;
  shoulderTarget = savedpose[playIndex].shoulder;
  elbowTarget = savedpose[playIndex].elbow;
  wristTarget = savedpose[playIndex].wrist;
  gripperTarget = savedpose[playIndex].gripper;

  if (atTarget()) {
    playIndex++;
  }
}
void clear(){
  playIndex = 0;
  playing = false;
  poseCount = 0;
  Serial.println("all poses cleared");
}

void home(){
  baseTarget=90;
  shoulderTarget=90;
  elbowTarget=90;
  wristTarget=90;
  gripperTarget=25;
  
}
void stop(){
  baseTarget=baseCurrent;
  shoulderTarget=shoulderCurrent;
  elbowTarget=elbowCurrent;
  wristTarget=wristCurrent;
  gripperTarget=gripperCurrent;
  playing=false;
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

void Shoulder(int angPle) {
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
  if (cmd == "SAVE"){
    savePose();
    return;
  }
  if (cmd == "UNDO"){
    undo();
    return;
  }
  if (cmd == "CLEAR"){
    clear();
    return;
  }
  if (cmd == "PLAY") {
    if (poseCount > 0) {
      playing = true;
      playIndex = 0;
    }
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
  Serial.println(gripperCurrent);

}

void loop(){
  readCommand();
  if (playing) {
    play();
  }
  moveServo();

}
