// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Print.h"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a char[]
class StringBuilder : public Print {
 public:
  StringBuilder(char *buf, int size)
      : buffer(buf), capacity(size - 1), length(0) {
    buffer[0] = '\0';
  }

  virtual size_t write(uint8_t c);

 private:
  char *buffer;
  int capacity;
  int length;
};
}
}
