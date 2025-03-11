//Code for self-test routine here
#ifndef SELFTEST_H
#define SELFTEST_H

#include <Arduino.h>

class ST {
  public:
    ST(int pin);
    void beginST();

  private:
    int pin_;
};

#endif