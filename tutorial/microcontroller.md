## Microcontroller
The microcontroller is the brain of your line follower. you can select a wide range of microcontrollers for your line follower. Some of are:

- Arduino 
- AVR 
- 8085
and soo many others.

In your bot microcontroller will take input from the sensor and it will do some calculation, based on those input microcontroller trigger output signal to control external devices like a motor driver. 

#### [Arduino](https://arduino.cc)
Arduino is an open-source microcontroller board and most common among robotics geeks, due to its easy to use concept. It has an onboard microcontroller that are listed below

| Arduino Model | Microcontroller | Flash Memory | EEPROM | IO Pins      |
|---------------|-----------------|--------------|--------|--------------|
| Uno/Pro Mini  | ATmega328P      | 32 KB        | 1 KB   | 14 + 6 = 20  |
| Mini          | ATmega328P      | 32 KB        | 1 KB   | 14 + 8 = 22  |
| [Mega 2560](images/arduino-mega2560-rev3.png)     | [ATmega2560](images/atmega2560.png)      | 256 KB       | 4 KB   | 54 + 16 = 70 |
| Micro         | ATmega32U4      | 32           | 1      | 20 + 12 = 32 |

Full information of the board are listed [here](https://www.arduino.cc/en/products/compare)

To program arduino you can use [Arduino Software](https://www.arduino.cc/en/main/software) 

##### [AVR](https://en.wikipedia.org/wiki/AVR_microcontrollers)

AVR is a microcontroller sometime it is referred to without us of Arduino firmware. I am writing AVR in the same context. 

Most common AVR microcontroller is ATmega328P, ATmega16, and ATmega32. You can select any of them. An AVR micro-controller needs a programmer(A special kind of USB). Some method of programming an AVR are
- USBasp
- Arduino as ISP ([ICSP](https://www.arduino.cc/en/Tutorial/ArduinoISP))
- USB to TTL Serial (UART)

To upload code on AVR, you need to assemble a simple circuit in breadboard/PCB (or you can purchase) you can use [Amtel Studio](https://www.microchip.com/mplab/avr-support/atmel-studio-7) to write code. 

[AVR Community](https://www.avrfreaks.net/) 

#### 8085
You can use an 8085 microprocessor into your project if you can program that processor. I am not writing about this because 8085 is in your B. Tech. syllabus and you will get many resources about that in the internet.
