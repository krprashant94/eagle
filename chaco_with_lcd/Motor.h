class Motor{
  public:
    int IN1, IN2, IN3, IN4, ENA, ENB;
    int IN_TRACK = 0;
    int OUT_OF_TRACK = 1;
    float kp = 1, kd = 2, ki = 4;
    int pid, prev_error = 0, integral =0;
    int topSpeed = 100;
    int lastSensor[10];
    int onLost = 1;
    int i =0;
    int k = 0;
  Motor(int ea, int eb, int a, int b, int c, int d){
      IN1 = a;
      IN2 = b;
      IN3 = c;
      IN4 = d;
      ENA = ea;
      ENB = eb;
      speedControl(0);
      lastSensor[9] = 9;
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
    pushHistory(9);
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
    myGLCD.print("Testing Wheel\n[Start in 2 Sec.]", 5, 120);
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
    myGLCD.print("PASS", 200, 125);
  }
  void speedTest(int t){
    speedControl(0);
    straight();
    delay(t);
    stop();
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
    void leftTrack(int S1, int S2, int S3, int S4, int S5){
          if(S1 == IN_TRACK){
            speedControl(0);
            rotateLeft();
          }else{
            pid = calcPID(S1, S2, S3);
            if(pid >= topSpeed){
              speedControl(0);
              turnLeft();
            }else{
              speedControl(pid);
              turnLeft();
            }
          }
          if(S5 == IN_TRACK || S4 == IN_TRACK){
            pushHistory(-1);
          }
          pushHistory(1);
    }
    void rightTrack(int S1, int S2, int S3, int S4, int S5){
          if(S5 == IN_TRACK){
            speedControl(0);
            rotateRight();
          }else{
            pid = calcPID(S5, S4, S3);
            if(pid >= topSpeed){
              speedControl(0);
              turnRight();
            }else{
              speedControl(-pid);
              straight();
            }
          }
          pushHistory(-1);
    }
    void fwrdTrack(int S1, int S2, int S3, int S4, int S5){
        if(lastSensor[9] != 0){
          integral = 0;
          prev_error = 0;
          speedControl(0);
          straight();
        }
        if(S1 == IN_TRACK || S2 == IN_TRACK  ){
          pushHistory(1);
        }
        if(S5 == IN_TRACK || S4 == IN_TRACK){
          pushHistory(-1);
        }
        pushHistory(0);
    }
    void directionControl(int S1, int S2, int S3, int S4, int S5, int CLP, int NEAR){
      
      if((S1 == IN_TRACK && S2 == IN_TRACK && S3 == IN_TRACK && S4 == IN_TRACK && S5 == IN_TRACK) && k == 0){
//        myGLCD.print(String(i), 0, 0);
//        Serial.println("CHECK POINT ");
        k = 1;
        digitalWrite(A11, HIGH);
        i++;
      }else if((S3 == OUT_OF_TRACK || S1 == OUT_OF_TRACK || S2 == OUT_OF_TRACK || S4 == OUT_OF_TRACK || S5 == OUT_OF_TRACK) && k == 1){
//        Serial.prifntln("clear");
        digitalWrite(A11, LOW);
        k=0;
      }
      if(CLP == 1 || NEAR == 1 || i >= 7){
//        Serial.println("STOP : "+ String(i));
        stop();
      }else if(S1 == IN_TRACK){
        leftTrack(S1, S2, S3, S4, S5);
      }
      else if(S3 == OUT_OF_TRACK){
        if(S1 == IN_TRACK || S2 == IN_TRACK  ){
          leftTrack(S1, S2, S3, S4, S5);
        }else if(S5 == IN_TRACK || S4 == IN_TRACK){
          rightTrack(S1, S2, S3, S4, S5);
        }else{
          whatToDo();
        }
      }else{
        fwrdTrack(S1, S2, S3, S4, S5);
      }
      
    }
    void speedControl(int diff){
      analogWrite(ENA, topSpeed-diff);
      analogWrite(ENB, topSpeed+diff);
    }
    void pushHistory(int input){
      // 1 for left, -1 for right
      int i;
      for(i=0; i < 9; i++){
        lastSensor[i] = lastSensor[i+1];
      }
      lastSensor[9] = input;
    }
    int getNextDirection(){
      int i;
      int l = 0, r=0;
      if(lastSensor[9] == lastSensor[8] == lastSensor[7]){
        return lastSensor[9];
      }
      for(i=9; i>=0; i--){
        if(lastSensor[i] == -1)
          r++;
        else if(lastSensor[i] == 1)
          l++;
      }
      if(l == r) return 0;
      return l>r ? 1 :-1;
    }
    void whatToDo(){
        switch(getNextDirection()){
          case -1:
            speedControl(0);
            rotateRight();
            break;
          case 1:
            speedControl(0);
            rotateLeft();
            break;
          default:
            speedControl(0);
            rotateRight();
            break;
        }
      }
};
