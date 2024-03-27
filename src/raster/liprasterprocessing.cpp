#include "liprasterprocessing.h"

LIPRasterProcessing::LIPRasterProcessing()
{

}

void LIPRasterProcessing::createContours(LIPRasterLayer *rasterLayer, int bandIndex, double interval, double baseValue, QString outputFileName)
{

    GDALDataset *poDS = rasterLayer->getDataSet();
    if (!poDS)
        return;
    GDALRasterBand *poSrcBand = poDS->GetRasterBand(bandIndex);
    if (!poSrcBand)
        return;
    const OGRSpatialReference* ref = poDS->GetSpatialRef();

    QByteArray bArray = outputFileName.toLocal8Bit();
    const char *pszDstFilename = bArray.data();
    const char *pszDriverName = "ESRI Shapefile";
    GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
    auto ds = poDriver->Create(pszDstFilename, 0, 0, 0, GDT_Unknown, NULL);
    auto layer = ds->CreateLayer("output", const_cast<OGRSpatialReference*>(ref), wkbLineString, NULL);


    GDALContourGenerate(poSrcBand, interval, baseValue, 0, nullptr, 1.0, 0.0, layer, -1, -1, nullptr, nullptr);

    GDALClose(ds);





}
