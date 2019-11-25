#include "tablecombobox.hpp"

int TableComboBox::getIndexInTable() const { return indexInTable; }

void TableComboBox::setIndexInTable(int value) { indexInTable = value; }

TableComboBox::TableComboBox() {
  connect(this, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged(int)));
}

void TableComboBox::indexChanged(int val) {
  value = val;
  emit tableComboBoxChanged(indexInTable, value);
}

int TableComboBox::getValue() const { return value; }
