class Motor{
    public:
        int IN1, IN2, IN3, IN4, ENA, ENB;
        int IN_TRACK = 1;
        int OUT_OF_TRACK = 0;
        float kp = 1, kd = 2, ki = 4;
        int pid, prev_error = 0, integral =0;
        int topSpeed = 128;
        int onLost = 1;
        int CHECKPOINT;
        bool blinker;
    Motor(int ea, int eb, int a, int b, int c, int d, int check){
            IN1 = a;
            IN2 = b;
            IN3 = c;
            IN4 = d;
            ENA = ea;
            ENB = eb;
            CHECKPOINT = check;
            speedControl(0);
    }
    void begin(){
            pinMode(IN2, OUTPUT);
            pinMode(IN1, OUTPUT);
            pinMode(IN4, OUTPUT);
            pinMode(IN3, OUTPUT);
            pinMode(ENA, OUTPUT);
            pinMode(ENB, OUTPUT);
    }
    void stop(){
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, LOW);
        
        digitalWrite(IN4, LOW);
        digitalWrite(IN3, LOW);
    }
    void straight(){
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);
        
        digitalWrite(IN4, HIGH);
        digitalWrite(IN3, LOW);
    }
    void back(){
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, HIGH);
        
        digitalWrite(IN4, LOW);
        digitalWrite(IN3, HIGH);
    }
    void turnLeft(){    
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);
        
        digitalWrite(IN4, LOW);
        digitalWrite(IN3, LOW);
    }
    void turnRight(){    
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, LOW);
        
        digitalWrite(IN4, HIGH);
        digitalWrite(IN3, LOW);
    }
    void rotateLeft(){
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);
        
        digitalWrite(IN4, LOW);
        digitalWrite(IN3, HIGH);
    }
    void rotateRight(){    
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, HIGH);
        
        digitalWrite(IN4, HIGH);
        digitalWrite(IN3, LOW);
    }
    void wheelTest(){
        speedControl(0);
        delay(2000);
        straight();
        delay(200);
        stop();
        delay(200);
        back();
        delay(200);
        stop();
        delay(200);
        turnLeft();
        delay(200);
        stop();
        delay(200);
        turnRight();
        delay(200);
        stop();
        delay(200);
        rotateLeft();
        delay(200);
        stop();
        delay(200);
        rotateRight();
        delay(200);
        stop();
    }
    void speedTest(int t){
        speedControl(0);
        straight();
        delay(t);
        stop();
    }
    /*
    * PID calculation for 5 sensor array
    */
    int calcPID(int S1, int S2, int S4, int S5){
        int error;
        error = kp * (2*S1 + S2 - (S4 + 2*S5));
        integral += error;
        if(integral <= -topSpeed){
            integral = -topSpeed;
        }
        if(integral >= topSpeed){
            integral = topSpeed;
        }
        pid = (kp*error)+(kd*(prev_error-error))+(ki*integral);
        prev_error = error;
        if(pid <= -topSpeed){
            pid = -topSpeed;
        }
        if(pid >= topSpeed){
            pid = topSpeed;
        }
        return pid;
    }
    /*
    * PID calculation for 8 sensor array
    */
    int calcPID(int S0, int S1, int S2, int S5, int S6, int S7){
        int error;
        error = kp * (3*S0 + 2*S1 + S2 - (S5 + 2*S6 + 3*S7));
        integral += error;
        if(integral <= -topSpeed){
            integral = -topSpeed;
        }
        if(integral >= topSpeed){
            integral = topSpeed;
        }
        pid = (kp*error)+(kd*(prev_error-error))+(ki*integral);
        prev_error = error;
        if(pid <= -topSpeed){
            pid = -topSpeed;
        }
        if(pid >= topSpeed){
            pid = topSpeed;
        }
        return pid;
    }

    void directionControl(int S0, int S1, int S2, int S3, int S4, int S5, int S6, int S7){
        //checkpoint and finish point 
        if((S0 == IN_TRACK && S1 == IN_TRACK && S2 == IN_TRACK && S3 == IN_TRACK && S4 == IN_TRACK && S5 == IN_TRACK && S6 == IN_TRACK && S7 == IN_TRACK) && blinker == false){
            blinker = true;
            digitalWrite(CHECKPOINT, HIGH);
        }else if(blinker){
            blinker = false;
            digitalWrite(CHECKPOINT, LOW);
        }

        if(S3 == OUT_OF_TRACK && S4 == OUT_OF_TRACK){
            if(S0 == OUT_OF_TRACK || S1 == OUT_OF_TRACK || S2 == OUT_OF_TRACK || S5 == OUT_OF_TRACK || S6 == OUT_OF_TRACK || S7 == OUT_OF_TRACK){
                //On all OUT_OF_TRACK search for wall using ultrasonic sensor
                int l = 0, c = 0, r = 0;
                
                servo.write(0);    // sets the servo position to 0deg
                l = hcsr04.distance();
                
                servo.write(90); // sets the servo position to 90deg
                c = hcsr04.distance();
                
                servo.write(180);    // sets the servo position to 180deg
                r = hcsr04.distance();
                
                if(l != 0 && r != 0 && c == 0){
                    //straight
                    Serial.print("TODO:");
                }else if(l != 0 && r == 0 && c != 0){
                    //rotate right and straight
                    Serial.print("TODO:");
                }
                if(hcsr04.distance() != 0){
                    //rotate left and straight
                    Serial.print("TODO:");
                }
            }else{
                // PID controlled bot set Kp, Kc, Ki correctly
                if(IN_TRACK == 1){
                    pid = calcPID(S0, S1, S2, S5, S6, S7);
                }else{
                    pid = calcPID(!S0, !S1, !S2, !S5, !S6, !S7);
                }
                speedControl(pid);
            }
        }else{
            // You are on a straight path, increase your speed and enjoy the ride.
            integral = 0;
            pid = 0;
            speedControl(0);
            straight();
        }
        //Serial.println(String(integral)+", "+String(topSpeed)+", "+String(pid));
    }
  
    void directionControl(int S1, int S2, int S3, int S4, int S5){
        //checkpoint and finish point 
        if((S1 == IN_TRACK && S2 == IN_TRACK && S3 == IN_TRACK && S4 == IN_TRACK && S5 == IN_TRACK) && blinker == false){
            blinker = true;
            digitalWrite(CHECKPOINT, HIGH);
        }else if(blinker){
            blinker = false;
            digitalWrite(CHECKPOINT, LOW);
        }

        if(S3 == OUT_OF_TRACK){
            if(S3 == OUT_OF_TRACK || S1 == OUT_OF_TRACK || S2 == OUT_OF_TRACK || S4 == OUT_OF_TRACK || S5 == OUT_OF_TRACK){
                //On all OUT_OF_TRACK search for wall using ultrasonic sensor
                int l = 0, c = 0, r = 0;
                
                servo.write(0);    // sets the servo position to 0deg
                l = hcsr04.distance();
                
                servo.write(90); // sets the servo position to 90deg
                c = hcsr04.distance();
                
                servo.write(180);    // sets the servo position to 180deg
                r = hcsr04.distance();
                
                if(l != 0 && r != 0 && c == 0){
                    //straight
                    //Serial.print("TODO:");
                }else if(l != 0 && r == 0 && c != 0){
                    //rotate right and straight
                    //Serial.print("TODO:");
                }
                if(hcsr04.distance() != 0){
                    //rotate left and straight
                    //Serial.print("TODO:");
                }
            }else{
                // PID controlled bot set Kp, Kc, Ki correctly
                if(IN_TRACK == 1){
                    pid = calcPID(S1, S2, S4, S5);
                }else{
                    pid = calcPID(!S1, !S2, !S4, !S5);
                }
                speedControl(pid);
            }
        }else{
            // You are on a straight path, increase your speed and enjoy the ride.
            integral = 0;
            pid = 0;
            speedControl(0);
            straight();
        }
        //Serial.println(String(integral)+", "+String(topSpeed)+", "+String(pid));
    }
    
    void speedControl(int diff){
        analogWrite(ENA, topSpeed-diff);
        analogWrite(ENB, topSpeed+diff);
    }
};
