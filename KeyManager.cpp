// KeyManager.cpp

#include "KeyManager.h"

KeyManager *KeyManager::instance = nullptr;

KeyManager::KeyManager()
{
  _numKeys = 0;
  instance = this; // Set the instance pointer to this instance
}

void KeyManager::add(int gpio, char id, unsigned long *holdTimes, unsigned long *delayTimes, int maxCount,
                     KeyConditionFunc condition)
{
  if (_numKeys < MAX_KEYS)
  {
    pinMode(gpio, INPUT_PULLUP);

    _keys[_numKeys].gpio = gpio;
    _keys[_numKeys].id = id;

    unsigned long *holdTimeCopy = new unsigned long[maxCount];
    unsigned long *delayTimeCopy = new unsigned long[maxCount];
    std::copy(holdTimes, holdTimes + maxCount, holdTimeCopy);
    std::copy(delayTimes, delayTimes + maxCount, delayTimeCopy);
    _keys[_numKeys].holdTime = holdTimeCopy;
    _keys[_numKeys].delayTime = delayTimeCopy;

    _keys[_numKeys].maxCount = maxCount;
    _keys[_numKeys].count = 0;
    _keys[_numKeys].reserved = 0;
    _keys[_numKeys].prevCount = 0;
    _keys[_numKeys].lastPressedTime = 0;
    _keys[_numKeys].interruptFlag = false;
    _keys[_numKeys].condition = condition; // Save the condition function

    // Attach the interrupt
    attachInterrupt(digitalPinToInterrupt(gpio), handleInterruptGlobal, FALLING);

    _numKeys++;
  }
}
void KeyManager::add(int gpio, char id, unsigned long holdTime, unsigned long delayTime, KeyConditionFunc condition)
{
  if (_numKeys < MAX_KEYS)
  {
    pinMode(gpio, INPUT_PULLUP);
    unsigned long *holdTimePtr = new unsigned long;
    *holdTimePtr = holdTime;
    _keys[_numKeys].holdTime = holdTimePtr;
    unsigned long *delayTimePtr = new unsigned long;
    *delayTimePtr = delayTime;
    _keys[_numKeys].delayTime = delayTimePtr;

    _keys[_numKeys].gpio = gpio;
    _keys[_numKeys].id = id;
    _keys[_numKeys].maxCount = 1;
    _keys[_numKeys].count = 0;
    _keys[_numKeys].reserved = 0;
    _keys[_numKeys].prevCount = 0;
    _keys[_numKeys].lastPressedTime = 0;
    _keys[_numKeys].interruptFlag = false;
    _keys[_numKeys].condition = condition; // Save the condition function

    // Attach the interrupt
    attachInterrupt(digitalPinToInterrupt(gpio), handleInterruptGlobal, FALLING);

    _numKeys++;
  }
}

void KeyManager::handleInterruptGlobal()
{
  if (instance != nullptr)
  {
    instance->handleInterrupt();
  }
}

void KeyManager::handleInterrupt()
{
  for (int i = 0; i < _numKeys; i++)
  {
    if (digitalRead(_keys[i].gpio) == LOW)
    {
      if (_keys[i].condition != nullptr && !_keys[i].condition())
      {
        continue; // Skip interrupt if the condition function returns false
      }
      _keys[i].interruptFlag = true;
    }
  }
}

void KeyManager::updateCounts()
{
  for (int i = 0; i < _numKeys; i++)
  {
    unsigned long currentMillis = millis();
    /****인터럽트 발생(키눌림, 키떔)****/
    if (_keys[i].interruptFlag)
    {
      // 키눌림 ( 키스텍이 최대 이하인가? && 쿨다임이 지났는가? )
      if (_keys[i].count < _keys[i].maxCount &&
          (currentMillis - _keys[i].lastPressedTime) > _keys[i].delayTime[_keys[i].count])
      {
        _keys[i].count++;
        _keys[i].lastPressedTime = currentMillis;
        _keys[i].changed = true;
      }
      else
      {
        if ((currentMillis - _keys[i].lastPressedTime) > MIN_DELAY_MILLIS)
        {
          _keys[i].reserved = true;
          _keys[i].reservCount++;
        }
      }
      _keys[i].interruptFlag = false;
    }
    /****유지시간 종료****/
    if (_keys[i].count != 0 && (millis() - _keys[i].lastPressedTime) > _keys[i].holdTime[_keys[i].count - 1])
    {
      _keys[i].count = 0;
      _keys[i].changed = true;
    }
  }
}

void KeyManager::printKeysCount()
{
  for (int i = 0; i < _numKeys; i++)
  {
    if (_keys[i].changed)
    {
      Serial.print("[ Key ");
      Serial.print(_keys[i].id);
      Serial.print(": ");
      Serial.print(_keys[i].count);
      Serial.print("] ");
      Serial.println();
    }
  }
}

// 바뀐 키들만 모아둔 KeyData리스트를 반환한다.
std::vector<KeyData> KeyManager::getChangedKeysData()
{
  std::vector<KeyData> changedKeysData;
  for (int i = 0; i < _numKeys; i++)
  {
    if (_keys[i].changed)
    {
      changedKeysData.push_back({_keys[i].id, _keys[i].count});
      _keys[i].changed = false; // Reset the changed flag
    }
  }
  return changedKeysData;
}

// 바뀐 키들만 모아둔 KeyData리스트를 반환한다.
std::vector<KeyData> KeyManager::getReservedKeysCount()
{
  std::vector<KeyData> changedKeysData;
  for (int i = 0; i < _numKeys; i++)
  {
    if (_keys[i].reserved)
    {
      changedKeysData.push_back({_keys[i].id, _keys[i].reservCount});
      _keys[i].reserved = false; // Reset the changed flag
      _keys[i].reservCount = 0;
    }
  }
  return changedKeysData;
}

int KeyManager::getKeyCount(char id)
{
  for (int i = 0; i < _numKeys; i++)
  {
    if (_keys[i].id == id)
    {
      return _keys[i].count;
    }
  }
  return 0; // If key with given id is not found, return 0.
}