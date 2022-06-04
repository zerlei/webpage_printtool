#include "printdatabase.h"
#include <QApplication>

PrintDatabase::PrintDatabase() {
  _db = QSqlDatabase::addDatabase("QSQLITE");
  _db.setDatabaseName(QCoreApplication::applicationDirPath() +
                      "/PrinterInfo.db");

  if (_db.open()) {
    _query = std::make_unique<QSqlQuery>(_db);

  } else {
    QApplication::quit();
  }
}

std::tuple<bool, std::string> PrintDatabase::Insert(const Json::Value &ob) {

  try {
    QString queryExit =
        QString(R"(SELECT 1 FROM PrinterConfigInfo where Name ='%1';)")
            .arg(ob["Name"].asCString());
    if (_query->exec(queryExit)) {
      if (_query->next()) {
        return std::make_tuple(false, "不能有重复的名称！");
      }
    }

    QString insertSql =
        QString(R"(
                  INSERT INTO PrinterConfigInfo (
                                  Name,
                                  PrinterName,
                                  TopMargin,
                                  BottomMargin,
                                  LeftMargin,
                                  RightMargin,
                                  Orientation,
                                  PaperName
                              )
                              VALUES (
                              '%1',
                              '%2',
                               %3,
                               %4,
                               %5,
                               %6,
                              '%7',
                              '%8'
                              );

  )")
            .arg(ob["Name"].asCString(), ob["PrinterName"].asCString())
            .arg(ob["TopMargin"].asInt())
            .arg(ob["BottomMargin"].asInt())
            .arg(ob["LeftMargin"].asInt())
            .arg(ob["RightMargin"].asInt())
            .arg(ob["Orientation"].asCString(), ob["PaperName"].asCString());

    if (_query->exec(insertSql)) {
      return std::make_tuple(true, "");
    }

    throw;

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

std::tuple<bool, std::string> PrintDatabase::Del(const int Id) {

  try {
    QString delSql =
        QString(R"(delete from PrinterConfigInfo where Id = %1;)").arg(Id);

    if (_query->exec(delSql)) {
      return std::make_tuple(true, "");
    }

    return std::make_tuple(false, "删除失败！");

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

std::tuple<bool, std::string> PrintDatabase::Update(const Json::Value &ob) {
  try {
    QString updateSql =
        QString(R"(
                update PrinterConfigInfo
                       set Name = '%1',
                       PrinterName = '%2',
                       PaperName = '%3',
                       Orientation = '%4',
                       TopMargin = %5,
                       BottomMargin = %6,
                       LeftMargin = %7,
                       RightMargin = %8
                       where Id = %9)")
            .arg(ob["Name"].asCString(), ob["PrinterName"].asCString(),
                 ob["PaperName"].asCString(), ob["Orientation"].asCString())
            .arg(ob["TopMargin"].asInt())
            .arg(ob["BottomMargin"].asInt())
            .arg(ob["LeftMargin"].asInt())
            .arg(ob["RightMargin"].asInt())
            .arg(ob["Id"].asInt());

    if (_query->exec(updateSql)) {
      return std::make_tuple(true, "");
    } else {
      return std::make_tuple(false, _query->lastError().text().toStdString());
    }

  } catch (...) {
    return std::make_tuple(false, "未知的错误发生了~");
  }
}

const Json::Value PrintDatabase::Query(int Id) {
  try {
    Json::Value v;
    QString querySql;
    if (Id == -1) { //查询全部
      querySql = QString(R"(SELECT * FROM PrinterConfigInfo)");
      if (_query->exec(querySql)) {
        while (_query->next()) {
          Json::Value v1;
          v1["Id"] = _query->value(0).toInt();
          v1["Name"] = _query->value(1).toString().toStdString();
          v1["TopMargin"] = _query->value(2).toInt();
          v1["BottomMargin"] = _query->value(3).toInt();
          v1["LeftMargin"] = _query->value(4).toInt();
          v1["RightMargin"] = _query->value(5).toInt();
          v1["Orientation"] = _query->value(6).toString().toStdString();
          v1["PaperName"] = _query->value(7).toString().toStdString();
          v1["PrinterName"] = _query->value(8).toString().toStdString();
          v.append(v1);
        }

      } else {
        return v;
      }

    } else {
      querySql =
          QString(R"(SELECT * FROM PrinterConfigInfo where Id=%1)").arg(Id);
      if (_query->exec(querySql)) {
        while (_query->next()) {
          Json::Value v1;
          v1["Id"] = _query->value(0).toInt();
          v1["Name"] = _query->value(1).toString().toStdString();
          v1["TopMargin"] = _query->value(2).toInt();
          v1["BottomMargin"] = _query->value(3).toInt();
          v1["LeftMargin"] = _query->value(4).toInt();
          v1["RightMargin"] = _query->value(5).toInt();
          v1["Orientation"] = _query->value(6).toString().toStdString();
          v1["PaperName"] = _query->value(7).toString().toStdString();
          v1["PrinterName"] = _query->value(8).toString().toStdString();
          v.append(v1);
        }

      } else {
        return v;
      }
    }

    return v;

  } catch (...) {
    Json::Value v;
    return v;
  }
}

const Json::Value PrintDatabase::QueryByName(const QString &Name) {
  try {
    Json::Value v;
    QString querySql =
        QString(R"(SELECT * FROM PrinterConfigInfo where Name=%1)").arg(Name);
    if (_query->exec(querySql)) {
      while (_query->next()) {
        Json::Value v1;
        v1["Id"] = _query->value(0).toInt();
        v1["Name"] = _query->value(1).toString().toStdString();
        v1["TopMargin"] = _query->value(2).toInt();
        v1["BottomMargin"] = _query->value(3).toInt();
        v1["LeftMargin"] = _query->value(4).toInt();
        v1["RightMargin"] = _query->value(5).toInt();
        v1["Orientation"] = _query->value(6).toString().toStdString();
        v1["PaperName"] = _query->value(7).toString().toStdString();
        v1["PrinterName"] = _query->value(8).toString().toStdString();
        v.append(v1);
      }
    }
    return v;

  } catch (...) {
    Json::Value v;
    return v;
  }
}
