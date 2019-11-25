#ifndef TABLECOMBOBOX_H
#define TABLECOMBOBOX_H

#include <QComboBox>

class TableComboBox : public QComboBox {
  Q_OBJECT

  int indexInTable{0};
  int value{0};

public:
  TableComboBox();
  int getValue() const;
  int getIndexInTable() const;
  void setIndexInTable(int value);

private slots:
  void indexChanged(int val);

signals:
  void tableComboBoxChanged(int indexInTable, int value);
};

#endif // TABLECOMBOBOX_H
