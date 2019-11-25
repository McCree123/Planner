#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVariant>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  mDoingsTableModel.ImportInSettings(settings);

  ui->tblTasks->setModel(&mDoingsTableModel);

  initializeWidgetsInTableView();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event)
  mDoingsTableModel.ExportInSettings(settings);
}

void MainWindow::on_btnNewTask_clicked() {
  mDoingsTableModel.addRow();
  int ind = mDoingsTableModel.rowCount() - 1;

  TableComboBox *tblComboBox = new TableComboBox();
  tblComboBox->setIndexInTable(ind);

  TableDateTimeEdit *tblDateTimeEdit = new TableDateTimeEdit();
  tblDateTimeEdit->setIndexInTable(ind);

  // Инициализация TableComboBox
  tblComboBox->insertItem(0, QString::fromLocal8Bit("Низкий"));
  tblComboBox->insertItem(1, QString::fromLocal8Bit("Средний"));
  tblComboBox->insertItem(2, QString::fromLocal8Bit("Высокий"));
  tblComboBox->insertItem(3, QString::fromLocal8Bit("Горит!!!"));

  tblComboBox->setCurrentIndex(1); // По умолчанию добавляем средний
  mDoingsTableModel.setPriority(ind, 1); // запоминаем приоритет в модели

  // Инициализация TableDateTimeEdit
  tblDateTimeEdit->setCalendarPopup(true);
  tblDateTimeEdit->setDateTime(QDateTime::currentDateTime());

  mDoingsTableModel.setDateTime(ind, QDateTime::currentDateTime());

  mBoxes.push_back(tblComboBox);
  mDateTimeEdits.push_back(tblDateTimeEdit);

  connect(mBoxes[ind], SIGNAL(tableComboBoxChanged(int, int)),
          SLOT(onTableComboBoxChanged(int, int)));

  connect(mDateTimeEdits[ind], SIGNAL(tableDateTimeEditChanged(int, QDateTime)),
          SLOT(onTableDateTimeEditChanged(int, QDateTime)));

  ui->tblTasks->setIndexWidget(mDoingsTableModel.index(ind, 0), mBoxes[ind]);
  ui->tblTasks->setIndexWidget(mDoingsTableModel.index(ind, 3),
                               mDateTimeEdits[ind]);
}

void MainWindow::on_btnDeleteSelectedTask_clicked() {
  mDoingsTableModel.deleteRow(ui->tblTasks->selectionModel()->currentIndex());

  int curInd = ui->tblTasks->selectionModel()->currentIndex().row();
  mBoxes.remove(curInd);
  mDateTimeEdits.remove(curInd);
}

void MainWindow::on_btnSortTasks_clicked() {
  mDoingsTableModel.sortModel();
  initializeWidgetsInTableView();
}

void MainWindow::onTableComboBoxChanged(int indexInTable, int value) {
  mDoingsTableModel.setPriority(indexInTable, value);
}

void MainWindow::onTableDateTimeEditChanged(int indexInTable,
                                            const QDateTime &dateTime) {
  mDoingsTableModel.setDateTime(indexInTable, dateTime);
}

void MainWindow::initializeWidgetsInTableView() {
  // Удаляем все элементы в таблице и по новой...
  mBoxes.clear();
  mDateTimeEdits.clear();

  QVector<int> prior = mDoingsTableModel.getPriorities();
  QVector<QString> strDateTimes = mDoingsTableModel.getDateTime();

  // сформируем ComboBox'ы в QTableView
  for (int i = 0; i < mDoingsTableModel.rowCount(); i++) {
    // mBoxes.push_back(TableComboBox()); // No - copy is disabled
    mBoxes.push_back(new TableComboBox());
    mBoxes[i]->insertItem(0, QString::fromLocal8Bit("Низкий"));
    mBoxes[i]->insertItem(1, QString::fromLocal8Bit("Средний"));
    mBoxes[i]->insertItem(2, QString::fromLocal8Bit("Высокий"));
    mBoxes[i]->insertItem(3, QString::fromLocal8Bit("Горит!!!"));

    mBoxes[i]->setCurrentIndex(prior[i]);
    mBoxes[i]->setIndexInTable(i);

    connect(mBoxes[i], SIGNAL(tableComboBoxChanged(int, int)),
            SLOT(onTableComboBoxChanged(int, int)));

    ui->tblTasks->setIndexWidget(mDoingsTableModel.index(i, 0), mBoxes[i]);
  }
  // сформируем DateTimeEdit'ы в QTableView
  for (int i = 0; i < mDoingsTableModel.rowCount(); i++) {
    mDateTimeEdits.push_back(new TableDateTimeEdit());
    mDateTimeEdits[i]->setCalendarPopup(true);
    mDateTimeEdits[i]->setDateTime(QDateTime::fromString(strDateTimes[i]));
    mDateTimeEdits[i]->setIndexInTable(i);

    connect(mDateTimeEdits[i], SIGNAL(tableDateTimeEditChanged(int, QDateTime)),
            SLOT(onTableDateTimeEditChanged(int, QDateTime)));

    ui->tblTasks->setIndexWidget(mDoingsTableModel.index(i, 3),
                                 mDateTimeEdits[i]);
  }
}
