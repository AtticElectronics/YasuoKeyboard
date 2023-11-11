
#ifndef RAMFILE_H
#define RAMFILE_H

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <functional>
#include <memory>
#include <vector>

class RAMFile
{
public:
  typedef std::function<void()> ProgressCallback;

  RAMFile(const char *filePath, ProgressCallback callback = nullptr);
  // RAMFile(const char *filePath, ProgressCallback callback = nullptr);
  RAMFile(const RAMFile &other); //TODO: 복사 생성자 테스트부터하고 메모리 누수확인
  ~RAMFile();
  size_t getSize();
  // uint8_t *getData();
  std::shared_ptr<std::vector<uint8_t>> getData();
  int read();
  size_t read(uint8_t *buf, size_t len);
  uint16_t read2Bytes();
  int available();
  void close();
  bool seek(size_t pos);

private:
  std::shared_ptr<std::vector<uint8_t>> data = nullptr; // changed to shared_ptr<vector<uint8_t>>
  // uint8_t *data = nullptr;
  size_t size = 0;
  size_t position = 0;
  ProgressCallback vcallback;
};

#endif // RAMFILE_H
