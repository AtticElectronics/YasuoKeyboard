#ifndef WAVEFILEMIXER_H_
#define WAVEFILEMIXER_H_

#include <Arduino.h>
#include <LittleFS.h>

#include <vector>

#include "RAMFile.h"

#define WAVE_HEADER_SIZE 44
#define BUFFER_SIZE 1024
#define HALF_BUFFER_SIZE 512
#define FADE_IN_SAMPLES 500

struct FileInfo
{
  RAMFile *file;
  bool isDone;
  bool isFirst;
  uint8_t buffer[BUFFER_SIZE];
  size_t dataSize;
  uint16_t id;
};

class WaveFileMixer
{
public:
  WaveFileMixer();
  ~WaveFileMixer();
  std::vector<FileInfo> fileInfoArray;
  uint16_t finalBuffer[HALF_BUFFER_SIZE];
  size_t dataSize;

  void begin();
  void addStream(RAMFile *obj, uint16_t id);
  bool stopStreamFile(uint16_t id);
  void play();
  size_t getFileCount();
  uint16_t clearDoneFile();
  uint16_t *getBufferAdr();
  size_t getDataSize();
  bool isPlay();

private:
  //귀찮음 올 퍼블릭
};

#endif // WAVEFILEMIXER_H_
