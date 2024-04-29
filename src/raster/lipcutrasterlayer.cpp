#include "lipcutrasterlayer.h"

LIPCutRasterLayer::LIPCutRasterLayer()
{

}

bool LIPCutRasterLayer::cutRasterByVector(LIPRasterLayer *rLayer, LIPPolygonLayer *vLayer, QString outPath)
{
    if (rLayer == nullptr || vLayer == nullptr || outPath.isEmpty())
        return false;
    QByteArray ba = outPath.toLocal8Bit();
    const char *outputPath = ba.data();
    const char *read_filenamePoly = vLayer->getFileName().toLocal8Bit().data();
    char **argv = nullptr;
    argv = CSLAddString(argv, "-cutline");
    argv = CSLAddString(argv, read_filenamePoly);
    argv = CSLAddString(argv, "-crop_to_cutline");
    argv = CSLAddString(argv, "-multi");
    argv = CSLAddString(argv, "-dstnodata");
    argv = CSLAddString(argv, 0);
    argv = CSLAddString(argv, "-ot");
    argv = CSLAddString(argv, "Uint16");
    argv = CSLAddString(argv, "-wm");
    argv = CSLAddString(argv, "512");
    argv = CSLAddString(argv, "-wo");
    argv = CSLAddString(argv, "NUM_THREADS=8");

    //Input data
    GDALDatasetH inputDs = rLayer->getDataSet();

    int anOverviewList[7] = { 2, 4, 8, 16, 32,64,128 };
    int bUsageError = false;

    //gdalwarp
    GDALWarpAppOptions *opt = GDALWarpAppOptionsNew(argv, nullptr);
    GDALDataset *outputDs = (GDALDataset *)GDALWarp(outputPath, nullptr, 1, &inputDs, opt, &bUsageError);
    GDALWarpAppOptionsFree(opt);
    CSLDestroy(argv);
    if (outputDs)
    {
        outputDs->BuildOverviews("NEAREST", 5, anOverviewList, 0, nullptr, nullptr, nullptr);
        GDALClose(outputDs);
        return true;
    }
    return false;
}
