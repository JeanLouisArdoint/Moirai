# Moirai
An Arduino library to manage state changes over time.

The goal of this library is to permit an asynchronous programming style.

A Moirai is basically a object which states changes as time passes. When a state change occurs, a callback function is called. 

A Moirai can have 5 different states:
 * INIT, the initial state, nothing is happening
 * BEGINNING, following a call to `begin`, the moirai has began
 * ONGOING, the moirai is ongoing (see periodic moirai)
 * ENDING, if the moirai had a planned end time, it is now past and the moirai is ending 
 * STOPPED, the moirai has been stopped explicitly by a call to `stop`.

## Using a simple Moirai

The following example is just waiting for some random time, keeping the builtin led on while waiting.

To use a Moirai, you need to create a variable:
```c++
Moirai randomWait(randomWaitCB);
```

The corresponding logic, where the reactions to the state changes are defined, is in the callback function. Here, when the Moirai begins, it changes it planned end time with a random duration, and turn on the built in led.
When it ends (the planned end time is passed), or if it is explicitely stopped, the built in led is turned off.
```c++
void randomWaitCB(Moirai& moirai) {
  switch(moirai.getState()) {
    case Moirai::State::BEGINNING:
      moirai.setPlannedEndTime(millis()+random(100, 1000));
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED:
      digitalWrite(LED_BUILTIN, LOW);
  }
}
```

At some point the Moirai has to begin. Here it is simply done in the setup function. It may happen in another Moirai, or comes from an external input like a button...

```c++
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  randomWait.begin();
}
```

In the loop function, you have to call all the Moirais so they can be updated when the time passes.

```c++
void loop() {
  randomWait.checkTime();
}
```

## Using a periodic Moirai

A periodic Moirai gets called regularly. At creation, you can pass the period, that is the duration between calls.

Here is how to create a periodic Moirai which period is 1 second:
```c++
PeriodicMoirai blinker(blinkCB, 1000);
```

The callback is called every period, with a Moirai in the ONGOING state.

## Chaining Moirai

To chain Moirais, that is having callback beginning or stopping other Moirais. The only issue here is to have the variable already defined. You may put all the Moirai variable declarations before the callback functions:

```c++
Moirai randomWait(randomWaitCB);
PeriodicMoirai elevatorLeds(elevatorCB, 100);

void randomWait(Moirai& moirai) {
    ... 
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED: {
      elevatorLeds.begin();
    }
}
void elevatorCB(Moirai& moirai) {
    ... 
    case Moirai::State::ENDING:
    case Moirai::State::STOPPED: {
      randomWait.begin();
    }
}
```

## Troubleshooting

The state of the Moirai doesn't change. Check that the checkTime function is being called in loop().
