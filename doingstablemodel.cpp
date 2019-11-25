#include "doingstablemodel.hpp"

#include <QDateTime>
#include <QStringList>

enum COLUMN_TYPE {
  CT_PRIORITIES = 0,
  CT_TASKS,
  CT_TAGS,
  CT_DATETIME,
  CT_COUNT
};

struct DoingsTableModel::Impl {
  QVector<int> priorities;
  QVector<QString> tasks;
  QVector<QString> tags;
  QVector<QString> datetime;
};

DoingsTableModel::DoingsTableModel(QObject *parent)
    : QAbstractTableModel(parent), impl_(new Impl()) {}

DoingsTableModel::~DoingsTableModel() = default;

QVariant DoingsTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case CT_PRIORITIES:
      return QString::fromUtf8("Приоритет");
    case CT_TASKS:
      return QString::fromUtf8("Задача");
    case CT_TAGS:
      return QString::fromUtf8("Теги");
    case CT_DATETIME:
      return QString::fromUtf8("Дата/время");
    default:
      return QString("---");
    }
  }

  return QVariant();
}

int DoingsTableModel::columnCount(const QModelIndex & /*parent*/) const {
  return CT_COUNT;
}

int DoingsTableModel::rowCount(const QModelIndex & /*parent*/) const {
  return impl_->priorities.size();
}

Qt::ItemFlags DoingsTableModel::flags(const QModelIndex &index) const {
  Q_UNUSED(index);
  // if (index.column() == CT_PRIORITY)
  return Qt::ItemIsEnabled | Qt::ItemIsEditable;

  // return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DoingsTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() >= rowCount() ||
      index.column() >= columnCount())
    return QVariant();

  switch (role) {
  case Qt::EditRole:
  case Qt::DisplayRole: {
    if (index.column() == CT_PRIORITIES) {
      return QString::number(impl_->priorities.at(index.row()));
    }
    if (index.column() == CT_TASKS) {
      return impl_->tasks.at(index.row());
    }
    if (index.column() == CT_TAGS) {
      return impl_->tags.at(index.row());
    }
    if (index.column() == CT_DATETIME) {
      return impl_->datetime.at(index.row());
    }
    break;
  }
  }

  return QVariant();
}

bool DoingsTableModel::setData(const QModelIndex &idx, const QVariant &value,
                               int role) {
  if (!idx.isValid() || idx.row() >= rowCount() ||
      idx.column() >= columnCount())
    return false;

  if (role == Qt::EditRole) {
    if (idx.column() == CT_PRIORITIES) {
      bool ok;
      int priority = value.toString().toInt(&ok);
      if (ok)
        impl_->priorities[idx.row()] = priority;
    }
    if (idx.column() == CT_TASKS) {
      impl_->tasks[idx.row()] = value.toString();
    }
    if (idx.column() == CT_TAGS) {
      impl_->tags[idx.row()] = value.toString();
    }
    if (idx.column() == CT_DATETIME) {
      impl_->datetime[idx.row()] = value.toString();
    }

    emit dataChanged(index(idx.row(), 0), index(idx.row(), columnCount()-1));

    return true;
  }

  return false;
}

void DoingsTableModel::setPriority(int row, int val) {
  impl_->priorities[row] = val;
}

void DoingsTableModel::setDateTime(int row, const QDateTime &val) {
  impl_->datetime[row] = val.toString();
}

void DoingsTableModel::setData(const QVector<int> &priorities,
                               const QVector<QString> &tasks,
                               const QVector<QString> &tags,
                               const QVector<QString> &datetime) {
  // Поиск минимального размера массива из четырёх размеров,
  // чтобы цикл добавления шёл по реально существующим данным, иначе
  // IndexOfBoundsException.
  int size_l = 0;
  int size_r = 0;
  int size = 0;

  if (priorities.size() >= tasks.size()) {
    size_l = tasks.size();
  } else {
    size_l = priorities.size();
  }

  if (tags.size() >= datetime.size()) {
    size_r = datetime.size();
  } else {
    size_r = tags.size();
  }

  if (size_l >= size_r) {
    size = size_r;
  } else {
    size = size_l;
  }

  clear_();

  beginInsertRows(QModelIndex(), 0, size - 1);

  for (int i = 0; i < size; i++) {
    impl_->priorities.push_back(priorities.at(i));
    impl_->tasks.push_back(tasks.at(i));
    impl_->tags.push_back(tags.at(i));
    impl_->datetime.push_back(datetime.at(i));
  }

  endInsertRows();
}

void DoingsTableModel::clear_() {
  beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

  impl_->priorities.clear();
  impl_->tasks.clear();
  impl_->tags.clear();
  impl_->datetime.clear();

  endRemoveRows();
}

void DoingsTableModel::addRow() {
  beginInsertRows(QModelIndex(), impl_->priorities.size() /* - 1*/,
                  impl_->priorities.size() /* - 1*/);

  impl_->priorities.push_back(0);
  impl_->tasks.push_back(0);
  impl_->tags.push_back(0);
  impl_->datetime.push_back(0);

  endInsertRows();
}

