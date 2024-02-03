#ifndef LIPPOSTGISCONNECTIONFORM_H
#define LIPPOSTGISCONNECTIONFORM_H

#include <QDialog>
#include "lippostgisprovider.h"
#include "lipwidgetmanager.h"
#include "lipvectortypechecker.h"

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
    void on_pushButtonConnect_clicked();

private:
    Ui::LIPPostGisConnectionForm *ui;
    LIPPostGisProvider *mPGProvider;
};

#endif // LIPPOSTGISCONNECTIONFORM_H
