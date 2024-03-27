#ifndef LIPVECTORASSIGNPROJECTFORM_H
#define LIPVECTORASSIGNPROJECTFORM_H

#include <QDialog>

namespace Ui {
class LIPVectorAssignProjectForm;
}

class LIPVectorAssignProjectForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVectorAssignProjectForm(QWidget *parent = nullptr);
    ~LIPVectorAssignProjectForm();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::LIPVectorAssignProjectForm *ui;
};

#endif // LIPVECTORASSIGNPROJECTFORM_H
