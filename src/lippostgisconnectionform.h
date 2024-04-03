#ifndef LIPPOSTGISCONNECTIONFORM_H
#define LIPPOSTGISCONNECTIONFORM_H

#include <QDialog>
#include "lippostgisprovider.h"
#include "lipwidgetmanager.h"
#include "vector/lipvectortypechecker.h"

namespace Ui {
class LIPPostGisConnectionForm;
}

class LIPPostGisConnectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPPostGisConnectionForm(QWidget *parent = nullptr);
    ~LIPPostGisConnectionForm();
    GDALDataset* returnDataSet();

private slots:
    //void on_pushButtonConnect_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LIPPostGisConnectionForm *ui;
    LIPPostGisProvider *mPGProvider;
};

#endif // LIPPOSTGISCONNECTIONFORM_H
