#ifndef LIPATTRIBUTESTABLEFORM_H
#define LIPATTRIBUTESTABLEFORM_H

#include <QDialog>
#include "vector/lipvectorlayer.h"
#include <QCloseEvent>

namespace Ui {
class LIPAttributesTableForm;
}

class LIPAttributesTableForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPAttributesTableForm(QWidget *parent = nullptr);
    ~LIPAttributesTableForm();
    void setLayer(LIPVectorLayer* layer);

private slots:
    void on_tableWidget_itemSelectionChanged();

private:
    Ui::LIPAttributesTableForm *ui;
    LIPVectorLayer* layer;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // LIPATTRIBUTESTABLEFORM_H
