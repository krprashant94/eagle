class LIGHT_HCSR04
{
    public:
        int trigger;
        int echo;
        long echo_time;
        int echo_dist;
    public:
        LIGHT_HCSR04(int trig, int ech){
            trigger = trig;
            echo = ech;
        }

        /*
        * Get time taken by the ultrasonic sound 
        */
        long duration(){
            // Clears the trigPin
            digitalWrite(trigger, LOW);
            delayMicroseconds(2);
            // Sets the trigPin on HIGH state for 10 micro seconds
            digitalWrite(trigger, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigger, LOW);
            return pulseIn(echo, HIGH);
        }
        
        void begin(){
            pinMode(trigger, OUTPUT); // Sets the trigPin as an Output
            pinMode(echo, INPUT); // Sets the echoPin as an Input
        }

        /*
        * Get distance of obstical or wall
        */
        int distance(){
            // Reads the echoPin, returns the sound wave travel time in microseconds
            echo_time = duration();
            // Calculating the distance
            echo_dist = echo_time*0.34779/2;
            return echo_dist < 0 ? 0 :echo_dist;
        }
};
