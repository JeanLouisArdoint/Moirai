/*
  Fade with Moirai

  This example shows how to fade an LED on pin 9 using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

  Inspired from the Fade example http://www.arduino.cc/en/Tutorial/Fade
  This one is not blocking (no use of delay function).
  The code is ready if you want to fade for a duration, or if you want to stop it at some point.

*/
#include <moirai.h>

const int led = 9;     // the PWM pin the LED is attached to
int brightness;    // how bright the LED is
int fadeAmount;    // how many points to fade the LED by


void fadeCB(Moirai& moirai) {
  switch (moirai.getState()) {
    case Moirai::State::BEGINNING: 
      pinMode(led, OUTPUT);
      brightness = 0;
      fadeAmount = 5;
      break;
    case Moirai::State::ONGOING: {
      // set the brightness of pin 9:
      analogWrite(led, brightness);

       // change the brightness for next time through the loop:
       brightness = brightness + fadeAmount;

       // reverse the direction of the fading at the ends of the fade:
       if (brightness <= 0 || brightness >= 255) {
         fadeAmount = -fadeAmount;
        }
        break;
    }
    /* the following code is useful is the fader is meant 
    to cease after some time or be stopped */
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED: {
      analogWrite(led, 0);
      break;
    } 
  }
}

PeriodicMoirai fader(fadeCB, 40);

// the setup function runs once when you press reset or power the board
void setup() {
  fader.begin();
}

// the loop function runs over and over again forever
void loop() {
  fader.checkTime();
}
