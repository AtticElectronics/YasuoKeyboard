// RAMFile.cpp
#include "RAMFile.h"

RAMFile::RAMFile(const char *filePath, ProgressCallback callback) : position(0)
{
  vcallback = callback;

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open(filePath);
  if (!file || file.isDirectory())
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  size = file.size();
  try
  {

    data = std::make_shared<std::vector<uint8_t>>(size);
  }
  catch (const std::bad_alloc &e)
  {
    esp_restart();
  }

  for (size_t i = 0; i < size; i++)
  {
    (*data)[i] = file.read();

    if (vcallback)
    {
      vcallback();
    }
  }
  file.close();
}

RAMFile::RAMFile(const RAMFile &other)
{
  this->position = other.position;
  this->size = other.size;
  this->data = other.data;
}

// empty destructor, smart pointer will automatically deallocate memory
RAMFile::~RAMFile()
{
}

std::shared_ptr<std::vector<uint8_t>> RAMFile::getData() { return data; }

size_t RAMFile::getSize() { return size; }

int RAMFile::read()
{
  if (position < size)
  {
    return (*data)[position++];
  }
  else
  {
    return -1; // EOF
  }
}

size_t RAMFile::read(uint8_t *buf, size_t len)
{
  size_t toRead = len;
  if (position + len > size)
  {
    toRead = size - position; // don't read past the end of the file
  }
  std::copy(data->begin() + position, data->begin() + position + toRead, buf);
  position += toRead;
  return toRead; // Return the actual number of bytes read
}

uint16_t RAMFile::read2Bytes()
{
  uint16_t result = 0;

  if (available() >= 2)
  {
    // 여기서는 little endian을 가정하고, 첫 번째 바이트가 하위 바이트이고 두 번째 바이트가 상위 바이트라고 가정합니다.
    // 시스템이 big endian인 경우, 아래 두 줄의 순서를 바꿔야 할 수 있습니다.
    result = (*data)[position++];
    result |= ((*data)[position++] << 8);
    return result;
  }
  else
  {
    return 0;
  }
}

int RAMFile::available() { return size - position; }

void RAMFile::close()
{
  data.reset();
  size = 0;
  position = 0;
}

bool RAMFile::seek(size_t pos)
{
  if (pos < size)
  {
    position = pos;
    return true;
  }
  else
  {
    return false;
  }
}
