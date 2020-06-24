/*
  goldenbird
  https://github.com/krprashant94/eagle

  Demonstration of port manipulation and PID controlled Arduino bot
  It performs bit manipulation on microprocessor register for
  reading and writing operation that save too much space and 
  makes switching too fast and parallel. That makes more than 
  10x faster reading and writing speed on the port.
  ~60% less program size.  

  The circuit:
    - Given in circuit.svg
  Sensor:
    - 5 IR sensor
  Motor Driver:
    - L293/L298/TB6612 like motor driver

  Created 2020
  Prashant K. Prasad
  
*/

#define LED_PIN 13

//Track value, changing number of sensor need to change PID algorithm
const uint8_t TRACK = 0b00100;

int proportional;             // Kp
int integral;                 // Ki
const int differential = 100; // inverse of Kd

int carStop();
int straight();
int turnLeft();
int turnRight();

// Function for PID
int error_calc();
void speedControl(int);

ISR(PCINT1_vect){
    //Serial.println("Data changed");
    int reading = PORTC >> 5 & 1;
    if(reading == 1){
        carStop();
    }
    reading = PORTC & 0b00011111;
    if(reading == 0b00000100){
        straight();
        speedControl(0);
        proportional = 0;
        integral  = 0;
    }else if(reading == 0b00011000){
        //Kp = 1
        proportional = error_calc();
        // or simply
        // turnLeft();
    }else if(reading == 0b00000011)
         proportional = error_calc();
        //turnRight();
    }

void setup() {
    // 5 sensor and one obsticle A0-A4 and A5
    DDRC = 0x00;
    //2 motor PWM output: Pin 5, 6
    DDRD = 0b00000110;
    //4 Motor direction: Pin 9, 10, 11, 12 and LED: 13
    DDRB = 0b01111100;

    //Enable PCINT
    PCMSK1 = 0xFF;
    PCICR  = PCIE1;
    sei();

    proportional = 0;
    integral = 0;
    
    //Console log on serial port pin 0 and 1 (TX0, RX0)
    //Serial.begin(9600);
}

void loop() {  
    if(proportional != 0){
        // Ki = 1
        integral += proportional;
        // Yes, its a PID by https://github.com/krprashant94
        speedControl(integral);
    }
    // Delay should be inverse of Kd, as less the value of delay, the bot will take a more sharp turn.
    delay(differential);
}

int carStop(){
    PORTB &= 0b00000100;
    return 0;
}
int straight(){
    PORTB &= 0b00000100;
    PORTB |= 0b00101000;
    return 10;
}
int turnLeft(){
    PORTB &= 0b00000100;
    PORTB |= 0b00100000;
    return 1;
}
int turnRight(){
    PORTB &= 0b00000100;
    PORTB |= 0b00001000;
    return 2;
}
int rotateLeft(){
    PORTB &= 0b00000100;
    PORTB |= 0b00110000;
    return 3;
}
int rotateRight(){
    PORTB &= 0b00000100;
    PORTB |= 0b01001000;
    return 4;
}
int error_calc(){
    uint8_t str =  (PORTC>>2) & 0b11;
    uint8_t right = (PORTC^TRACK) & 0b00000011;
    uint8_t left = ((PORTC^TRACK) & 0b00011111)>>3;
    if (str != 0){
        return 0;
    }else if (right != 0){
        switch(right){
            case 0b10:
                return 1;
            case 0b11:
                return 2;
            case 0b01:
                return 3; 
            default:
                return 0;
        }
    }else if (left != 0){
        switch(left){
            case 0b01:
                return -1;
            case 0b11:
                return -2;
            case 0b10:
                return -3; 
            default:
                return 0;
        }
    }
}
void speedControl(int diff){
    analogWrite(5, 128+diff);
    analogWrite(6, 128-diff);
}
