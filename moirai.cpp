#include <moirai.h>



Moirai::Moirai(MoiraiCallback callback) : 
callbacks(Moirai::Callbacks(callback, callback, callback, callback, callback))
{
    state = State::INIT;
    callbacks.getInitCallback()(*this);
}

Moirai::Moirai(MoiraiCallback initCallback,
               MoiraiCallback beginningCallback,
               MoiraiCallback ongoingCallback,
               MoiraiCallback endingCallback,
               MoiraiCallback stoppedCallback) : 
               callbacks(Moirai::Callbacks(initCallback,
                                           beginningCallback,
                                           ongoingCallback,
                                           endingCallback,
                                           stoppedCallback))
{
    state = State::INIT;
    callbacks.getInitCallback()(*this);
}

void Moirai::begin()
{
    if (state != State::BEGINNING && state != State::ONGOING)
    {
        setPlannedEndTime(0);
        state = State::BEGINNING;
        callbacks.getBeginningCallback()(*this);
        state = State::ONGOING;
    }
}

void Moirai::begin(unsigned long duration)
{
    if (state != State::BEGINNING && state != State::ONGOING)
    {
        setPlannedEndTime(millis() + duration);
        state = State::BEGINNING;
        callbacks.getBeginningCallback()(*this);
        state = State::ONGOING;
    }
}

void Moirai::checkTime()
{
    if (state == State::ONGOING && endTime != 0 && millis() >= endTime)
    {
        state = State::ENDING;
        callbacks.getEndingCallback()(*this);
    }
}

void Moirai::stop()
{
    state = State::STOPPED;
    callbacks.getStoppedCallback()(*this);
}

Moirai::State Moirai::getState()
{
    return state;
}

void Moirai::setPlannedEndTime(unsigned long endTime)
{
    this->endTime = endTime;
}

unsigned long Moirai::getPlannedEndTime()
{
    return endTime;
}

PeriodicMoirai::PeriodicMoirai(MoiraiCallback callback, unsigned long periodInMs)
    : Moirai(callback)
{
    this->periodInMs = periodInMs;
}

void PeriodicMoirai::begin()
{
    Moirai::begin();
    cycleEnd = millis() + periodInMs;
}

void PeriodicMoirai::begin(unsigned long endTime)
{
    Moirai::begin(endTime);
    cycleEnd = millis() + periodInMs;
}

void PeriodicMoirai::checkTime()
{
    Moirai::checkTime();
    unsigned long currentTime = millis();
    if (getState() == State::ONGOING && currentTime > cycleEnd)
    {
        callbacks.getOngoingCallback()(*this);
        cycleEnd = currentTime + periodInMs;
    }
}