class Sensor{
    public:
    int S1, S2, S3, S4, S5, S6;
    int S1_D, S2_D, S3_D, S4_D, S5_D, S6_D;
    int IN_TRACK = 1, OUT_OF_TRACK = 0;
    int topSpeed;

    int pid, prev_error = 0, integral = 0;
    float kp = 1, kd = 0.2, ki = 0.4;

    Sensor(int se1, int se2, int se3, int se4, int se5, int se6){
        S1 = se1;
        S2 = se2;
        S3 = se3;
        S4 = se4;
        S5 = se5;
        S6 = se6;
    }

    void begin(){
        pinMode(S1, INPUT);
        pinMode(S2, INPUT);
        pinMode(S3, INPUT);
        pinMode(S4, INPUT);
        pinMode(S5, INPUT);
        pinMode(S6, INPUT);
        
        topSpeed = 100;
    }


    int calcPID(int S1, int S2, int S3){
        int error;
        error = kp*(2*S1+S2+S3);
        integral += error;
        if(integral > topSpeed){
            integral = topSpeed;
        }
        pid = (kp*error)+(kd*(prev_error-error))+(ki*integral);
        prev_error = error;
        return pid;
    }

    void read(){
        S1_D = digitalRead(S1);
        S2_D = digitalRead(S2);
        S3_D = digitalRead(S3);
        S4_D = digitalRead(S4);
        S5_D = digitalRead(S5);
        S6_D = digitalRead(S6);
        // Serial.println(String(S1_D) + String(S2_D) + String(S3_D) + String(S4_D) + String(S5_D) + String(S5_D));
    }
        
        
};
