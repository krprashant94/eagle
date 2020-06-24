/*
  ornate
  https://github.com/krprashant94/eagle
  
  An 8 IR sensor based PID controlled line follower bot
  this can follow black and white strip using IR sensor
  with ultrasonic obstacle/wall detection

  The circuit:
    - Given in circuit.svg
  Sensor:
    - 8 IR sensor
    - HC-SR 04
  Motor Driver:
    - L293/L298/TB6612 like motor driver

  Created 04, Oct 2017
  Prashant K. Prasad
  Updated 30, Oct 2018
  Prashant K. Prasad
  Updated 07, Nov 2018
  Prashant K. Prasad
  Updated 04, Feb 2019
  Prashant K. Prasad
  
*/

#include <Servo.h>
#define SERVO_PIN 9
Servo servo;    // Create servo object to control a servo

#include "LIGHT_HCSR04.h"
LIGHT_HCSR04 hcsr04(9, 10);

#include "Sensor.h"
Sensor sensor(3, 4, 5, 6, 7, 8, A4, A5); //S1, S2, S3, S4, S5..

#include "Motor.h"
Motor motor(11, 12, A0, A1, A2, A3, 13); // ENA, ENB, IN1, IN2, IN3, IN4, CHECKPOINT

//Interrupt pin for menu
#define INTR_PIN 2

bool open_menu = false;
#include "Menu.h"

void setup() {
    Serial.begin(9600);
    motor.begin();
    sensor.begin();
    hcsr04.begin();
    servo.attach(SERVO_PIN);

    attachInterrupt(digitalPinToInterrupt(INTR_PIN), menuISR, RISING);

    // 128 is good for PID. You can increase it in straight path
    motor.topSpeed = 128;
}

void loop() {
     if(open_menu){
        menu();
     }

     
    sensor.readSensordata();
    if(sensor.SENSOR_DATA[3] == motor.IN_TRACK && sensor.same){
        // Do nothing. No change found since the last scanning.
    }else{
        // motor.directionControl(sensor.SENSOR_DATA[1], sensor.SENSOR_DATA[2], sensor.SENSOR_DATA[3], sensor.SENSOR_DATA[4], sensor.SENSOR_DATA[5]);
        motor.directionControl(sensor.SENSOR_DATA[0], sensor.SENSOR_DATA[1], sensor.SENSOR_DATA[2], sensor.SENSOR_DATA[3], sensor.SENSOR_DATA[4], sensor.SENSOR_DATA[5], sensor.SENSOR_DATA[6], sensor.SENSOR_DATA[7]);
    }
}
