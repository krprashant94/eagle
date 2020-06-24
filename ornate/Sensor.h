class Sensor{
    public:
        int count = 0;
        bool same;
        int SENSOR_PIN[8];
        int SENSOR_DATA[8];
        int PREV_DATA[8];
        
        Sensor(int s0, int s1, int s2, int s3, int s4, int s5, int s6, int s7){
            count = 8;
            SENSOR_PIN[0] = s0;
            SENSOR_PIN[1] = s1;
            SENSOR_PIN[2] = s2;
            SENSOR_PIN[3] = s3;
            SENSOR_PIN[4] = s4;
            SENSOR_PIN[5] = s5;
            SENSOR_PIN[6] = s6;
            SENSOR_PIN[7] = s7;
        }
        Sensor(int s0, int s1, int s2, int s3, int s4){
            count = 5;
            SENSOR_PIN[0] = s0;
            SENSOR_PIN[1] = s1;
            SENSOR_PIN[2] = s2;
            SENSOR_PIN[3] = s3;
            SENSOR_PIN[4] = s4;
        }
        
        void begin(){
          int i;
            for (i = 0; i<count; i++){
                pinMode(SENSOR_PIN[i], INPUT);
            }
        }
        
        void readSensordata(){
            int i;
            for (i = 0; i<count; i++){
                SENSOR_DATA[i] = digitalRead(SENSOR_PIN[i]);
            }
            for (i = 0; i<count; i++){
                same = true;
                if(PREV_DATA[i] != SENSOR_DATA[i]){
                    same = false;
                    break;
                }
            }
            for (i = 0; i<count; i++){
                PREV_DATA[i] = SENSOR_DATA[i];
            }
        }
};
