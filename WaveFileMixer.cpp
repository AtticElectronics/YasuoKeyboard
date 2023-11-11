#include "WaveFileMixer.h"

WaveFileMixer::WaveFileMixer() {}
WaveFileMixer::~WaveFileMixer() {}
void WaveFileMixer::begin() {}

void WaveFileMixer::addStream(RAMFile *obj, uint16_t id)
{
  FileInfo fileInfo;
  fileInfo.id = id;
  fileInfo.file = new RAMFile(*obj);
  fileInfo.isDone = false;
  fileInfo.isFirst = true;
  fileInfo.file->seek(WAVE_HEADER_SIZE);

  fileInfoArray.push_back(fileInfo);
}

void WaveFileMixer::play()
{
  memset(finalBuffer, 0, BUFFER_SIZE);
  dataSize = 0;
  for (dataSize = 0; dataSize < HALF_BUFFER_SIZE; dataSize++)
  {
    bool allFilesDone = true;
    uint32_t chunk_sample = 0;
    for (auto &fileInfo : fileInfoArray)
    {
      if (!fileInfo.isDone)
      {

        if (fileInfo.file->available() >= 2)
        {
          uint32_t temp = fileInfo.file->read2Bytes();
          chunk_sample += temp;
          allFilesDone = false;
        }
        else
        {
          fileInfo.file->close();
          fileInfo.isDone = true;
          continue;
        }
      }
    }
    finalBuffer[dataSize] = chunk_sample;
    if (allFilesDone)
    {
      break; // 파일끝,루프종료
    }
  }
  clearDoneFile();
}

// 스트림에서 아이디와 같은 파일의 isDone을 true 변환하고,
// 성공여부를 반환합니다.
bool WaveFileMixer::stopStreamFile(uint16_t id)
{
  for (auto it = fileInfoArray.begin(); it != fileInfoArray.end(); ++it)
  {
    if (it->id == id)
    {
      it->isDone = true;
      return true;
    }
  }
  return false;
}

// 완료된 파일을 하나만 제거하고,
// 제거된 파일의 이름을 반환합니다.
uint16_t WaveFileMixer::clearDoneFile()
{
  for (auto it = fileInfoArray.begin(); it != fileInfoArray.end(); ++it)
  {
    if (it->isDone)
    {
      fileInfoArray.erase(it);
      return it->id; // 제거된 스트림파일의 id 반환
    }
  }
  return 0; // 제거할 스트림 파일없음
}

size_t WaveFileMixer::getFileCount() { return fileInfoArray.size(); }
bool WaveFileMixer::isPlay() { return (fileInfoArray.size() != 0); }
uint16_t *WaveFileMixer::getBufferAdr() { return finalBuffer; }
size_t WaveFileMixer::getDataSize() { return dataSize; }
