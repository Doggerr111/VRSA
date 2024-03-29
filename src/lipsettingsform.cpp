#include "lipsettingsform.h"
#include "ui_lipsettingsform.h"
#include "mainwindow.h"

LIPSettingsForm::LIPSettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPSettingsForm),
    isMapColChanged{false}
{
    ui->setupUi(this);
    // Установите режим ресайза для таблицы на основе содержимого ячеек
    ui->tableWidgetToolBarSettings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidgetToolBarSettings->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    // Уберите названия колонок таблицы
    ui->tableWidgetToolBarSettings->horizontalHeader()->hide();
    ui->tableWidgetToolBarSettings->verticalHeader()->hide();

    // Измените стиль номеров строк
    ui->tableWidgetToolBarSettings->verticalHeader()->setStyleSheet("background-color: lightgray; color: black;");
    ui->tableWidgetToolBarSettings->setStyleSheet("QTableWidget::item { border: none; }");
    ui->tableWidgetToolBarSettings->setStyleSheet("QTableView { border: 0px; gridline-color: white; }");

    switch (ui->tabWidget->currentIndex())
    {
        case 0:
        {
            setActionsTable();
            break;
        }
        case 1:
        {
            ui->lineEditVectorDir->setText(LIPProject::getInstance().getVectorDataFolder());
            ui->lineEditRasterDir->setText(LIPProject::getInstance().getRasterDataFolder());
        }
        case 2:
        {
            ui->pushButtonColor->setStyleSheet("background-color:" + LIPProject::getInstance().getMapCanvasColor().name() + ";"
                                               " border:none;");
            ui->checkBoxAntiAliasing->setChecked(LIPProject::getInstance().isAntiAliasingOn());
        }
    }


}

LIPSettingsForm::~LIPSettingsForm()
{
    delete ui;
}

void LIPSettingsForm::on_pushButtonToolBarSettings_clicked()
{

}


void LIPSettingsForm::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
        case 0:
        {
            setActionsTable();
        }
        case 1:
        {
            ui->lineEditVectorDir->setText(LIPProject::getInstance().getVectorDataFolder());
            ui->lineEditRasterDir->setText(LIPProject::getInstance().getRasterDataFolder());
        }
        case 2:
        {
            ui->pushButtonColor->setStyleSheet("background-color:" + LIPProject::getInstance().getMapCanvasColor().name() + ";"
                                               " border:none;");
            ui->checkBoxAntiAliasing->setChecked(LIPProject::getInstance().isAntiAliasingOn());
        }
    }


}

void LIPSettingsForm::setActionsTable()
{
    ui->tableWidgetToolBarSettings->clear();
    ui->tableWidgetToolBarSettings->setRowCount(0);
    MainWindow* mW = LIPWidgetManager::getInstance().getMainWindow();
    QList<QAction *> allActions = mW->findChildren<QAction *>();
    foreach (QAction *action, allActions)
    {
        QMenu *menu = qobject_cast<QMenu*>(action->parentWidget());
        if (!menu)
        {

            int currentIndex = ui->tableWidgetToolBarSettings->rowCount();
            ui->tableWidgetToolBarSettings->insertRow(currentIndex);
            QLabel *iconLabel = new QLabel;
            iconLabel->setPixmap(action->icon().pixmap(16,16));
            iconLabel->setAlignment(Qt::AlignCenter);
            ui->tableWidgetToolBarSettings->setCellWidget(currentIndex, 0, iconLabel);

            QTableWidgetItem *textItem = new QTableWidgetItem(action->text());
            ui->tableWidgetToolBarSettings->setItem(currentIndex, 1, textItem);

            QCheckBox *checkBox = new QCheckBox();
            if (LIPProject::getInstance().isActionAdded(action))
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);
            ui->tableWidgetToolBarSettings->setCellWidget(currentIndex, 2, checkBox);


            connect(checkBox, &QCheckBox::stateChanged, this, [action, mW](int state)
            {
                if (state == Qt::Checked)
                {
                    foreach (QWidget* child, mW->findChildren<QWidget*>())
                    {
                        if (child->objectName() == "frameMainToolBar")
                        {
                            QPushButton* button = new QPushButton(action->icon(), "");
                            button->setStyleSheet("QPushButton { border:none; } QPushButton:pressed { background-color: gray; "
                                                  "border: 1px solid } QPushButton:hover { border: 1px solid;"
                                                      "border-color: #333; /* Цвет рамки при наведении */  }");
                            button->setIcon(action->icon());
                            button->setMinimumSize(QSize(50, 50));
                            button->setIconSize(QSize(45, 45));
                            connect(button, &QPushButton::clicked, action, &QAction::triggered);
                            child->layout()->addWidget(button);
                            action->setProperty("associatedButton", QVariant::fromValue<QWidget*>(button));
                            LIPProject::getInstance().addAction(action);
                        }
                    }
                }
                else if (state == Qt::Unchecked)
                {
                    LIPProject::getInstance().deleteAction(action);
                }
            }
            );

        }

    }
    ui->tableWidgetToolBarSettings->resizeColumnsToContents();
    ui->tableWidgetToolBarSettings->resizeRowsToContents();
}


void LIPSettingsForm::on_pushButtonVectorDir_clicked()
{
    ui->lineEditVectorDir->setText(QFileDialog::getExistingDirectory(nullptr, "Выберите путь для работы с векторными файлами",
                                                           QDir::currentPath()));

}


void LIPSettingsForm::on_pushButtonRasterDir_clicked()
{
    ui->lineEditRasterDir->setText(QFileDialog::getExistingDirectory(nullptr, "Выберите путь для работы с векторными файлами",
                                                           QDir::currentPath()));
}


void LIPSettingsForm::on_pushButtonDirSave_clicked()
{
    QString vectorDir = ui->lineEditVectorDir->text();
    QString rasterDir = ui->lineEditRasterDir->text();
    LIPProject::getInstance().setVectorDataFolder(vectorDir);
    LIPProject::getInstance().setRasterDataFolder(rasterDir);
}


void LIPSettingsForm::on_pushButtonMapSettingSave_clicked()
{
    QColor canvasColor = ui->pushButtonMapColor->getColor();
    bool state = ui->checkBoxAntiAliasing->isChecked();
    if (isMapColChanged)
        LIPProject::getInstance().setMapCanvasColor(canvasColor);
    LIPProject::getInstance().setAntiAliasing(state);

}


void LIPSettingsForm::on_pushButtonMapColor_clicked()
{
    ui->pushButtonColor->setStyleSheet("background-color:" + ui->pushButtonMapColor->getColor().name() + ";"
                                       " border:none;");
    isMapColChanged=true;
}

