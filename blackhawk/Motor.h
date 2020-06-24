class Motor{
    public:
        int ENA;
        int ENB;
        int IN1;
        int IN2;
        int IN3;
        int IN4;
        int topSpeed;
        
        Motor(int ea, int eb, int lf, int lb, int rf, int rb){
            ENA = ea;
            ENB = eb;
            IN1 = lf;
            IN2 = lb;
            IN3 = rf;
            IN4 = rb;
        }

        void begin(){
            topSpeed = 255;
            
            pinMode(ENA, OUTPUT);
            pinMode(ENB, OUTPUT);

            pinMode(IN1, OUTPUT);
            pinMode(IN2, OUTPUT);
            pinMode(IN3, OUTPUT);
            pinMode(IN4, OUTPUT);

            analogWrite(ENA, topSpeed);
            analogWrite(ENB, topSpeed);
        }
        void forword(){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }

        void backword(){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }


        void left(){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
        }

        void right(){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }

        void rotate_left(){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }

        void rotate_right(){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        void stop(){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
        }
        
        void speedControl(int diff){
            analogWrite(ENA, topSpeed - diff);
            analogWrite(ENB, topSpeed + diff);
        }
};
