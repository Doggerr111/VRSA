#ifndef LIPNEWPOINTLAYERFORM_H
#define LIPNEWPOINTLAYERFORM_H

#include <QDialog>

namespace Ui {
class LIPNewPointLayerForm;
}

class LIPNewPointLayerForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPNewPointLayerForm(QWidget *parent = nullptr);
    ~LIPNewPointLayerForm();

private:
    Ui::LIPNewPointLayerForm *ui;
};

#endif // LIPNEWPOINTLAYERFORM_H
