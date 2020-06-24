void menuISR(){
    open_menu = true;
}
void menu(){
    open_menu = false;
    Serial.println("-----------------------------------------------------");
    Serial.println("");
    Serial.println("Line Follower Robot");
    Serial.println("https://github.com/krprashant94/Arduino-Line-Follower");
    Serial.println("IR Sensor: " + String(sensor.SENSOR_PIN[1]) + ", " + String(sensor.SENSOR_PIN[2]) + ", " + String(sensor.SENSOR_PIN[3]) + ", " + String(sensor.SENSOR_PIN[4]) + ", " + String(sensor.SENSOR_PIN[5]) + ", " + String(sensor.SENSOR_PIN[6]) + ", " + String(sensor.SENSOR_PIN[7]));
    Serial.println("Motor: " + String(motor.IN1) + ", " + String(motor.IN2) + ", " + String(motor.IN3) + ", " + String(motor.IN4) + ", " + String(motor.ENA) + ", " + String(motor.ENB));
    Serial.println("HCSR04: " + String(hcsr04.trigger) + ", " +String(hcsr04.echo));
    Serial.println("Servo: " + String(SERVO_PIN));
    Serial.println("");
    Serial.println("-----------------------------------------------------");
    Serial.println(" 1. Max Speed");
    Serial.println(" 2. Diagnostic");
    Serial.println(" 3. Read Sensor");
    Serial.println(" 4. Set Track");
    Serial.println(" 5. 1 Second Forward");
    Serial.println(" 6. 1 Second Left");
    Serial.println(" 7. 1 Second Right");
    Serial.println(" 8. Kp");
    Serial.println(" 9. Kd");
    Serial.println("10. Ki");
    Serial.println("x. Exit");
    Serial.println("Enter your choice : ");
    while(true){
        
        if (Serial.available()) {
            String cmd = Serial.readString();
            Serial.println(cmd);
            if(cmd == "x\r\n"){
              break;
            }
        }

        //TODO:

    }
    
    Serial.println("--------------------EXIT-----------------------------");
}
