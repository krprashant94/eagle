## goldenbird

    An optimized Arduino line follower that uses port manipulation for fast performance. It is a 10x-20x faster than the ordinary Arduino powered PID controlled bot. The size is reduced by \~60% as compared to other files. 

### Port Manipulation

As we know Arduino is an 8-bit processor. We can use an 8-bit line to read and write data to/from its bus. TO do that we use microprocessor register `DDRx` for Data direction register for I/O. `PORTn` for reading and writing data. 

```c
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
```

To write this at once, first, we need to know the pin mapping of [ATmega328P](/images/atmega328.png) or [ATmega2560](images/atmega2560.png)

Each bit of the `DDRx` register represents the IO of each pin where 1 represents output and 0 for input.

```c
	PORTC = 0x00;
```

Similarly, `DDRB = 0b01111100;` represent pin 9, 10, 11, 12, 13 as output

#### Reading a port

```c
    S0 = digitalRead(A0);
    S1 = digitalRead(A1);
    S2 = digitalRead(A2);
    S3 = digitalRead(A3);
    S4 = digitalRead(A4);
    S5 = digitalRead(A5);
```

TO do this simply
```c
	sensors = PORTC;
```

As we know `digitalRead()` can read value as `HIGH` or `LOW` which is the state of that pin using PORTn we can do the same work in one step. PORTn register holds 8bit data which is A0-A5, Reset and last bit is don't care.
Using bit manipulation on that register, we can do more efficient calculations in less memory and time. 
like to get `A4, A5` we have to bit shift like this `(PORTC & 0b00011000) >> 3` or `(PORTC & 0x18) >> 3`.
considering the track value (in case of inverted track) `((PORTC^TRACK) & 0x18) >> 3`.

### Pin Change Interrupt (PCINT)

In Arduino UNO and mega approximately all port have PCINT. It is a special kind of interrupt trigger when any selected pin of that port change. This interrupt applies to the port, unlike other external interrupts.

> A0-A5 comes on PCINT1 as it is in PORTC

```c	
    // Setting pin mask for all pin change detection. you can change it as per sensor count. 
    PCMSK1 = 0xFF;
    // Pin change interrupt control register
    PCICR  = PCIE1;
    // Accept external interrupt
    sei();
```

The above line of code setup pin change interrupt on PORTC. Now we have to define ISR(Interrupt Service Routine) 

```c
	// ISR can be overloaded by any external interrupt
	// PCINT1_vect passes when PCINT1 (PORTC) trigger.
	ISR(PCINT1_vect){
		// Code to read the sensor 
		// Do not perform any big calculation
		// an interrupt signal can stop other executing tasks
	}
```

Congratulations, we applied in optimization on your bot Thank You.
If you want to support me, star, fork or pull [this](https://github.com/krprashant94/marlin)

[@krprashant94](https://github.com/krprashant94/marlin)
