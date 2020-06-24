#define TOUCH_ORIENTATION  PORTRAIT
#define USE_UTOUCH         0

#include <Adafruit_GFX.h>
#include <UTFTGLUE.h>            //we are using UTFT display methods
UTFTGLUE myGLCD(0x9341, A2, A1, A3, A4, A0); //ILI9320

#include <TouchScreen.h>

// MCUFRIEND UNO shield shares pins with the TFT.   Due does NOT work
#define YP A1   //A3 for ILI9320
#define YM 7    //9
#define XM A2
#define XP 6    //8  

TouchScreen myTouch(XP, YP, XM, YM, 300);
TSPoint tp;                      //Touchscreen_due branch uses Point

void readResistiveTouch(void)
{
    tp = myTouch.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
}
bool ISPRESSED(void)
{
    readResistiveTouch();
    return tp.z > 20 && tp.z < 1000;
}

#include <EEPROM.h>
#include "ColorName.h"
#include "Numpad.h"
#include "Motor.h"
#include "Sensor.h"
#include "menu.h"
int screenWidth, screenHeight;
int INTR_PIN = 21;
int menuOperation = 0;
int halt_time = 2;

Motor motor(46, 44, 35, 37, 39, 41); // ENA, ENB, IN1, IN2, IN3, IN4
Sensor sensor(53, 49, 50, 52, 51, 19, 20);//S1, S2, S3, S4, S5, CLP, NEAR
int x,y, key, i=0;


/*
  UI Function
*/

void menu(){
  myGLCD.print("1. Speed Control", 5, (3*screenHeight/4)+5);
  myGLCD.print("2. Diagnostic", 120, (3*screenHeight/4)+5);
  myGLCD.print("3. Sensor Test", 5, (3*screenHeight/4)+15);
  myGLCD.print("4. Track Test[B/W]", 120, (3*screenHeight/4)+15);
  myGLCD.print("5. One Sec. Run", 5, (3*screenHeight/4)+25);
  myGLCD.print("6. Kp Kd Ki", 120, (3*screenHeight/4)+25);
  myGLCD.print("7. Check Time", 5, (3*screenHeight/4)+35);
  myGLCD.print("8. Run", 120, (3*screenHeight/4)+35);
  myGLCD.print("9. Misc", 5, (3*screenHeight/4)+45);
}
void speedControlMenu(){
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0, (3*screenHeight/4)+1, 240, 360);
  myGLCD.setColor(LIME);
  myGLCD.print("1. 100 of MAX", 5, (3*screenHeight/4)+5);
  myGLCD.print("2. 130 of MAX", 120, (3*screenHeight/4)+5);
  myGLCD.print("3. 160 of MAX", 5, (3*screenHeight/4)+15);
  myGLCD.print("4. 190 of MAX", 120, (3*screenHeight/4)+15);
  myGLCD.print("5. 220 of MAX", 5, (3*screenHeight/4)+25);
  myGLCD.print("6. 240 of MAX", 120, (3*screenHeight/4)+25);
  myGLCD.print("7. MAX SPEED", 5, (3*screenHeight/4)+35);
  myGLCD.print("8. +1 SPEED", 120, (3*screenHeight/4)+35);
  myGLCD.print("9. -1 SPEED", 5, (3*screenHeight/4)+45);
}

