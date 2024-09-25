
#ifndef Moirai_h
#define Moirai_h

#include "Arduino.h"

class Moirai;

using MoiraiCallback = void (*)(Moirai &);

/**
 * Moirai class to control the destiny of stuff.4
 * version 0.2
 */
class Moirai
{
public:
  /**
   * State of the Moirai
   */
  enum State
  {
    INIT,
    BEGINNING,
    ONGOING,
    ENDING,
    STOPPED
  };
  class Callbacks
  {
  public:
    Callbacks(MoiraiCallback initCallback,
              MoiraiCallback beginningCallback,
              MoiraiCallback ongoingCallback,
              MoiraiCallback endingCallback,
              MoiraiCallback stoppedCallback)
    {
      this->initCallback = initCallback;
      this->beginningCallback = beginningCallback;
      this->ongoingCallback = ongoingCallback;
      this->endingCallback = endingCallback;
      this->stoppedCallback = stoppedCallback;
    }
    MoiraiCallback getInitCallback()
    {
      return initCallback;
    }
    MoiraiCallback getBeginningCallback()
    {
      return beginningCallback;
    }
    MoiraiCallback getOngoingCallback()
    {
      return ongoingCallback;
    }
    MoiraiCallback getEndingCallback()
    {
      return endingCallback;
    }
    MoiraiCallback getStoppedCallback()
    {
      return stoppedCallback;
    }

  private:
    MoiraiCallback initCallback;
    MoiraiCallback beginningCallback;
    MoiraiCallback ongoingCallback;
    MoiraiCallback endingCallback;
    MoiraiCallback stoppedCallback;
  };

  /**
   * Constructor, pass a callback
   */
  Moirai(MoiraiCallback callback);

  /**
   * Constructor, pass Callbacks
   */
  Moirai(MoiraiCallback initCallback,
         MoiraiCallback beginningCallback,
         MoiraiCallback ongoingCallback,
         MoiraiCallback endingCallback,
         MoiraiCallback stoppedCallback);

  /**
   *  Begins the Moirai without time limit
   */
  virtual void begin();
  /**
   *  Begins the Moirai for a duration in milliseconds
   */
  virtual void begin(unsigned long duration);
  /**
   *  Method to call in loop function
   */
  virtual void checkTime();
  /**
   * Stops the Moirai
   */
  void stop();
  /**
   * Returns the state of the Moirai
   */
  State getState();
  /**
   * Change the planned end time of the Moirai
   */
  void setPlannedEndTime(unsigned long endTime);
  /**
   * Returns the planned end time (max unsigned long if not set)
   */
  unsigned long getPlannedEndTime();

protected:
  Callbacks callbacks;

private:
  State state;
  unsigned long endTime = -1;
};

/**
 * The periodic moirai notifies in ONGOING state periodically after a given period.
 */
class PeriodicMoirai : public Moirai
{
public:
  /**
   * Creates a periodic moirai, passing the callback and the period in milliseconds
   */
  PeriodicMoirai(MoiraiCallback callback, unsigned long periodInMs);

  /**
   * Method to call in loop function
   */
  virtual void checkTime();

  /**
   *  Begins the Moirai without time limit
   */
  virtual void begin();

  /**
   *  Begins the Moirai for a duration in milliseconds
   */
  virtual void begin(unsigned long duration);

  /**
   * Returns the period of this periodic Moirai
   */
  unsigned long getPeriod() { return periodInMs; }

private:
  unsigned long periodInMs;
  unsigned long cycleEnd;
};

#endif