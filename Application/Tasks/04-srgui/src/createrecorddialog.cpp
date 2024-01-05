#include "createrecorddialog.h"
#include "ui_createrecorddialog.h"

#include "QMessageBox"

CreateRecordDialog::CreateRecordDialog(CreateCallback callback, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateRecordDialog), m_onAccept(callback)
{
    ui->setupUi(this);
}

CreateRecordDialog::~CreateRecordDialog()
{
    delete ui;
}

void CreateRecordDialog::on_controls_accepted()
{
    QString sid_string = ui->txt_sid->text();
    QString name = ui->txt_name->text();

    bool sidParseOk;
    uint32_t sid = sid_string.toUInt(&sidParseOk);

    if (!sidParseOk)
    {
        QMessageBox::critical(this, "Validation Error", "SID needs to be a number");
        return;
    }

    QString message;
    bool success = m_onAccept(sid, name, message);

    if (!success)
    {
        QMessageBox::critical(this, "Validation Error", message);
        return;
    }
}


void CreateRecordDialog::on_controls_rejected()
{
    this->close();
}

