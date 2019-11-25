#include "tabledatetimeedit.hpp"

TableDateTimeEdit::TableDateTimeEdit() {
  connect(this, SIGNAL(dateTimeChanged(QDateTime)),
          SLOT(dateTimeChanged(QDateTime)));
}

int TableDateTimeEdit::getIndexInTable() const { return indexInTable; }

void TableDateTimeEdit::setIndexInTable(int value) { indexInTable = value; }

void TableDateTimeEdit::dateTimeChanged(const QDateTime &dateTime) {
  emit tableDateTimeEditChanged(indexInTable, dateTime);
}
