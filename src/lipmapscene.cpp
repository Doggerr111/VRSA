#include "lipmapscene.h"
#include <QDebug>

LIPMapScene::LIPMapScene(QObject *parent)
    : QGraphicsScene{parent},
      isDraging{false},
      isAddingFeaturesToMap{false},
      isPoint{false},
      isLine{false},
      isPolygon{false},
      tempLine{nullptr},
      tempPoly{nullptr}
{

}

void LIPMapScene::startAddingFeatures(LIPVectorLayer *aL)
{
    emit startAdding();
    isAddingFeaturesToMap=true;
    activeLayer = aL;
    //isPolygon=true;
    //return;

    if (activeLayer!=nullptr)
    {
        LIPPointLayer* new_layer=dynamic_cast<LIPPointLayer*>(activeLayer);
        if (new_layer!=nullptr) //если активный слой точечный
        {
            isPoint=true;
            isLine=false;
            isPolygon=false;
            return;

        }

        LIPLineLayer* new_line_layer=dynamic_cast<LIPLineLayer*>(activeLayer);
        if(new_line_layer!=nullptr) //если линия
        {
            isPoint=false;
            isLine=true;
            isPolygon=false;
            return;
        }

        LIPPolygonLayer* new_poly_layer=dynamic_cast<LIPPolygonLayer*>(activeLayer);
        if(new_poly_layer!=nullptr) //если полигон
        {
            isPoint=false;
            isLine=false;
            isPolygon=true;
            return;
        }


    }

}

void LIPMapScene::stopAddingFeatures()
{
    emit stopAdding();
    isAddingFeaturesToMap=true;
    activeLayer = nullptr;
    if (items().contains(tempLine))
        removeItem(tempLine);
    if (items().contains(tempPoly))
        removeItem(tempPoly);
    isPoint=false;
    isLine=false;
    isPolygon=false;
    vectPoints.clear();
   // this->views();


}

void LIPMapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit pos_changed(event->scenePos());
    if (isDraging)
    {
//        QPointF delta=clickPos-event->pos();
//        clickPos=event->pos();
//        //centerOn(sceneRect().center()+delta.toPoint());
//        setSceneRect(sceneRect().translated(-delta));
        emit scene_dragging(clickPos,event->scenePos());
    }
    if (isLine && tempLine!=nullptr)
    {
        vectPoints.append(event->scenePos());
        tempLine->setPoints(LIPVectorConvertor::QPointsFtoLIPPoints(vectPoints));
        update(tempLine->bRect);
        vectPoints.removeLast();

    }
    if (isPolygon && tempPoly!=nullptr)
    {
        vectPoints.append(event->scenePos());
        tempPoly->setPolygon(vectPoints);
        vectPoints.removeLast();

    }
}

void LIPMapScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{

}

void LIPMapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickPos=event->scenePos();
    if (event->button()==Qt::RightButton) //если ПКМ
    {
        if (isAddingFeaturesToMap) //если добавляем обьекты
        {
            if (isPoint) //если добавляем точки
            {
                //добавляем точку на сцену
                //LIPPointGraphicsItem *point = new LIPPointGraphicsItem;
                //LIPPoint *p= new LIPPoint;
                //p->setX(clickPos.x());
                //p->setY(clickPos.y());
                //point->setPoint(p);
                //addItem(point);
                vectPoints.append(clickPos);

                LIPNewAttrFeatureForm *form = new LIPNewAttrFeatureForm;
                form->setAttributeNames(activeLayer->getAttributeNames());
                form->exec();
                //values to attrs

                dynamic_cast<LIPPointLayer*>(activeLayer)->addFeature(vectPoints,
                    LIPVectorLayer::stringValToAttrs(form->getNames(), form->getValues(), activeLayer->getAttributeTypes()));
                activeLayer->update();
                vectPoints.clear();
                return;
            }
            else if (isLine) //если добавляем линии
            {

                tempLine->setPoints(LIPVectorConvertor::QPointsFtoLIPPoints(vectPoints));
                //tempLine->set(QPolygonF(vectPoints));
                if (vectPoints.size()>1) //линия только с 2+ точками
                {
                    LIPNewAttrFeatureForm *form = new LIPNewAttrFeatureForm;
                    form->setAttributeNames(activeLayer->getAttributeNames());
                    form->exec();
                    dynamic_cast<LIPLineLayer*>(activeLayer)->addFeature(vectPoints,
                        LIPVectorLayer::stringValToAttrs(form->getNames(), form->getValues(), activeLayer->getAttributeTypes()));
                    activeLayer->update();
                    vectPoints.clear();
                    //drawVectorLayer(activeLayer);
                    tempLine->setPoints(LIPVectorConvertor::QPointsFtoLIPPoints(vectPoints));
                    removeItem(tempLine);
                    tempLine=nullptr;
                    return;
                }
                LIPWidgetManager::getInstance().showMessage("Для создания линии необходимо добавить минимум 2 точки",
                                                            1000, messageStatus::Error);
                return;

            }
            else if (isPolygon) //если добавляем полигон
            {
                tempPoly->setPolygon(QPolygonF(vectPoints));
                if (vectPoints.size()>2) //3+ точки для полигона
                {
                LIPNewAttrFeatureForm *form = new LIPNewAttrFeatureForm;
                form->setAttributeNames(activeLayer->getAttributeNames());
                form->exec();
                dynamic_cast<LIPPolygonLayer*>(activeLayer)->addFeature(vectPoints,
                    LIPVectorLayer::stringValToAttrs(form->getNames(), form->getValues(), activeLayer->getAttributeTypes()));
                vectPoints.clear();
                //drawVectorLayer(activeLayer);
                activeLayer->update();
                removeItem(tempPoly);
                tempPoly=nullptr;
                return;
                }
                LIPWidgetManager::getInstance().showMessage("Для создания полигона необходимо добавить минимум 3 точки",
                                                            1000, messageStatus::Error);
                return;

            }
        }
    }
    //QGraphicsScene::mousePressEvent(event);


    if (isAddingFeaturesToMap && event->button()!=Qt::MiddleButton) //если добавляем обьекты
    {
        if (isPoint) //если добавляем точки
        {


            //TODO
            //здесь нужно открыть таблицу атрибутов (окно с добавлением
            //атрибута слоя, после проверки добавить уже объект (поинт графикс итем удалить)
        }
        else if (isLine) //если добавляем линии
        {
            if (tempLine==nullptr)
            {
                tempLine = new LIPLineGraphicsItem;
                QPen pen;
                pen.setWidthF(1);
                tempLine->setPen(pen);
            }
            vectPoints.append(clickPos);
            if (vectPoints.size()>1)
                tempLine->setPoints(LIPVectorConvertor::QPointsFtoLIPPoints(vectPoints));
            //addItem(tempLine); //добавляем временный элемент с линией
            if (!items().contains(tempLine))
                addItem(tempLine);
            //(при движении мыши линия будет меняться, при нажатии ПКМ должна завершаться)
            //аналогично с полигоном
        }
        else if (isPolygon) //если добавляем полигон
        {
            if (tempPoly==nullptr)
            {
                tempPoly = new QGraphicsPolygonItem;
                QPen pen;
                pen.setWidthF(0);
                tempPoly->setPen(pen);
            }
            vectPoints.append(clickPos);
            tempPoly->setPolygon(QPolygonF(vectPoints));
            if (!items().contains(tempPoly))
                addItem(tempPoly);
        }
    }
    else
    {
        isDraging=true;

    }
    event->accept();
}

void LIPMapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    isDraging=false;
}

