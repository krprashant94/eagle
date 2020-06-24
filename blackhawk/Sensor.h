class Sensor{
    public:
        int S1, S2, S3, S4, S5, S6, S7, S8;
        int IN_TRACK = 0, OUT_OF_TRACK = 1;
        int topSpeed;
    
        int pid, prev_error = 0, integral = 0;
        float kp = 1, kd = 0.2, ki = 0.4;
    
        Sensor(int se1, int se2, int se3, int se4, int se5, int se6, int se7, int se8){
            S1 = se1;
            S2 = se2;
            S3 = se3;
            S4 = se4;
            S5 = se5;
            S6 = se6;
            S7 = se7;
            S8 = se8;
        }
    
        void begin(){
            pinMode(S1, INPUT);
            pinMode(S2, INPUT);
            pinMode(S3, INPUT);
            pinMode(S4, INPUT);
            pinMode(S5, INPUT);
            pinMode(S6, INPUT);
            pinMode(S7, INPUT);
            pinMode(S8, INPUT);
            
            topSpeed = 128;
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
    
        void read(boolean *S_D){
            S_D[0] = digitalRead(S1);
            S_D[1] = digitalRead(S2);
            S_D[2] = digitalRead(S3);
            S_D[3] = digitalRead(S4);
            S_D[4] = digitalRead(S5);
            S_D[5] = digitalRead(S6);
            S_D[6] = digitalRead(S7);
            S_D[7] = digitalRead(S8);
        }
};