void savePrompt(){
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0, (3*screenHeight/4)+1, 240, 360);
  myGLCD.setColor(LIME);
  myGLCD.print("1. Save", 5, (3*screenHeight/4)+5);
  myGLCD.print("2. HALT TIME +1", 120, (3*screenHeight/4)+5);
  myGLCD.print("3. HALT TIME -1", 5, (3*screenHeight/4)+15);
  myGLCD.print("4. onLost AI", 120, (3*screenHeight/4)+15);
  myGLCD.print("5. onLost Cont.", 5, (3*screenHeight/4)+25);
  myGLCD.print("6. onLost Stop", 120, (3*screenHeight/4)+25);
  myGLCD.print("Only Limited Number of Saving in Memory", 5, (3*screenHeight/4)+55);
}
void kpkdkiControl(){
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0, (3*screenHeight/4)+1, 240, 360);
  myGLCD.setColor(LIME);
  myGLCD.print("1. +0.01 Kp", 5, (3*screenHeight/4)+5);
  myGLCD.print("2. 0 Kp", 120, (3*screenHeight/4)+5);
  myGLCD.print("3. -0.01 Kp", 5, (3*screenHeight/4)+15);
  myGLCD.print("4. +0.01 Kd", 120, (3*screenHeight/4)+15);
  myGLCD.print("5. 0 Kd", 5, (3*screenHeight/4)+25);
  myGLCD.print("6. -0.01 Kd", 120, (3*screenHeight/4)+25);
  myGLCD.print("7. +0.01 Ki", 5, (3*screenHeight/4)+35);
  myGLCD.print("8. 0 Ki", 120, (3*screenHeight/4)+35);
  myGLCD.print("9. -0.01 Ki", 5, (3*screenHeight/4)+45);
}

/*End of UI*/

/*Dignostic Function*/
void dignostic(){
    screenTest();
    sensor.sensorTest();
    TrackTest();
    motor.wheelTest();
    myGLCD.print("Testing Speed\n[Start in 2 Sec.]", 5, 150);
    delay(2000);
    motor.speedTest(1000);
    myGLCD.print("PASS", 200, 155);
    myGLCD.print("Testing Speed Error\n[Start in 2 Sec.]", 5, 180);
    delay(2000);
    motor.speedTest(20);
    myGLCD.print("PASS", 200, 185);
}
void screenTest(){
  myGLCD.print("Testing Screen\n[Touch the screen]", 5, 30);
  while (!ISPRESSED()) {}
  myGLCD.print("PASS", 200, 35);
}

void TrackTest(){
  myGLCD.print("Testing Track \n[Start in 2 Sec.]", 5, 90);
  delay(2000);
  while(1){
    sensor.readSensordata();
    if(sensor.SENSOR_DATA[1] == sensor.SENSOR_DATA[5] && sensor.SENSOR_DATA[1] != sensor.SENSOR_DATA[3]){
      motor.IN_TRACK = sensor.SENSOR_DATA[3];
      motor.OUT_OF_TRACK = sensor.SENSOR_DATA[1];
      myGLCD.print("PASS", 200, 95);
      break;
    }
  }
}
void timeCalc(){
  unsigned long init = millis();
  while(1){
    sensor.readSensordata();
    if(sensor.SENSOR_DATA[3] == motor.IN_TRACK){
      break;
    }
    motor.straight();
  }
  motor.stop();
  myGLCD.print("\nTime Taken : "+String(millis()-init)+" milliSecond", 0, 50);
}
void printLog(){
  myGLCD.setColor(ROYAL_BLUE);
  myGLCD.print("Log\n------\n", 5, 200);
  myGLCD.print("Screen = OK\n", 5, 220);
  myGLCD.print("Sensor value ("+String(motor.lastSensor[9])+")= ", 5, 230);
  for(i=1;i<=7;i++){
    myGLCD.print(String(sensor.SENSOR_DATA[i]), 5, 250);
  }
  
  myGLCD.print("\n", 5, 250);
  myGLCD.print("[IN_TRACK][OUT_OF_TRACK] = "+String(motor.IN_TRACK)+String(motor.OUT_OF_TRACK)+"\n", 5, 260);
  myGLCD.print("Top Speed, onLost  = "+String(motor.topSpeed)+" | "+String(motor.onLost)+"\n", 5, 270);
  myGLCD.print("KP, KD, KI = "+String(motor.kp)+", "+String(motor.kd)+", "+String(motor.ki)+"\n", 5, 300);
  myGLCD.print("S[1-5], CLP, NEAR ="+String(sensor.SENSOR_PIN[1])+", "+String(sensor.SENSOR_PIN[2])+", "+String(sensor.SENSOR_PIN[3])+", "+String(sensor.SENSOR_PIN[4])+", "+String(sensor.SENSOR_PIN[5])+" | "+String(sensor.CLP)+", "+String(sensor.NEAR)+"\n", 5, 280);
  myGLCD.print("IN[1-4], EN[A, B] ="+String(motor.IN1)+", "+String(motor.IN2)+", "+String(motor.IN3)+", "+String(motor.IN4)+" | "+String(motor.ENA)+", "+String(motor.ENB)+"\n", 5, 290);
  myGLCD.print("\n", 5, 310);
}
/*End of dignostic*/

