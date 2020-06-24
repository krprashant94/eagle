## Arduino Bot

![](../Five_Sensor_Line_Follower/Schematic.svg)

```c
int SENSOR[6]= {0, 28, 27, 26, 25, 24}; //S1 to S5

//Motor driver pins L298N/L293D/TB6612FNG driver
#define IN2 50
#define IN1 51
#define IN4 52
#define IN3 53

//Motor driver motoe enable pins 
#define ENA 44
#define ENB 45

//Track value for auto calebrating system
int IN_TRACK = false;
int OUT_OF_TRACK = true;

//Sensor read value
int S1=0, S2=0, S3=0, S4=0, S5=0, clp = 0, near=0;

/* For PID
* kp = proportional constant
* kd = differential constant
* ki = integral constant
* integral = summation of all errors(track deviation) from last alignment(forward condition)
*/
const float kp=1, kd=2, ki=32;
int pid, prev_error,    integral =0;


int carStop();
int straight();
int turnLeft();
int turnRight();
// PID calculation algorithm, may change person to person
int PID_calc();
// Main function responsable for PID, makes speed diffrence between wheels
void speedControl(int);
void forward();

void setup() {
    // Setting up input and output pins
    pinMode(SENSOR[1], INPUT);
    pinMode(SENSOR[2], INPUT);
    pinMode(SENSOR[3], INPUT);
    pinMode(SENSOR[4], INPUT);
    pinMode(SENSOR[5], INPUT);
    
    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN3, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    analogWrite(ENA, 128);
    analogWrite(ENB, 128);
}

void loop() {
	//    Reading sensor value (track info)
    S1 = digitalRead(SENSOR[1]);
    S2 = digitalRead(SENSOR[2]);
    S3 = digitalRead(SENSOR[3]);
    S4 = digitalRead(SENSOR[4]);
    S5 = digitalRead(SENSOR[5]);
    
    
    if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        // Car lost the track. write a batter algorithm 
        carStop();
    }else{
        // PID implimentation
        pid = PID_calc();
        speedControl(pid);

        // Direction 
        if(S5 == IN_TRACK || S4 == IN_TRACK){
            turnRight();
        }else if(S1 == IN_TRACK && S2 == IN_TRACK){
            turnLeft();
        }else{
            // straight is not so streagt in case of PID
            // Even you are calling streaght your bot will not move streaght untill your pid = 0
            // ve pid denots Right direction and +ve PID denots Left direction
            straight();
        }
    }
    // Relex or burn yourself with overload
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
int PID_calc(){
    int error;
    int ret;
    if( S1 == IN_TRACK && S2 == OUT_OF_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 4;
    }else if( S1 == IN_TRACK && S2 == IN_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 3;
    }else if( S1 == OUT_OF_TRACK && S2 == IN_TRACK && S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 2;
    }else if( S1 == OUT_OF_TRACK && S2 == IN_TRACK && S3 == IN_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 1;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == IN_TRACK && S4 == OUT_OF_TRACK && S5 == OUT_OF_TRACK){
        error = 0;
        integral = 0;
    }else if( S1 == OUT_OF_TRACK && S2 == OUT_OF_TRACK && S3 == IN_TRACK && S4 == IN_TRACK && S5 == OUT_OF_TRACK){
        error =- 1;
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
    prev_error = error;
    //Clipping value is 128 which is equal to top speed
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

```