#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "doingstablemodel.hpp"
#include "tablecombobox.hpp"
#include "tabledatetimeedit.hpp"

#include <QComboBox>
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void on_btnDeleteSelectedTask_clicked();
  void on_btnNewTask_clicked();
  void on_btnSortTasks_clicked();
  void onTableComboBoxChanged(int indexInTable, int value);
  void onTableDateTimeEditChanged(int indexInTable, const QDateTime &dateTime);

private:
  Ui::MainWindow *ui;
  QSettings settings{"tasks.ini", QSettings::IniFormat};
  DoingsTableModel mDoingsTableModel{};

  // ComboBox'ы в QTableView
  QVector<TableComboBox *> mBoxes;

  // DateTimeEdit'ы в QTableView
  QVector<TableDateTimeEdit *> mDateTimeEdits;

  void initializeWidgetsInTableView();
};

#endif // MAINWINDOW_H
