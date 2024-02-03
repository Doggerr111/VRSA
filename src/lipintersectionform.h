#ifndef LIPINTERSECTIONFORM_H
#define LIPINTERSECTIONFORM_H

#include <QDialog>
#include "lipvectortypechecker.h"
#include <QFileDialog>
namespace Ui {
class LIPIntersectionForm;
}

class LIPIntersectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPIntersectionForm(QWidget *parent = nullptr);
    ~LIPIntersectionForm();

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonFileDialog_clicked();

private:
    Ui::LIPIntersectionForm *ui;
    QString fileName;
};

#endif // LIPINTERSECTIONFORM_H
