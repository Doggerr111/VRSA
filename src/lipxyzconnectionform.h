#ifndef LIPXYZCONNECTIONFORM_H
#define LIPXYZCONNECTIONFORM_H

#include <QDialog>
#include <QMessageBox>
#include "lipxyzconnection.h"

namespace Ui {
class LIPXYZConnectionForm;
}

class LIPXYZConnectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPXYZConnectionForm(QWidget *parent = nullptr);
    ~LIPXYZConnectionForm();
    LIPXYZConnection *getXYZConnection();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LIPXYZConnectionForm *ui;
    LIPXYZConnection *mConnection;
};

#endif // LIPXYZCONNECTIONFORM_H
