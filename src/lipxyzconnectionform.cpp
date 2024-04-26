#include "lipxyzconnectionform.h"
#include "ui_lipxyzconnectionform.h"

LIPXYZConnectionForm::LIPXYZConnectionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPXYZConnectionForm),
    mConnection{nullptr}
{
    ui->setupUi(this);
    setWindowTitle("Подключение к XYZ-сервису");
}

LIPXYZConnectionForm::~LIPXYZConnectionForm()
{
    delete ui;
}

LIPXYZConnection *LIPXYZConnectionForm::getXYZConnection()
{
    return mConnection;
}

void LIPXYZConnectionForm::on_buttonBox_accepted()
{
    int minZoom = ui->spinBoxMinZoom->value();
    int maxZoom = ui->spinBoxMaxZoom->value();
    QString connectionName = ui->lineEditConnectionName->text();
    if (connectionName.isEmpty())
        connectionName = "XYZ Тайлы";
    QString url = ui->lineEditURL->text();
    if (minZoom>maxZoom)
    {
        QMessageBox::warning(this, "Ошибка", "Минимальный уровень зума не может быть выше максимального уровня зума");
        return;
    }
    mConnection= new LIPXYZConnection(url, minZoom, maxZoom, connectionName);
}


void LIPXYZConnectionForm::on_buttonBox_rejected()
{
    close();
}

