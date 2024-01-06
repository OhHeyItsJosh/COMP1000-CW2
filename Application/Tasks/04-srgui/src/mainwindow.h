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
    void on_eg_contextMenuRequested(QPoint point);

    void on_egDelete(uint32_t column);

    void on_action_Open_Database_triggered();

    void on_btn_next_clicked();

    void on_btn_prev_clicked();

    void on_actionExit_triggered();

    void on_txt_name_editingFinished();

    void on_txt_phone_editingFinished();

    void on_tbl_eg_cellChanged(int row, int column);

    void on_action_Save_triggered();

    void on_action_Close_triggered();

    void on_btn_addEntry_clicked();

    void on_btn_removeEntry_clicked();

    void on_btn_addEG_clicked();

    void on_btn_search_clicked();

    void on_action_New_triggered();

    void on_action_CreateTestDB_triggered();

private:
    Ui::MainWindow *ui;
    DatabaseController m_dbController;
    std::vector<bool> m_isActiveUpdate;

private:
    void clearEntryDisplay();
    void updateEntryDisplay(Record* record);
    void updateMenuState();
    void setEntryControlsEnabled(bool enabled);
    void eg_setInactiveUpdate(int row, int column);
    bool checkUnsavedChanges();
};
#endif // MAINWINDOW_H
