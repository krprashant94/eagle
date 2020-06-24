/*
  blackhawk
  https://github.com/krprashant94/eagle
  
  An 8 IR sensor based PID controlled line follower bot
  this can follow black and white strip using IR sensor

  The circuit:
    - Given in circuit.svg
  Sensor:
    - 8 IR sensor
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

#include "Motor.h"
#include "Sensor.h"

// Calibrate kp, kd, ki at Motor.h
// TODO: Update me if you found a good value of constant 
Motor motor(7, 8, 10, 11, 5, 6);
Sensor sensor(A0,A1,A2,A3,A4,A5,3,2);

boolean data[8];

void setup() {
    // initlize devices
    motor.begin();
    sensor.begin();

    Serial.begin(9600);
    // For checkpoint
    pinMode(13, OUTPUT);
}

void loop() {
    //Checkpint detection not implimented as It vary from 
    sensor.read(data);
    Serial.println(String(data[0]) + String(data[1]) + String(data[2]) + String(data[3]) + String(data[4]) + String(data[5]) + String(data[6]) + String(data[7]));

    if(data[3] == sensor.IN_TRACK || data[4] == sensor.IN_TRACK){
        motor.forword();
    }else if(data[0] == sensor.IN_TRACK || data[1] == sensor.IN_TRACK || data[2] == sensor.IN_TRACK){
        int pid = sensor.calcPID(data[0], data[1], data[2]);
        motor.speedControl(pid);
        motor.forword();
    }else if(data[5] == sensor.IN_TRACK || data[6] == sensor.IN_TRACK || data[7] == sensor.IN_TRACK){
        int pid = sensor.calcPID(data[0], data[1], data[2]);
        motor.speedControl(pid);
        motor.forword();
    }else{
        // What to doooo... chackpoint missing
        motor.stop();
        delay(1000);
    }
    
    delay(200);
}
