/*
  Blink with Moirai

  Turns a LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products
  
  Inspired from the Blink example http://www.arduino.cc/en/Tutorial/Blink
  This one is not blocking (no use of delay function).
  The code is ready if you want to blink for a duration, or if you want to stop it at some point.
*/
#include <moirai.h>

PeriodicMoirai blinker(blinkCB, 1000); // blink every sec

boolean light;
void blinkCB(Moirai& moirai) {
  switch (moirai.getState()) {
    case Moirai::State::BEGINNING: 
      pinMode(LED_BUILTIN, OUTPUT);
      light = false;
      break;
    case Moirai::State::ONGOING: {
      light = ! light;
      digitalWrite(LED_BUILTIN, light ? HIGH : LOW);
      break; 
    }
    /* the following code is useful is the blinker is meant 
    to cease after some time or be stopped */
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED: {
      digitalWrite(LED_BUILTIN, LOW);
      break;
    } 
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  blinker.begin();
}

// the loop function runs over and over again forever
void loop() {
  blinker.checkTime();
}