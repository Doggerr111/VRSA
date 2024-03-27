#ifndef LIPVECTORREPROJECTFORM_H
#define LIPVECTORREPROJECTFORM_H

#include <QDialog>
#include <QFileDialog>
#include "liplayercreator.h"
namespace Ui {
class LIPVectorReprojectForm;
}

class LIPVectorReprojectForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVectorReprojectForm(QWidget *parent = nullptr);
    ~LIPVectorReprojectForm();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonFileDialog_clicked();

private:
    Ui::LIPVectorReprojectForm *ui;
    QString fileName;
};

#endif // LIPVECTORREPROJECTFORM_H
