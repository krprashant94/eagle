/*
  chaco
  https://github.com/krprashant94/eagle

  6(8) IR sensor PID controlled bot. can follow black and white edge track
  
  
  The circuit:
    - Given in circuit.svg
  Sensor:
    - 5 IR sensor
  Motor Driver:
    - L293/L298/TB6612 like motor driver

  Created 7, Nov 2018
  Prashant K. Prasad
  Updated 4, Feb 2019
  Prashant K. Prasad
*/
#include "Motor.h"
#include "Sensor.h"

int LED_PIN = 13;
Motor motor(46, 44, 40, 36, 38, 42);
// You can disable any pin and change your "if condition" accordingly
Sensor sensor(A0, A1, A2, A3, A4, A5);
int pid;

void setup() {
    // put your setup code here, to run once:
    motor.begin();
    sensor.begin();
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    motor.speedControl(0);
    sensor.read();
    sensor.IN_TRACK = sensor.S1_D;
}

void loop() {
    sensor.read();
    Serial.println(sensor.S1_D);
    Serial.println(sensor.S2_D);
    Serial.println(sensor.S3_D);
    Serial.println(sensor.S4_D);
    Serial.println(sensor.S5_D);

    if(sensor.S1_D ==  sensor.S6_D){
        motor.stop();
        delay(10);
    }else if(sensor.S1_D == sensor.IN_TRACK && sensor.S2_D == sensor.IN_TRACK && sensor.S3_D == sensor.IN_TRACK && sensor.S4_D == sensor.IN_TRACK && sensor.S5_D == sensor.IN_TRACK  && sensor.S6_D == sensor.IN_TRACK){
       // Serial.println("CHK");
        digitalWrite(LED_PIN, HIGH);
        motor.speedControl(0);
        motor.forword();
        digitalWrite(LED_PIN, LOW);
    }else if((sensor.S1_D == sensor.IN_TRACK && sensor.S2_D == sensor.IN_TRACK && sensor.S3_D == sensor.IN_TRACK) && (sensor.S4_D != sensor.IN_TRACK && sensor.S5_D != sensor.IN_TRACK && sensor.S6_D != sensor.IN_TRACK)){
       // Serial.println("ST");
        motor.speedControl(0);
        motor.forword();
    }else if(sensor.S4_D == sensor.IN_TRACK || sensor.S5_D == sensor.IN_TRACK || sensor.S6_D == sensor.IN_TRACK){
       // Serial.println("LFT");
        motor.speedControl(0);
        motor.left();
    }else if(sensor.S1_D != sensor.IN_TRACK || sensor.S2_D != sensor.IN_TRACK || sensor.S3_D != sensor.IN_TRACK){
       // Serial.println("RT");
        motor.speedControl(0);
        motor.right();
    }

    delay(100);
}
