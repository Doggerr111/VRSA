#ifndef LIPVORONOIDIAGRAMFORM_H
#define LIPVORONOIDIAGRAMFORM_H

#include <QDialog>
#include "lipvectortypechecker.h"
#include "liptriangulationgeos.h"
#include <QFileDialog>
namespace Ui {
class LIPVoronoiDiagramForm;
}

class LIPVoronoiDiagramForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVoronoiDiagramForm(QWidget *parent = nullptr);
    ~LIPVoronoiDiagramForm();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonPath_clicked();

private:
    Ui::LIPVoronoiDiagramForm *ui;
    QString fileName;
    LIPVectorLayer* outputLayer;
};

#endif // LIPVORONOIDIAGRAMFORM_H
