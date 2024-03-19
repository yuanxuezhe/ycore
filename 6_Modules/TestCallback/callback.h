#ifndef __TEST_CALLBACK_H__
#define __TEST_CALLBACK_H__

#include <string>
#include <sstream>
#include "define.h"
#include <iostream>
using namespace std;

extern "C" {
  DLLExport  const char* TestPrint1(char* str);
  DLLExport  const char* TestPrint2(char* str);
  DLLExport  const char* TestPrint3(char* str);
  DLLExport  const char* TestPrint4(char* str);
};

#endif // __TEST_CALLBACK_H__