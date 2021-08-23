#include <moirai.h>

Moirai::Moirai(MoiraiCallback callback) {
    this->callback = callback;
    state = State::INIT;
    notify();
}

void Moirai::begin() {
    if (state != State::BEGINNING && state != State::ONGOING) {
      setPlannedEndTime(0);
      state = State::BEGINNING;
      notify();
      state = State::ONGOING;
    }
}

void Moirai::begin(unsigned long duration) {
    if (state != State::BEGINNING && state != State::ONGOING) {
      setPlannedEndTime(millis() + duration);
      state = State::BEGINNING;
      notify();
      state = State::ONGOING;
    }
}

void Moirai::checkTime() {
    if (state == State::ONGOING 
      && endTime != 0
      && millis() >= endTime) {
        state = State::ENDING;
        notify();
     }
}

void Moirai::stop() {
    state = State::STOPPED;
    notify();
}

Moirai::State Moirai::getState() {
    return state;
}

void Moirai::setPlannedEndTime(unsigned long endTime) {
    this->endTime = endTime;
}

unsigned long Moirai::getPlannedEndTime() {
    return endTime;
}

void Moirai::notify() {
    callback(*this);
}

PeriodicMoirai::PeriodicMoirai(MoiraiCallback callback, unsigned long periodInMs) 
: Moirai(callback) {
    this->periodInMs = periodInMs;
}

void PeriodicMoirai::begin() {
    Moirai::begin();
    cycleEnd = millis()+periodInMs;
}


void PeriodicMoirai::begin(unsigned long endTime) {
    Moirai::begin(endTime);
    cycleEnd = millis()+periodInMs;
}

void PeriodicMoirai::checkTime() {
    Moirai::checkTime();
    unsigned long currentTime = millis();
     if (getState() == State::ONGOING && currentTime > cycleEnd) {
         Moirai::notify();
         cycleEnd = currentTime +periodInMs;
    }       
}