void menuISR(){
  TFTKeypad(0,0,screenWidth-1, screenHeight);
  menuOperation = 0;
  menu();
}


void saveToEEPROM(){
  /*
   * kp , kd , ki 
   * topSpeed 
   * IN_TRACK  OUT_OF_TRACK
  */
  int eeAddress=0;
  EEPROM.put(eeAddress, motor.kp);
  eeAddress += sizeof(float);
  EEPROM.put(eeAddress, motor.kd);
  eeAddress += sizeof(float);
  EEPROM.put(eeAddress, motor.ki);
  eeAddress += sizeof(float);
  EEPROM.put(eeAddress, motor.topSpeed);
  eeAddress += sizeof(int);
  EEPROM.put(eeAddress, motor.IN_TRACK);
  eeAddress += sizeof(int);
  EEPROM.put(eeAddress, motor.onLost);

}
void getEEPROM(){
  int eeAddress=0;
  EEPROM.get(eeAddress, motor.kp);
  eeAddress += sizeof(float);
  EEPROM.get(eeAddress, motor.kd);
  eeAddress += sizeof(float);
  EEPROM.get(eeAddress, motor.ki);
  eeAddress += sizeof(float);
  EEPROM.get(eeAddress, motor.topSpeed);
  eeAddress += sizeof(int);
  EEPROM.get(eeAddress, motor.IN_TRACK);
  motor.OUT_OF_TRACK = motor.IN_TRACK == 0? 1:0;
  eeAddress += sizeof(int);
  EEPROM.get(eeAddress, motor.onLost);
}


void setup()
{
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);

//Serial.begin(9600);
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);
  
  pinMode(A11, OUTPUT); 
  
  sensor.begin();
  motor.begin();
  
  getEEPROM();
  
  myGLCD.InitLCD(TOUCH_ORIENTATION);
  //myGLCD.clrScr();
  myGLCD.setFont(SmallFont);  
  screenWidth = myGLCD.getDisplayXSize();
  screenHeight = myGLCD.getDisplayYSize();
  
  attachInterrupt(digitalPinToInterrupt(INTR_PIN), menuISR, LOW);
  menuOperation = 11;