bool DoingsTableModel::deleteRow(const QModelIndex &idx) {
  if (!idx.isValid() || idx.row() >= rowCount() ||
      idx.column() >= columnCount())
    return false;

  beginRemoveRows(QModelIndex(), idx.row(), idx.row());

  impl_->priorities.remove(idx.row());
  impl_->tasks.remove(idx.row());
  impl_->tags.remove(idx.row());
  impl_->datetime.remove(idx.row());

  endRemoveRows();

  return true;
}

void DoingsTableModel::sortModel() {
  // Сортировка массива priority методом Шелла (упорядочиваем по глубинам)
  int size = impl_->priorities.size();
  int step = size / 2;
  int tmp, i, j;
  int *ind = new int[size];

  for (i = 0; i < size; i++)
    ind[i] = i;

  while (step > 0) {
    for (i = 0; i < (size - step); i++) {
      j = i;
      while (j >= 0 &&
             impl_->priorities[ind[j]] > impl_->priorities[ind[j + step]]) {
        tmp = ind[j];
        ind[j] = ind[j + step];
        ind[j + step] = tmp;
        j--;
      }
    }
    step = step / 2;
  }

  QVector<int> priorities;
  QVector<QString> tasks;
  QVector<QString> tags;
  QVector<QString> datetime;

  for (i = 0; i < size; i++) {
    priorities.push_back(impl_->priorities[ind[i]]);
    tasks.push_back(impl_->tasks[ind[i]]);
    tags.push_back(impl_->tags[ind[i]]);
    datetime.push_back(impl_->datetime[ind[i]]);
  }

  delete[] ind;

  clear_(); // Отчистка impl_->zMeters и impl_->speedSounds, в т. ч. таблицы
  setData(priorities, tasks, tags, datetime); // перезапись
}

void DoingsTableModel::ExportInSettings(QSettings &settings) {
  settings.clear(); // чистит только значения
  // Они будут по каждой ячейке, потому что всё выравняли через while в
  // DoingsTableModel::data()

  settings.beginGroup("priorities");
  for (int i = 0; i < impl_->priorities.size(); i++) {
    settings.setValue(QString::fromLocal8Bit("%1").arg(i),
                      impl_->priorities[i]);
  }
  settings.endGroup();
  settings.beginGroup("tasks");
  for (int i = 0; i < impl_->tasks.size(); i++) {
    settings.setValue(QString::fromLocal8Bit("%1").arg(i), impl_->tasks[i]);
  }
  settings.endGroup();
  settings.beginGroup("tags");
  for (int i = 0; i < impl_->tags.size(); i++) {
    settings.setValue(QString::fromLocal8Bit("%1").arg(i), impl_->tags[i]);
  }
  settings.endGroup();
  settings.beginGroup("datetime");
  for (int i = 0; i < impl_->datetime.size(); i++) {
    settings.setValue(QString::fromLocal8Bit("%1").arg(i), impl_->datetime[i]);
  }
}

void DoingsTableModel::ImportInSettings(QSettings &settings) {
  // Заполняем модель для QTableView из общего конфига программы

  QStringList keys;

  int val{0};
  QString str;

  settings.beginGroup("priorities");
  keys = settings.allKeys();
  for (QString key : keys) {
    val = qvariant_cast<int>(settings.value(key, -1000));
    // Выделяем элементов количеством равным номеру ключа(столбца в таблице)
    // заранее
    for (int i = impl_->priorities.size(); i <= key.toInt(); i++) {
      impl_->priorities.push_back(0);
    }
    impl_->priorities.replace(key.toInt(), val);
  }
  settings.endGroup();

  settings.beginGroup("tasks");
  keys = settings.allKeys();
  for (QString key : keys) {
    str = qvariant_cast<QString>(settings.value(key, ""));
    // Выделяем элементов количеством равным номеру ключа(столбца в таблице)
    // заранее
    for (int i = impl_->tasks.size(); i <= key.toInt(); i++) {
      impl_->tasks.push_back(0);
    }
    impl_->tasks.replace(key.toInt(), str);
  }
  settings.endGroup();

  settings.beginGroup("tags");
  keys = settings.allKeys();
  for (QString key : keys) {
    str = qvariant_cast<QString>(settings.value(key, ""));
    // Выделяем элементов количеством равным номеру ключа(столбца в таблице)
    // заранее
    for (int i = impl_->tags.size(); i <= key.toInt(); i++) {
      impl_->tags.push_back(0);
    }
    impl_->tags.replace(key.toInt(), str);
  }
  settings.endGroup();

  settings.beginGroup("datetime");
  keys = settings.allKeys();
  for (QString key : keys) {
    str = qvariant_cast<QString>(settings.value(key, ""));
    // Выделяем элементов количеством равным номеру ключа(столбца в таблице)
    // заранее
    for (int i = impl_->datetime.size(); i <= key.toInt(); i++) {
      impl_->datetime.push_back(0);
    }
    impl_->datetime.replace(key.toInt(), str);
  }
  settings.endGroup();
}

QVector<int> DoingsTableModel::getPriorities() const {
  return impl_->priorities;
}

QVector<QString> DoingsTableModel::getTasks() const { return impl_->tasks; }

QVector<QString> DoingsTableModel::getTags() const { return impl_->tags; }

QVector<QString> DoingsTableModel::getDateTime() const {
  return impl_->datetime;
}
