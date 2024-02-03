#include "lippostgisconnectionform.h"
#include "ui_lippostgisconnectionform.h"
#include <QMessageBox>
LIPPostGisConnectionForm::LIPPostGisConnectionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPPostGisConnectionForm)
{
    ui->setupUi(this);
    mPGProvider = new LIPPostGisProvider;

    //TESTING
    ui->lineEditDBName->setText("LIPServer");
    ui->lineEditDBHost->setText("127.0.0.1");
    ui->lineEditPassword->setText("1234");
    ui->lineEditDBPort->setText("5432");
    ui->lineEditUserName->setText("postgres");
}

LIPPostGisConnectionForm::~LIPPostGisConnectionForm()
{
    delete ui;
}

GDALDataset *LIPPostGisConnectionForm::returnDataSet()
{
    if (mPGProvider->isConnected() && mPGProvider->isPostGIS())
        return mPGProvider->readData();
    else
    {
        return nullptr;
    }
}

void LIPPostGisConnectionForm::on_pushButtonConnect_clicked()
{
    mPGProvider->connectionName=ui->lineEditName->text();
    mPGProvider->databaseName=ui->lineEditDBName->text();
    mPGProvider->host=ui->lineEditDBHost->text();
    mPGProvider->password=ui->lineEditPassword->text();
    mPGProvider->port=ui->lineEditDBPort->text();
    mPGProvider->userName=ui->lineEditUserName->text();

    mPGProvider->connect();
    if (mPGProvider->isConnected())
    {
        if (mPGProvider->isPostGIS())
        {
            LIPWidgetManager::getInstance().showMessage(tr("Успешно подключились к ")+mPGProvider->databaseName, 5000, messageStatus::Success);
        }
        else
        {
            LIPWidgetManager::getInstance().showMessage(tr("Успешно подключились к ")+mPGProvider->databaseName, 5000, messageStatus::Success);
        }
    }
}

