#ifndef TABLEDATEEDIT_HPP
#define TABLEDATEEDIT_HPP

#include <QDateTimeEdit>

class TableDateTimeEdit : public QDateTimeEdit {
  Q_OBJECT
  int indexInTable{0};

public:
  TableDateTimeEdit();
  int getIndexInTable() const;
  void setIndexInTable(int value);

private slots:
  void dateTimeChanged(const QDateTime &date);

signals:
  void tableDateTimeEditChanged(int indexInTable, const QDateTime &date);
};

#endif // TABLEDATEEDIT_HPP
