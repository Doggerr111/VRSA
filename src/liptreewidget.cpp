#include "liptreewidget.h"
#include <QDebug>

LIPTreeWidget::LIPTreeWidget(QWidget* parent)
    :   QTreeWidget(parent)
{

}


void LIPTreeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
//    QTreeWidgetItem *item = itemAt(event->pos());
//            if (item != nullptr) {
//                item->setFlags(item->flags() | Qt::ItemIsDragEnabled);
//            }
}

void LIPTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTreeWidget::mouseMoveEvent(event);
//    if (event->buttons() == Qt::LeftButton) {
//               QTreeWidgetItem *item = currentItem();
//               if (item != nullptr && item->flags() & Qt::ItemIsDragEnabled) {
//                   QMimeData *mimeData = new QMimeData;
//                   mimeData->setText(item->text(0));
//                   QDrag *drag = new QDrag(this);
//                   drag->setMimeData(mimeData);
//                   drag->exec(Qt::MoveAction);
//               }
//           }
}

void LIPTreeWidget::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);
    emit itemDropped();
    //qDebug()<<event->mimeData()->imageData();
//    if (event->mimeData()->hasText()) {
//                //QTreeWidgetItem *item = new QTreeWidgetItem({event->mimeData()->imageData(), event->mimeData()->text(), ""});
//               // currentItem()->addChild(item);
//                event->accept();
//            } else {
//                event->ignore();
//            }
}
