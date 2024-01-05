#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QFileDialog"
#include "QMessageBox"
#include "QStandardItem"
#include "QDebug"
#include <optional>
#include "QInputDialog"

#include "coreUtils.h"
#include "createrecorddialog.h"
#include "dataValidation.h"

#define ROW_ENROLLMENTS 0
#define ROW_GRADES 1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->updateMenuState();

    ui->tbl_eg->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tbl_eg->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_eg_contextMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Open_Database_triggered()
{
    QString databasePath = QFileDialog::getOpenFileName(this, "Open Database", "", "Text Files (*.txt)");
    if (databasePath == "")
        return;

    bool success = m_dbController.importDatabaseFile(databasePath.toStdString());

    QMessageBox message;
    if (success)
    {
        message.information(this, "Import Success", "Successfully imported databse from file");
        this->updateMenuState();
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

    // set whether buttons are enabled or not
    bool prevEnabled, nextEnabled;
    m_dbController.getSwitcherActiveStates(prevEnabled, nextEnabled);
    ui->btn_prev->setEnabled(prevEnabled);
    ui->btn_next->setEnabled(nextEnabled);

    // set text content for labels and text containers
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
        tableModel->setData(tableModel->index(ROW_GRADES, i), QString().setNum(record->grades[i], 'g', 6));
    }
}

void MainWindow::updateMenuState()
{
    // if there is a database
    if (m_dbController.hasActiveDatabase())
    {
        this->setEntryControlsEnabled(true);
        ui->action_Close->setEnabled(true);
        ui->action_Save->setEnabled(true);
        ui->btn_addEntry->setEnabled(true);
        ui->lbl_selectedDB->setText(m_dbController.getDatabaseName());
    }
    // if there is no database
    else {
        this->setEntryControlsEnabled(false);
        ui->action_Close->setEnabled(false);
        ui->action_Save->setEnabled(false);
        ui->btn_addEntry->setEnabled(false);
        ui->val_sid->setText("");
        ui->txt_name->setText("");
        ui->txt_phone->setText("");
        ui->tbl_eg->setColumnCount(0);
        ui->lbl_selectedDB->setText("[No DB selected]");
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
    ui->btn_removeEntry->setEnabled(enabled);
    ui->btn_addEG->setEnabled(enabled);
    ui->btn_search->setEnabled(enabled);
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_txt_name_editingFinished()
{
    //data authentication
    const QString& input = ui->txt_name->text();
    if (!DataValidation::wordCountCheck(input, 2, -1))
    {
        DataValidation::showInvalidBoundsMessage(this, 2, -1);
        ui->txt_name->setText(QString::fromStdString(m_dbController.getCurrentRecord()->name));
        return;
    }

    // set record name to text content
    m_dbController.getCurrentRecord()->name = ui->txt_name->text().toStdString();
    m_dbController.setDirty(true);
}


void MainWindow::on_txt_phone_editingFinished()
{
    const QString& input = ui->txt_phone->text();
    if (!DataValidation::wordCountCheck(input, 1, 1))
    {
        DataValidation::showInvalidBoundsMessage(this, 1, 1);
        ui->txt_phone->setText(QString::fromStdString(m_dbController.getCurrentRecord()->phone));
        return;
    }

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
        // don't really like this way of authentication, might change it if i have the time
        if (!DataValidation::wordCountCheck(newText, 1, 1))
        {
            DataValidation::showInvalidBoundsMessage(this, 1, 1);
            ui->tbl_eg->item(row, column)->setText(QString::fromStdString(currentRecord->enrollments[column]));
            return;
        }

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
    this->updateMenuState();
}

void MainWindow::on_btn_addEntry_clicked()
{
    CreateRecordDialog input(
        // callback for when the form is submitted
        [&](uint32_t sid, QString& name, QString& message)
        {
            if (!DataValidation::wordCountCheck(name, 2, -1))
            {
                message = "Name must have at least 2 words";
                return false;
            }

            bool success = m_dbController.createRecord(sid, name);
            if (!success)
            {
                message = QString("Record with SID: ") + QVariant(sid).toString() + " already exists";
                return false;
            }

            this->updateEntryDisplay(m_dbController.getCurrentRecord());
            return true;
        },
    this);

    // show the form
    input.exec();
}

void MainWindow::on_btn_removeEntry_clicked()
{
    // perform confirmation check
    QMessageBox::StandardButton response = QMessageBox::question(this, "Remove entry?", "Are you sure you want to remove this entry?");
    if (response != QMessageBox::StandardButton::Yes)
        return;

    // remove the record and update display
    m_dbController.deleteCurrentRecord();
    this->updateEntryDisplay(m_dbController.getCurrentRecord());
}

void MainWindow::on_eg_contextMenuRequested(QPoint point)
{
    int column = ui->tbl_eg->horizontalHeader()->logicalIndexAt(point);

    // create the menu
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    // create the delete action and bind it to callback
    QAction* deleteAction = new QAction("Delete", this);
    connect(deleteAction, &QAction::triggered, this, [this, column]{ on_egDelete(column); });
    menu->addAction(deleteAction);

    // show the popup where the user clicks
    menu->popup(ui->tbl_eg->horizontalHeader()->viewport()->mapToGlobal(point));
}

void MainWindow::on_egDelete(uint32_t column)
{
    //
    QMessageBox::StandardButton response = QMessageBox::question(this, "Remove?", "Are you sure you want to remove this enrollment and grade?");
    if (response != QMessageBox::StandardButton::Yes)
        return;

    // remove the current column from the record
    Record& record = *m_dbController.getCurrentRecord();

    record.enrollments.erase(record.enrollments.begin() + column);
    record.grades.erase(record.grades.begin() + column);

    this->updateEntryDisplay(&record);
}


void MainWindow::on_btn_addEG_clicked()
{
    Record& record = *m_dbController.getCurrentRecord();

    record.enrollments.push_back(std::string("ENROLLMENT_") + std::to_string(record.enrollments.size() + 1));
    record.grades.push_back(0);

    this->updateEntryDisplay(&record);

    // set focus to the new enrollments cell
    ui->tbl_eg->setFocus();
    ui->tbl_eg->setCurrentCell(ROW_ENROLLMENTS, record.enrollments.size() - 1);
}


void MainWindow::on_btn_search_clicked()
{
    // get sid input
    bool ok;
    int32_t sidInput = QInputDialog::getInt(this, "Search Record", "SID:", QLineEdit::Normal, 0, 2147483647, 1, &ok);

    // return if the user aborted
    if (!ok)
        return;

    // set the record and update display
    Record* record = m_dbController.setEntry(sidInput);
    if (record == nullptr)
    {
        QMessageBox::critical(this, "Not found", "The provided SID does not exist within this database");
        return;
    }

    this->updateEntryDisplay(record);
}

