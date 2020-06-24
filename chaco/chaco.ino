/*
  chaco
  https://github.com/krprashant94/eagle

  5 IR sensor PID controlled bot. can follow black and white track
  with obstacle sensor
  
  The circuit:
    - Given in circuit.svg
  Sensor:
    - 5 IR sensor
  Motor Driver:
    - L293/L298/TB6612 like motor driver

  Created 04, Oct 2017
  Prashant K. Prasad
  Updated 30, Oct 2018
  Prashant K. Prasad
  Created 7, Nov 2018
  Prashant K. Prasad
  Updated 4, Feb 2019
  Prashant K. Prasad
  Updated 22, Feb 2020
  Prashant K. Prasad
*/
int SENSOR[6]= {0, 28, 27, 26, 25, 24}; //S1 to S5

#define CLP 22     //Pin number for bump(clamp) sensor 
#define NEAR 23    //Pin number for IR bump sensor

//Motor driver pins L298N(used)/L293D/TB6612FNG driver
#define IN2 50
#define IN1 51
#define IN4 52
#define IN3 53

//Motor driver PWM speed control
#define ENA 44
#define ENB 45

//Track value for an auto-calibrating system (auto type casting)
int IN_TRACK = false;
int OUT_OF_TRACK = true;

#define LED_PIN 13

//Sensor read value
int S1=0, S2=0, S3=0, S4=0, S5=0, clp = 0, near=0;

/* For PID
* kp = Proportional constant
* kd = Differential constant
* ki = Integral constant
* integral = Summation of all errors(track deviation) from last alignment(forward condition)
*/
const float kp=1, kd=2, ki=32;
int pid, prev_error, integral =0;


int carStop();
int straight();
int turnLeft();
int turnRight();
int rotateLeft();
int rotateRight();
// PID calculation algorithm, may change person to person
int PID_calc();
// Main function responsable for PID, makes speed diffrence between wheels
void speedControl(int);

void setup() {
    // Setting up input and output pins
    pinMode(SENSOR[1], INPUT);
    pinMode(SENSOR[2], INPUT);
    pinMode(SENSOR[3], INPUT);
    pinMode(SENSOR[4], INPUT);
    pinMode(SENSOR[5], INPUT);
    pinMode(CLP, INPUT);
    pinMode(NEAR, INPUT);
    
    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN3, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    
    pinMode(LED_PIN, OUTPUT);


    analogWrite(ENA, 128);
    analogWrite(ENB, 128);

//    Console log on serial port pin 0 and 1 (TX0, RX0)
//    Comment out all Serial.println() upon run for optimum performance
    Serial.begin(9600);
    
}

void loop() {
//    Reading sensor value (track info)
    S1 = digitalRead(SENSOR[1]);
    S2 = digitalRead(SENSOR[2]);
    S3 = digitalRead(SENSOR[3]);
    S4 = digitalRead(SENSOR[4]);
    S5 = digitalRead(SENSOR[5]);
    near = digitalRead(NEAR);
    clp = digitalRead(CLP);
    
    Serial.println("S1 : "+String(S1) +", "+"S2 : "+String(S2) +", "+"S3 : "+String(S3) +", "+"S4 : "+String(S4) +", "+"S5 : "+String(S5) +", ""NEAR : "+String(near) +", "+String(clp) +", ");
    
    if(near == true || clp==true){
        carStop();
        Serial.println("Obstical Found.");        
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        Serial.println("Car lost the track. Finding track...");    
        rotateLeft();
    }else{
        //PID implimentation
        pid = PID_calc();
        speedControl(pid);
        
        //Direction 
        if(S1 == IN_TRACK && S2 == IN_TRACK && S3 == IN_TRACK  && S4 == IN_TRACK && S5 == IN_TRACK ){
            // Wait a minute, it may be a chackpint, blink a LED, you can call ISR also. Be carefull dont lost the track.
            // Count the checkpint or stop or go forword
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            //Read sensor again it may be a long long check point, if needed
            pid = 0;
            prev_error = 0;
            integral = 0;
            straight();
        }else if(S5 == IN_TRACK || S4 == IN_TRACK){
           // You can add more custom condition for trrning and rotation
            turnRight();
            Serial.println("Turning");
        }else if(S2 == IN_TRACK || S1 == IN_TRACK ){
            turnLeft();
            Serial.println("turnLeft");
        }else if(S3 == IN_TRACK){
            pid = 0;
            prev_error = 0;
            integral = 0;
            straight();
        }else{
            // Anything left??
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
int PID_calc(){
    int error;
    int ret;
    if( S1 == IN_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error =4;
    }else if( S1 == IN_TRACK && S2 == IN_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 3;
    }else if( S1 == OUT_OF_TRACK && S2 == IN_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error =2;
    }else if( S1 == OUT_OF_TRACK && S2 == IN_TRACK && S3 == IN_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 1;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == IN_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error=0;
        integral=0;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == IN_TRACK && S4 == IN_TRACK && S5 == OUT_OF_TRACK){
        error=-1;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == IN_TRACK && S5 == OUT_OF_TRACK){
        error = -2;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == IN_TRACK && S5 == IN_TRACK){
        error = -3;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == IN_TRACK){
        error = -4;
    }else{
        error = prev_error;
    }    
    integral = integral + error;
    ret = kp*error + kd*(error-prev_error) + ki*integral;
    //Serial.println(String(kp*error)+", " +String(kd*(error-prev_error))+", " +String(ki*integral)+" = " +String(ret)+", " );
    prev_error = error;    
    if(ret > 128){
        return 128;
    }else if(ret < -128){
        return -128;
    }
    return ret;
}
void speedControl(int diff){
    analogWrite(ENA, 128+diff);
    analogWrite(ENB, 128-diff);
}