void LIPMapScene::drawVectorLayer(LIPVectorLayer* layer)
{
    if (layer!=nullptr)
    {
        LIPPointLayer* new_layer=dynamic_cast<LIPPointLayer*>(layer);
        if (new_layer!=nullptr)
        {
            new_layer->setMapFeatures(); //для создания графических айтемов
            for (int i=0; i<new_layer->returnMapFeatures().size(); i++)
            {
                if (!items().contains(new_layer->returnMapFeatures().at(i)))
                    addItem(new_layer->returnMapFeatures().at(i));
            }
//            QVector<LIPPoint*> vect = new_layer->returnCords();
//            for (int i=0; i<vect.size(); i++)
//            {
//                //addEllipse(vect.at(i)->x(), vect.at(i)->y(),0.1,0.1);

//                LIPPointGraphicsItem *el = new LIPPointGraphicsItem;
//                LIPPoint point;
//                point.setX(vect.at(i)->x());
//                point.setY(vect.at(i)->y());
//                el->setPoint(point);
//                addItem(el);
//                    //addItem(el);
//            }
        }
        else
        {
            LIPLineLayer* new_line_layer=dynamic_cast<LIPLineLayer*>(layer);
            if(new_line_layer!=nullptr)
            {
                new_line_layer->setMapFeatures(); //для создания графических айтемов
                for (int i=0; i<new_line_layer->returnMapFeatures().size(); i++)
                {
                    if (!items().contains(new_line_layer->returnMapFeatures().at(i)))
                        addItem(new_line_layer->returnMapFeatures().at(i));
                }

            }
            else
            {
                LIPPolygonLayer* new_poly_layer=dynamic_cast<LIPPolygonLayer*>(layer);
                if(new_poly_layer!=nullptr)
                {
                    new_poly_layer->setMapFeatures(); //для создания графических айтемов
                    for (int i=0; i<new_poly_layer->returnMapFeatures().size(); i++)
                    {
                        if (!items().contains(new_poly_layer->returnMapFeatures().at(i)))
                            addItem(new_poly_layer->returnMapFeatures().at(i));
                    }

                }
            }

    }
    }
}

void LIPMapScene::redrawVectorLayer(LIPVectorLayer *)
{

}

void LIPMapScene::updateVectorLayer()
{
    LIPVectorLayer *layer = qobject_cast<LIPVectorLayer *>(sender());
    if (layer!=nullptr)
    {
        LIPPointLayer* pointLayer=dynamic_cast<LIPPointLayer*>(layer);
        if (pointLayer!=nullptr)
        {
            pointLayer->setMapFeatures(); //для создания графических айтемов
            for (int i=0; i<pointLayer->returnMapFeatures().size(); i++)
            {
                if (!items().contains(pointLayer->returnMapFeatures().at(i)))
                    addItem(pointLayer->returnMapFeatures().at(i));
            }

        }
        else
        {
            LIPLineLayer* lineLayer=dynamic_cast<LIPLineLayer*>(layer);
            if(lineLayer!=nullptr)
            {
                //TODO проходим по всем айтемам очень долго, нужно сделать флаги (возможно) для объектов, которые показывают
                //добавлены ли объекты или нет
                lineLayer->setMapFeatures(); //для создания графических айтемов
                for (int i=0; i<lineLayer->returnMapFeatures().size(); i++)
                {
                    if (!items().contains(lineLayer->returnMapFeatures().at(i)))
                        addItem(lineLayer->returnMapFeatures().at(i));
                }

            }
            else
            {
                LIPPolygonLayer* polyLayer=dynamic_cast<LIPPolygonLayer*>(layer);
                if(polyLayer!=nullptr)
                {
                    polyLayer->setMapFeatures(); //для создания графических айтемов
                    for (int i=0; i<polyLayer->returnMapFeatures().size(); i++)
                    {
                        if (!items().contains(polyLayer->returnMapFeatures().at(i)))
                            addItem(polyLayer->returnMapFeatures().at(i));
                    }

                }
            }

    }
    }

}

void LIPMapScene::addPointFeature()
{
    //delete this;
}


void LIPMapScene::keyPressEvent(QKeyEvent *event)
{
    if (isAddingFeaturesToMap && event->key()==Qt::Key_Escape)
    {
        if (isPoint) //если добавляем точки
        {
            vectPoints.clear();
            return;
        }
        else if (isLine) //если добавляем линии
        {
            vectPoints.clear();
            removeItem(tempLine);
            tempLine=nullptr;
            return;

        }
        else if (isPolygon) //если добавляем полигон
        {
            vectPoints.clear();
            removeItem(tempPoly);
            tempPoly=nullptr;
            return;

        }
    }
}
