/*
  Wait for a random time, with a light on, with Moirai

  Turns a LED on for a random duration and then stop.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products
  */
#include <moirai.h>

Moirai randomWait(randomWaitCB);

void randomWaitCB(Moirai& moirai) {
  switch(moirai.getState()) {
    case Moirai::State::BEGINNING:
      moirai.setPlannedEndTime(millis()+random(100, 1000));
      digitalWrite(LED_BUILTIN, HIGH); // to signal the beginning of the wait
      break;
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED: // this never happens in this sample
      digitalWrite(LED_BUILTIN, LOW);
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  randomWait.begin();
}

// the loop function runs over and over again forever
void loop() {
  randomWait.checkTime();
}