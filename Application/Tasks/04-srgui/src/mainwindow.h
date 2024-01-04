#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "coreModels.h"
#include "databaseController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Open_Database_triggered();

    void on_btn_next_clicked();

    void on_btn_prev_clicked();

    void on_test_btn_clicked();

    void on_actionExit_triggered();

    void on_txt_name_editingFinished();

    void on_txt_phone_editingFinished();

    void on_tbl_eg_cellChanged(int row, int column);

    void on_action_Save_triggered();

    void on_action_Close_triggered();

private:
    Ui::MainWindow *ui;
    DatabaseController m_dbController;
    std::vector<bool> m_isActiveUpdate;

private:
    void updateEntryDisplay(Record* record);
    void setEntryControlsEnabled(bool enabled);
    void eg_setInactiveUpdate(int row, int column);
};
#endif // MAINWINDOW_H
