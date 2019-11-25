#ifndef DOINGSTABLEMODEL_H
#define DOINGSTABLEMODEL_H

#include <QAbstractItemView>
#include <QAbstractTableModel>
#include <QSettings>
#include <QVector>
#include <memory>

class DoingsTableModel : public QAbstractTableModel {
  Q_OBJECT

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
  void clear_();

public:
  explicit DoingsTableModel(QObject *parent = nullptr);
  DoingsTableModel(const DoingsTableModel &obj);
  ~DoingsTableModel();

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role) const;

  bool setData(const QModelIndex &idx, const QVariant &value, int role);
  void setData(const QVector<int> &priorities, const QVector<QString> &tasks,
               const QVector<QString> &tags, const QVector<QString> &datetime);
  void setPriority(int row, int val);

  void addRow();
  bool deleteRow(const QModelIndex &idx);
  void sortModel(); // Упорядочить по глубинам
  void ExportInSettings(QSettings &settings);
  void ImportInSettings(QSettings &settings);

  QVector<int> getPriorities() const;
  QVector<QString> getTasks() const;
  QVector<QString> getTags() const;
  QVector<QString> getDateTime() const;

signals:
};

#endif // DOINGSTABLEMODEL_H
