#ifndef KeyManager_h
#define KeyManager_h

#include <vector>

#include "Arduino.h"

#define MAX_KEYS 7
#define MIN_DELAY_MILLIS 70

struct KeyData
{
  char id;
  int count;
};

class KeyManager
{
public:
  typedef std::function<bool(void)> KeyConditionFunc;

  struct Key
  {
    int gpio;
    char id;
    unsigned long *holdTime;
    unsigned long *delayTime;
    unsigned long lastPressedTime;
    int maxCount;
    int count;
    int prevCount;
    int reservCount;
    bool reserved;
    volatile bool interruptFlag;
    bool changed;
    std::function<bool(void)> condition;
  };

  KeyManager();
  void add(int gpio, char id, unsigned long *holdTimes, unsigned long *delayTimes, int maxCount,
           KeyConditionFunc condition = nullptr);
  void add(int gpio, char id, unsigned long holdTime, unsigned long delayTimes, KeyConditionFunc condition = nullptr);
  void updateCounts();
  void printKeysCount();

  static KeyManager *instance;

  static void handleInterruptGlobal();
  int getKeyCount(char id);
  void handleInterrupt();
  std::vector<KeyData> getChangedKeysData();
  std::vector<KeyData> getReservedKeysCount();

private:
  Key _keys[MAX_KEYS];
  int _numKeys;
};

#endif
