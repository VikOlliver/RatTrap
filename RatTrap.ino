/* RatTrap - A Rat Trap Actuator by Vik Olliver vik@diamondage.co.nz

 An IR proximity sensor (active low) is connected to SIGNALPIN and
 a hobby servo is connected to SERVO_PIN. An optional LED indicating
 that the trap has been triggered may be connected to BLINKYLIGHT_PIN.

 A video on how to construct the mechanics of the trap can be found
 on the Geeko Farm channel on YouTube, and on Playeur.

 The servo is set to the zero position, and the code awaits a solid
 signal on the proximity sensor. When this is detected, the servo is
 moved to nearly maximum travel (to avoid jitter) and the indicator
 light is set flashing.

 To reset the trap, push the RST switch on the Arduino.

 A high quality USB power supply is recommended to avoid power
 supply fluctuations and blips causing a false trigger.
 
 This code is in the public domain.

*/

#include <Servo.h>

#define SIGNAL_PIN 3        // Sensor that will trigger servo
#define BLINKYLIGHT_PIN 13  // Trap triggered indicator light
#define SERVO_PIN 9         // Hobby servo

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  pinMode(SIGNAL_PIN,INPUT_PULLUP);
  pinMode(BLINKYLIGHT_PIN,OUTPUT);
}

void loop() {
  int i;

  // Zero the servo and set LED definitively off
  myservo.write(0);
  digitalWrite(BLINKYLIGHT_PIN,0);

  i=0;
  // Sensor is active when high. Wait for it to go low for 5 readings
  // so we don't trigger on flies, spiders, dust motes, lightning etc.
  while (digitalRead(SIGNAL_PIN)!=0) {
    delay(10);                       // waits 10ms for debounce
    if ((digitalRead(SIGNAL_PIN)==0)&&(i++>4))   // If it's *still* triggered, drop out of this loop
      break;
    else  // False alarm, reset activation count.
      i=0;
  }

  // Move the servo to not quite full travel (which makes the servo hum dangerously)
  myservo.write(160);

  while(true) {    // And we can stop there until RESET is hit
    digitalWrite(BLINKYLIGHT_PIN,0);
    delay(500);
    digitalWrite(BLINKYLIGHT_PIN,1);
    delay(500);
  }

}
