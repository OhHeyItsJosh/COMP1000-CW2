#ifndef CREATERECORDDIALOG_H
#define CREATERECORDDIALOG_H

#include <QDialog>
#include "functional"

using CreateCallback = std::function<bool(uint32_t sid, QString& name, QString& in_mesage)>;

namespace Ui {
class CreateRecordDialog;
}

class CreateRecordDialog : public QDialog
{
    Q_OBJECT

public:

public:
    explicit CreateRecordDialog(CreateCallback callback, QWidget *parent = nullptr);
    ~CreateRecordDialog();

private slots:
    void on_controls_accepted();

    void on_controls_rejected();

private:
    Ui::CreateRecordDialog *ui;

    CreateCallback m_onAccept;
};

#endif // CREATERECORDDIALOG_H
