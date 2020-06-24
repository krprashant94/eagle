class Sensor{
  public:
  //S1, S2, S3, S4, S5, CLP, NEAR
    int SENSOR_PIN[6];
    int SENSOR_DATA[8];  //S1 to S5 form 1-5 and CLP , NEAR
    int CLP;
    int NEAR;
    Sensor(int s1, int s2, int s3, int s4, int s5, int cl, int ner){
      SENSOR_PIN[1] = s1;
      SENSOR_PIN[2] = s2;
      SENSOR_PIN[3] = s3;
      SENSOR_PIN[4] = s4;
      SENSOR_PIN[5] = s5;
      CLP = cl;
      NEAR = ner;
    }
    void begin(){      
      pinMode(SENSOR_PIN[1], INPUT);
      pinMode(SENSOR_PIN[2], INPUT);
      pinMode(SENSOR_PIN[3], INPUT);
      pinMode(SENSOR_PIN[4], INPUT);
      pinMode(SENSOR_PIN[5], INPUT);
      pinMode(CLP, INPUT);
      pinMode(NEAR, INPUT);
    }
  void readSensordata(){  
    SENSOR_DATA[1] = digitalRead(SENSOR_PIN[1]);
    SENSOR_DATA[2] = digitalRead(SENSOR_PIN[2]);
    SENSOR_DATA[3] = digitalRead(SENSOR_PIN[3]);
    SENSOR_DATA[4] = digitalRead(SENSOR_PIN[4]);
    SENSOR_DATA[5] = digitalRead(SENSOR_PIN[5]);
  
    SENSOR_DATA[6] = digitalRead(CLP);
    SENSOR_DATA[7] = digitalRead(NEAR);
  }
  void sensorTest(){
    myGLCD.print("Testing IR Sensor\n[Activate Sensor 1-5]", 5, 60);
    while(!digitalRead(SENSOR_PIN[1])){}
    myGLCD.print("1", 200, 65);
    while(!digitalRead(SENSOR_PIN[2])){}
    myGLCD.print("2", 200, 65);
    while(!digitalRead(SENSOR_PIN[3])){}
    myGLCD.print("3", 200, 65);
    while(!digitalRead(SENSOR_PIN[4])){}
    myGLCD.print("4", 200, 65);
    while(!digitalRead(SENSOR_PIN[5])){}
    myGLCD.print("5", 200, 65);
    while(!digitalRead(CLP)){}
    myGLCD.print("CLP", 200, 65);
    while(!digitalRead(NEAR)){}
    myGLCD.print("PASS", 200, 65);
  }
};

