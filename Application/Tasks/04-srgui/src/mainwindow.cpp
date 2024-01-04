#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QFileDialog"
#include "QMessageBox"
#include "QStandardItem"
#include "QDebug"
#include <optional>

#include "coreUtils.h"

#define ROW_ENROLLMENTS 0
#define ROW_GRADES 1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setEntryControlsEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Open_Database_triggered()
{
    QString databasePath = QFileDialog::getOpenFileName(this, "Open Database", "", "Text Files (*.txt)");
    bool success = m_dbController.importDatabaseFile(databasePath.toStdString());

    QMessageBox message;
    if (success)
    {
        message.information(this, "Import Success", "Successfully imported databse from file");
        ui->lbl_selectedDB->setText(databasePath.split("/").last());
        this->updateEntryDisplay(m_dbController.getCurrentRecord());
    }
    else
        message.critical(this, "Import failed", "Database could not be imported");
}

void MainWindow::updateEntryDisplay(Record* record)
{
    if (record == nullptr)
    {
        this->setEntryControlsEnabled(false);
        return;
    }
    else {
        this->setEntryControlsEnabled(true);
    }

    ui->val_sid->setText(QVariant(record->sid).toString());
    ui->txt_name->setText(QString::fromStdString(record->name));
    ui->txt_phone->setText(QString::fromStdString((record->phone)));

    // remove existing columns from table
    ui->tbl_eg->setColumnCount(0);

    // re-create update map
    m_isActiveUpdate = std::vector<bool>(2 * record->enrollments.size(), true);

    // add enrollments and grades
    auto tableModel = ui->tbl_eg->model();
    for (uint32_t i = 0; i < record->enrollments.size(); i++)
    {
        ui->tbl_eg->insertColumn(i);

        // set inactive updates so that onChanged signal is blocked
        this->eg_setInactiveUpdate(ROW_ENROLLMENTS, i);
        this->eg_setInactiveUpdate(ROW_GRADES, i);

        tableModel->setData(tableModel->index(ROW_ENROLLMENTS, i), QString::fromStdString(record->enrollments[i]));
        tableModel->setData(tableModel->index(ROW_GRADES, i), QVariant(record->grades[i]).toString());
    }
}


void MainWindow::on_btn_prev_clicked()
{
    Record* record = m_dbController.previousEntry();
    if (record != nullptr)
        this->updateEntryDisplay(record);
}


void MainWindow::on_btn_next_clicked()
{
    Record* record = m_dbController.nextEntry();
    if (record != nullptr)
        this->updateEntryDisplay(record);
}

void MainWindow::setEntryControlsEnabled(bool enabled)
{
    ui->btn_next->setEnabled(enabled);
    ui->btn_prev->setEnabled(enabled);
    ui->txt_name->setEnabled(enabled);
    ui->txt_phone->setEnabled(enabled);
    ui->tbl_eg->setEnabled(enabled);
}


void MainWindow::on_test_btn_clicked()
{
    auto model = ui->tbl_eg->model();
    model->setData(model->index(0, 0), "Test");
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_txt_name_editingFinished()
{
    // set record name to text content
    m_dbController.getCurrentRecord()->name = ui->txt_name->text().toStdString();
    m_dbController.setDirty(true);
}


void MainWindow::on_txt_phone_editingFinished()
{
    // set record phone to text content
    m_dbController.getCurrentRecord()->phone = ui->txt_phone->text().toStdString();
    m_dbController.setDirty(true);
}


void MainWindow::on_tbl_eg_cellChanged(int row, int column)
{
    // check whether the current update is an active update
    uint32_t cellIndex = row * ui->tbl_eg->columnCount() + column;
    if (!m_isActiveUpdate[cellIndex])
    {
        m_isActiveUpdate[cellIndex] = true;
        return;
    }

    qDebug() << "Update for table cell: " << row << ", " << column;

    // update enrollment or grade to new text content
    QString newText = ui->tbl_eg->item(row, column)->text();
    Record* currentRecord = m_dbController.getCurrentRecord();

    if (currentRecord == nullptr)
        return;

    switch(row)
    {
    case ROW_ENROLLMENTS:
        currentRecord->enrollments[column] = newText.toStdString();
        m_dbController.setDirty(true);
        break;

    case ROW_GRADES:
        bool ok;
        float gradeParse = newText.toFloat(&ok);

        if (!ok)
        {
            qDebug() << "Invalid input!";
            this->eg_setInactiveUpdate(row, column);
            ui->tbl_eg->item(row, column)->setText(QVariant(currentRecord->grades[column]).toString());
            break;
        }

        currentRecord->grades[column] = gradeParse;
        m_dbController.setDirty(true);
        break;
    }
}

void MainWindow::eg_setInactiveUpdate(int row, int column)
{
    uint32_t cellIndex = row * ui->tbl_eg->columnCount() + column;
    // qDebug() << "Setting Cell with index " << cellIndex << " inactive. Length is " << m_isActiveUpdate.size();
    m_isActiveUpdate[cellIndex] = false;
}


void MainWindow::on_action_Save_triggered()
{
    bool success = m_dbController.saveDatabaseFile();

    QMessageBox message;
    if (success)
    {
        message.information(this, "", "Successfully saved database");
        m_dbController.setDirty(false);
    }
    else
        message.information(this, "", "Failed to save database");

    message.show();
}


void MainWindow::on_action_Close_triggered()
{
    if (m_dbController.isDirty())
    {
        qDebug() << "Unsaved changes";
        // TODO: implement unsaved changes check
    }

    m_dbController.closeDatabase();

    ui->val_sid->setText("");
    ui->txt_name->setText("");
    ui->txt_phone->setText("");
    ui->tbl_eg->setColumnCount(0);
    ui->lbl_selectedDB->setText("[No DB selected]");

    this->setEntryControlsEnabled(false);
}

