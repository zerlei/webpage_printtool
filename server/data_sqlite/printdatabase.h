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

class PrintDatabase : QObject {
  Q_OBJECT
public:
  PrintDatabase();

  std::tuple<bool, std::string> Insert(const Json::Value &ob);
  std::tuple<bool, std::string> Del(const int Id);
  std::tuple<bool, std::string> Update(const Json::Value &ob);
  const Json::Value Query(int Id = -1);
  const Json::Value QueryByName(const QString &Name);

  const Json::Value GetPrintedPage(int size,int page);

  void InsertPrintedPage(const Json::Value& ob);

private:
  QSqlDatabase _db;
  std::unique_ptr<QSqlQuery> _query;
};

#endif // PrintDatabase_H
