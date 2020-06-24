/*
  harpy
  https://github.com/krprashant94/eagle

  An absolute beginner line follower the demonstrate basic 
  working of motor driver and sensor and algorithm
  
  The circuit:
    - Given in circuit.svg
  Sensor:
    - 5 IR sensor
  Motor Driver:
    - L293/L298/TB6612 like motor driver

  Created 24, June 2020
  Prashant K. Prasad
  
*/
int SENSOR[5]= {A0, A1, A2, A3, A4}; //S1 to S5

#define CLP 4     //Pin number for bump(clamp) sensor 
#define NEAR A5   //Pin number for IR bump sensor

//Motor driver pins L298N(used)/L293D/TB6612FNG driver
// Analog pin can work as digital
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10

//Motor driver PWM speed control
#define ENA 11
#define ENB 12

//Track value for an auto-calibrating system (auto type casting)
int IN_TRACK = false;
int OUT_OF_TRACK = true;

/*        
 *         Sensor read value
 * +--------------------------------+
 * |            /     ^^            |
 * |          [CLP] [NEAR]          |
 * |  [S0]  [S1]  [S2]  [S3]  [S4]  |
 * |                                |
 * +--------------------------------+
*/
int S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0, clp = 0, near = 0;

int carStop();
int straight();
int turnLeft();
int turnRight();
int rotateLeft();
int rotateRight();

void setup() {
    // Setting up input and output pins
    pinMode(SENSOR[0], INPUT);
    pinMode(SENSOR[1], INPUT);
    pinMode(SENSOR[2], INPUT);
    pinMode(SENSOR[3], INPUT);
    pinMode(SENSOR[4], INPUT);
    pinMode(CLP, INPUT);
    pinMode(NEAR, INPUT);
    
    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN3, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // 50% power
    analogWrite(ENA, 128);
    analogWrite(ENB, 128);

   // Console log on serial port pin 0 and 1 (TX0, RX0)
   // Comment out all Serial.println() upon run for optimum performance
    Serial.begin(9600);
    
}

void loop() {
   // Reading sensor value (track info)
    S0 = digitalRead(SENSOR[0]);
    S1 = digitalRead(SENSOR[1]);
    S2 = digitalRead(SENSOR[2]);
    S3 = digitalRead(SENSOR[3]);
    S4 = digitalRead(SENSOR[4]);
    near = digitalRead(NEAR);
    clp = digitalRead(CLP);
    
    Serial.println("S0 : " + String(S0) + "S1 : " + String(S1) + ", " + "S2 : " + String(S2) + ", " + "S3 : " + String(S3) + ", " + "S4 : " + String(S4) + ", " + "NEAR : " + String(near) + ", " + String(clp) + ", ");
    
    if(near == true || clp==true){
        carStop();
        Serial.println("Obstical Found.");        
    }else if( S0 == OUT_OF_TRACK && S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK){
        Serial.println("Car lost the track. Finding track...");    
        rotateLeft();
    }
    else{        
       // Direction 
        if(S3 == IN_TRACK || S4 == IN_TRACK){
           // Turning is diffrent then rotation (in bot)
            turnRight();
            Serial.println("Turn Right");
        }else if(S0 == IN_TRACK || S1 == IN_TRACK){
            turnLeft();
            Serial.println("Turn Left");
        }else{
            straight();
        }
    }
    delay(10);
}

int carStop(){
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, LOW);
    
    digitalWrite(IN4, LOW);
    digitalWrite(IN3, LOW);
    return 0;
}
int straight(){
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
    
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
    return 10;
}
int turnLeft(){    
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
    
    digitalWrite(IN4, LOW);
    digitalWrite(IN3, LOW);
    return 1;
}
int turnRight(){    
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, LOW);
    
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
    return 2;
}
int rotateLeft(){
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
    
    digitalWrite(IN4, LOW);
    digitalWrite(IN3, HIGH);
    return 3;
}
int rotateRight(){    
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, HIGH);
    
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
    return 4;
}
