#ifndef BASE_H
#define BASE_H
#include <json/value.h>
#include <string>


struct PrinterConfig {

  PrinterConfig(const Json::Value v) {


  }
  int Id{0};
  std::string Name;
  int TopMargin;
  int BottomMargin;
  int LeftMargin;
  int RightMargin;
  int PaperName;
  int PrinterName;
};

class PrintedPage {};

#endif // PrintDatabase_H