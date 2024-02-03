#include "lipvectorclipper.h"

LIPVectorClipper::LIPVectorClipper(LIPVectorLayer* clippedLayer, LIPVectorLayer* polyLayer)
{

}

OGRLayer* LIPVectorClipper::clip(LIPVectorLayer *layer, LIPVectorLayer *polyLayer)
{
    if (layer==nullptr || polyLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Работа инструмента невозможна, так как заданы некорректные слои", 1000,
                                              messageStatus::Error);
        return nullptr;
    }
    LIPPolygonLayer *newPolyLayer = dynamic_cast<LIPPolygonLayer*>(polyLayer);
    if (newPolyLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Работа инструмента невозможна, так как cлой, по которому производится обрезка не является"
                                              "полигональным", 2000, messageStatus::Error);
        return nullptr;
    }
    OGRLayer* clipOGRLayer = polyLayer->getOGRLayer();
    OGRLayer* inputLayer = layer->getOGRLayer();
    if (clipOGRLayer==nullptr || inputLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Работа инструмента невозможна, так как заданы некорректные слои", 1000,
                                              messageStatus::Error);
        return nullptr;
    }



    LIPLayerCreator *newLayer = new LIPLayerCreator(LIPGeometryType::LIPPoint, "/home/doger/Documents/GepPortal/testCutting.shp",
                                                    "outputLayer", dynamic_cast<LIPCoordinateSystem*>(layer->getOGRLayer()->GetSpatialRef()));
    OGRLayer* outputLayer=newLayer->returnOGRLayer();

    inputLayer->Intersection(clipOGRLayer,outputLayer);
    inputLayer->SyncToDisk();


    // Проход по всем полигонам из слоя обрезки
//    OGRFeature* clipFeature;
//    clipOGRLayer->ResetReading();
//    while ((clipFeature = clipOGRLayer->GetNextFeature()) != nullptr)
//    {
//        // Получение границы полигона
//        OGRGeometry* clipGeometry = clipFeature->GetGeometryRef();

//        // Обрезка исходного слоя по границе полигона
//        outputLayer->SetSpatialFilter(clipGeometry);

//        // Копирование фич из исходного слоя в результирующий слой
//        OGRFeature* sourceFeature;
//        while ((sourceFeature = inputLayer->GetNextFeature()) != nullptr)
//        {
//            OGRFeature* outputFeature = new OGRFeature(outputLayer->GetLayerDefn());
//            outputFeature->SetFrom(sourceFeature);
//            outputLayer->CreateFeature(outputFeature);
//            OGRFeature::DestroyFeature(outputFeature);
//        }

//        // Сброс фильтра исходного слоя
//        inputLayer->SetSpatialFilter(nullptr);

//        OGRFeature::DestroyFeature(clipFeature);
//    }

    return outputLayer;
}
