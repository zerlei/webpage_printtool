#ifndef PrintDatabase_H
#define PrintDatabase_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <json/value.h>
#include <memory>
#include <string>
#include <tuple>
#include "base.h"
#include <vector>

class PrintDatabase : QObject {
  Q_OBJECT
public:
  PrintDatabase();

  std::tuple<bool, std::string> printerConfigInsert(const PrinterConfig &pc_);
  std::tuple<bool, std::string> printerConfigDel(const int Id_);
  std::tuple<bool, std::string> printerConfigUpdate(const PrinterConfig &pc_);
  const std::vector<PrinterConfig>&&  printerConfigQueryById(int Id_ = -1);
  const std::vector<PrinterConfig>&& printerConfigQueryByName(const QString &Name_);


  const std::vector<PrintedPage>&& printedPageQuery(int page_size_,int page_index_);
  bool printedPageInsert(const PrintedPage& pp_);

private:
  QSqlDatabase _db;
  std::unique_ptr<QSqlQuery> _query;
};

#endif // PrintDatabase_H