//  printLog();

  motor.speedControl(0);
  motor.straight();
  delay(1000);
}
void loop()
{
  if(menuOperation == 11){
    sensor.readSensordata();
    motor.directionControl(sensor.SENSOR_DATA[1], sensor.SENSOR_DATA[2], sensor.SENSOR_DATA[3], sensor.SENSOR_DATA[4], sensor.SENSOR_DATA[5], sensor.SENSOR_DATA[6], sensor.SENSOR_DATA[7]);
  } else if (menuOperation != 10) {
    motor.stop();
    if(ISPRESSED()){
      x = map(tp.x, 930, 170, 0, 320);
      y = map(tp.y, 180, 900, 0, 240);
      key = GetNumpadValue(x, y, 240, (3*screenHeight/4));
      switch(menuOperation){
        case 0:
          delay(1000);
          switch(key){
            case 1:
              menuOperation = 1;
              speedControlMenu();
              break;
            case 2:
              myGLCD.clrScr();
              dignostic();
              printLog();
              break;
            case 3:
              menuOperation = 10;
              myGLCD.clrScr();
              sensor.sensorTest();
              printLog();
              break;
            case 4:
              menuOperation = 10;
              myGLCD.clrScr();
              TrackTest();
              printLog();
              break;
            case 5:
              motor.speedTest(1000);
              break;
            case 6:
              menuOperation = 6;
              kpkdkiControl();
              break;
            case 7:
              myGLCD.clrScr();
              timeCalc();
              break;
            case 8:
              menuOperation = 11;
              myGLCD.clrScr();
              printLog();
              break;
            case 9:
              menuOperation = 9;
              savePrompt();
              break;
            default:
              break;
          }
          break;
        case 1:
          if(key != 8 && key != 9){
                menuOperation = 0;
                myGLCD.clrScr();          
          }
          switch(key){
            case 1:
              motor.topSpeed = 100;
              break;
            case 2:
              motor.topSpeed = 130;
              break;
            case 3:
              motor.topSpeed = 160;
              break;
            case 4:
              motor.topSpeed = 190;
              break;
            case 5:
              motor.topSpeed = 220;
              break;
            case 6:
              motor.topSpeed = 240;
              break;
            case 7:
              motor.topSpeed = 255;
              break;
            case 8:
              motor.topSpeed = (motor.topSpeed+1)%256;
              break;
            case 9:
              motor.topSpeed = abs((motor.topSpeed-1)%256);
              break;
            default:
              break;
          }
          if(key != 8 && key != 9){
            myGLCD.print("MAX SPEED = "+String(motor.topSpeed), 5, 300);
            delay(1000);
            menuISR();
          }else{
            myGLCD.print("\n", 5, 290);
            myGLCD.print("MAX SPEED = "+String(motor.topSpeed), 5, 300);
          }
          break;
        case 6:
        switch(key){
            case 1:
              motor.kp=motor.kp+0.01;
              break;
            case 2:
              motor.kp=0;
              break;
            case 3:
              motor.kp=motor.kp-0.01;
              break;
            case 4:
              motor.kd=motor.kd+0.01;
              break;
            case 5:
              motor.kd=0;
              break;
            case 6:
              motor.kd=motor.kd-0.01;
              break;
            case 7:
              motor.ki=motor.ki+0.01;
              break;
            case 8:
              motor.ki=0;
              break;
            case 9:
              motor.ki=motor.ki-0.01;
              break;
            default:
              break;
          }
          myGLCD.print("\n", 5, 290);
          myGLCD.print("\nKp = "+String(motor.kp)+", Kd = "+String(motor.kd)+", Ki = "+String(motor.ki), 5, 290);
          break;
          case 9:
            switch(key){
              case 1:
                menuOperation = 0;
                myGLCD.clrScr();
                saveToEEPROM();
                myGLCD.print("Saved", 100, 50);
                delay(1000);
                menuISR();
                break;
              case 2:
                halt_time = halt_time+1;
                myGLCD.print("\n", 5, 290);
                myGLCD.print("\nHALT TIME = "+String(halt_time), 5, 290);
                break;
              case 3:
                halt_time = abs(halt_time-1);
                myGLCD.print("\n", 5, 290);
                myGLCD.print("\nHALT TIME = "+String(halt_time), 5, 290);
                break;
              case 4:
                menuOperation = 0;
                motor.onLost = 1;
                myGLCD.clrScr();
                myGLCD.print("AI Mode", 100, 50);
                delay(1000);
                menuISR();
                break;
              case 5:
                menuOperation = 0;
                motor.onLost = 2;
                myGLCD.clrScr();
                myGLCD.print("Straight mode", 100, 50);
                delay(1000);
                menuISR();
                break;
              case 6:
                menuOperation = 0;
                motor.onLost = 0;
                myGLCD.clrScr();
                myGLCD.print("Stop mode", 100, 50);
                delay(1000);
                menuISR();
                break;
              default:
                  menuOperation = 0;
                  myGLCD.clrScr();
                  myGLCD.print("Canceled", 100, 50);
                  delay(1000);
                  menuISR();
                  break;
            }
          break;
        default:
          delay(1000);
          break;
      }
   }
  }else{
    motor.stop();
  }
  delay(halt_time);
}
