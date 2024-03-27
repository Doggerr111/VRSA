#include "lipcutrasterlayer.h"

LIPCutRasterLayer::LIPCutRasterLayer()
{

}

bool LIPCutRasterLayer::cutRasterByVector(LIPRasterLayer *rLayer, LIPPolygonLayer *vLayer)
{





//    GDALAllRegister();

//       // Открытие растрового файла
//       GDALDataset *poDataset = (GDALDataset *)GDALOpen("input_raster.tif", GA_ReadOnly);
//       if (poDataset == NULL) {
//           // Обработка ошибки
//       }

//       // Открытие контурной линии
//       GDALDataset *poCutlineDS = (GDALDataset *)GDALOpenEx("cutline.shp", GDAL_OF_VECTOR, NULL, NULL, NULL);
//       if (poCutlineDS == NULL) {
//           // Обработка ошибки
//       }

//       // Задание опций обрезки (crop to cutline)
//       const char *pszSource = poDataset->GetDescription();
//       char *pszCutline = const_cast<char*>(poCutlineDS->GetDescription());
//       const char *pszDest = "output_cropped.tif";

//       char *options[] = {
//           "-crop_to_cutline",
//           pszCutline,
//           "-of", "GTiff",
//           "-ot", "Byte",
//           "-co", "COMPRESS=LZW",
//           NULL
//       };

//       // Применение операции обрезки
//       GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
//       psWarpOptions->nBandCount = poDataset->GetRasterCount();
//       psWarpOptions->papszWarpOptions = options;
//       psWarpOptions->hSrcDS = poDataset;
//       psWarpOptions->hDstDS = NULL;

//       GDALDataset *poCroppedDS = (GDALDataset *)GDALAutoCreateWarpedVRT(psWarpOptions, NULL, nullptr);

//       // Сохранение результата
//       GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
//       poDriver->CreateCopy(pszDest, poCroppedDS, FALSE, NULL, NULL, NULL);

//       // Освобождение ресурсов
//       GDALClose(poDataset);
//       GDALClose(poCutlineDS);

//       GDALDestroyDriverManager();

//       return 0;
//   }











    GDALDataset *poDataset = rLayer->getDataSet();


    //OGRSpatialReference *oSRS = targetCRS->getOGRSpatialRef();
    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff"); // Формат результата (может быть другой)
    GDALDataset *poReprojectedDS;

    poReprojectedDS = poDriver->Create("result.tif", poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), poDataset->GetRasterCount(), GDT_Float32, NULL);
    GDALDriverH hDriver;
    GDALDataType eDT;
    GDALDatasetH hDstDS;
    GDALDatasetH hSrcDS;

    GDALDataset *poMaskDS = vLayer->getDataSet();

    // Open the source file.
    hSrcDS = rLayer->getDataSet();
    CPLAssert( hSrcDS != NULL );

    // Create output with same datatype as first input band.
    eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS,1));

    // Get output driver (GeoTIFF format)
    hDriver = GDALGetDriverByName( "GTiff" );
    CPLAssert( hDriver != NULL );

    // Get Source coordinate system.
    const char *pszSrcWKT = NULL;
    char* pszDstWKT = NULL;
    pszSrcWKT = GDALGetProjectionRef( hSrcDS );
    CPLAssert( pszSrcWKT != NULL && strlen(pszSrcWKT) > 0 );

    // Setup output coordinate system that is UTM 11 WGS84.
    OGRSpatialReference oSRS;
    //oSRS.SetUTM( 11, TRUE );
    oSRS.SetWellKnownGeogCS( "WGS84" );
    oSRS.exportToWkt( &pszDstWKT );

    // Create a transformer that maps from source pixel/line coordinates
    // to destination georeferenced coordinates (not destination
    // pixel line).  We do that by omitting the destination dataset
    // handle (setting it to NULL).
    void *hTransformArg;
    hTransformArg =
        GDALCreateGenImgProjTransformer( hSrcDS, pszSrcWKT, NULL, pszDstWKT,
                                         FALSE, 0, 1 );
    CPLAssert( hTransformArg != NULL );

    // Get approximate output georeferenced bounds and resolution for file.
    double adfDstGeoTransform[6];
    int nPixels=0, nLines=0;
    CPLErr eErr;
    eErr = GDALSuggestedWarpOutput( hSrcDS,
                                    GDALGenImgProjTransform, hTransformArg,
                                    adfDstGeoTransform, &nPixels, &nLines );
    CPLAssert( eErr == CE_None );
    GDALDestroyGenImgProjTransformer( hTransformArg );

    // Create the output file.
    qDebug()<<adfDstGeoTransform;

    const char *nameChar = "filename";
    hDstDS = GDALCreate( hDriver, nameChar, nPixels, nLines,
                         GDALGetRasterCount(hSrcDS), eDT, NULL );

    CPLAssert( hDstDS != NULL );

    // Write out the projection definition.
    GDALSetProjection( hDstDS, pszDstWKT );
    GDALSetGeoTransform( hDstDS, adfDstGeoTransform );

    // Copy the color table, if required.
    GDALColorTableH hCT;
    hCT = GDALGetRasterColorTable( GDALGetRasterBand(hSrcDS,1) );
    if( hCT != NULL )
        GDALSetRasterColorTable( GDALGetRasterBand(hDstDS,1), hCT );

    //    char *crsWKT;
//    oSRS->exportToWkt(&crsWKT);
//    poReprojectedDS->SetProjection(crsWKT); // Установка проекции нового растра
//    GDALReprojectImage(poDataset, NULL, poReprojectedDS, NULL, GRA_Bilinear, 0, 0, NULL, NULL, NULL);
//    // Используйте нужные параметры интерполяции

    int nBands = GDALGetRasterCount(hSrcDS);

    GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
       psWarpOptions->hSrcDS = hSrcDS;
       psWarpOptions->hDstDS = hDstDS;
       psWarpOptions->nBandCount = nBands;

       psWarpOptions->panSrcBands =
           (int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
       //psWarpOptions->panSrcBands[0] = 1;
       psWarpOptions->panDstBands =
           (int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
       //psWarpOptions->panDstBands[0] = 1;
       psWarpOptions->pfnProgress = GDALTermProgress;
       for (int i = 0; i < nBands; i++) {
           psWarpOptions->panSrcBands[i] = i + 1;
           psWarpOptions->panDstBands[i] = i + 1;
       }
       // Establish reprojection transformer.
//       psWarpOptions->pTransformerArg =
//           GDALCreateGenImgProjTransformer( hSrcDS,
//                                           GDALGetProjectionRef(hSrcDS),
//                                           hDstDS,
//                                           GDALGetProjectionRef(hDstDS),
//                                           FALSE, 0.0, 1 );

       //psWarpOptions->papszWarpOptions = CSLSetNameValue(psWarpOptions->papszWarpOptions, "DST_METHOD", "NO_GEOTRANSFORM");

       char **papszTransformerOptions = nullptr;
       papszTransformerOptions = CSLSetNameValue(
                      papszTransformerOptions, "DST_METHOD", "NO_GEOTRANSFORM");
       psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer2(poDataset, poMaskDS, papszTransformerOptions);
       psWarpOptions->pfnTransformer = GDALGenImgProjTransform;


       vLayer->getOGRLayer()->ResetReading();
       //qDebug()<<vLayer->getOGRLayer()->GetNextFeature()->GetGeometryRef();
       psWarpOptions->hCutline = vLayer->getOGRLayer()->GetNextFeature()->GetGeometryRef();

       // Initialize and execute the warp operation.
       GDALWarpOperation oOperation;
       oOperation.Initialize( psWarpOptions );
       //oOperation.WarpRegion(0, 0, poDataset->GetRasterXSize(), poDataset->GetRasterYSize());
       oOperation.ChunkAndWarpImage(0, 0, poDataset->GetRasterXSize(), poDataset->GetRasterYSize());

       GDALDestroyGenImgProjTransformer( psWarpOptions->pTransformerArg );
       GDALDestroyWarpOptions( psWarpOptions );
       GDALClose( hDstDS );
       GDALClose( hSrcDS );
       return 0;















//    GDALDataset *poDataset = rLayer->getDataSet();
//    GDALDataset *poMaskDS = vLayer->getDataSet();

//    //todo for для каждого канала???
//    GDALRasterBand *poSrcBand = poDataset->GetRasterBand(1);

//    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

//    const char *options[] = { "INTERLEAVE=PIXEL", NULL };
//    GDALDataset *poMask = poDriver->Create("output.tif", poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), 1, GDT_Byte, options);

//    GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
//    psWarpOptions->hSrcDS = poDataset;
//    psWarpOptions->hDstDS = poMask;
//    psWarpOptions->nBandCount = 1;
//    psWarpOptions->panSrcBands = (int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount);
//    psWarpOptions->panSrcBands[0] = 1;
//    psWarpOptions->panDstBands = (int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount);
//    psWarpOptions->panDstBands[0] = 1;
//    psWarpOptions->pTransformerArg = GDALCreateGenImgProjTransformer2(poDataset, poMaskDS, NULL);
//    psWarpOptions->pfnTransformer = GDALGenImgProjTransform;

//    const char *pszWKT = NULL;
//   // psWarpOptions->papszWarpOptions = CSLSetNameValue(psWarpOptions->papszWarpOptions, "CUTLINE", "mask.shp");
//    psWarpOptions->hCutline = poMaskDS;
//    psWarpOptions->papszWarpOptions = CSLSetNameValue(psWarpOptions->papszWarpOptions, "DST_METHOD", "NO_GEOTRANSFORM");
//    psWarpOptions->eWorkingDataType = GDT_Byte;

//    GDALWarpOperation oOperation;

//    oOperation.Initialize(psWarpOptions);
//    oOperation.WarpRegion(0, 0, poDataset->GetRasterXSize(), poDataset->GetRasterYSize());

//    GDALClose(poMask);
//    GDALClose(poMaskDS);
//    GDALClose(poDataset);
//    GDALDestroyWarpOptions(psWarpOptions);

//    GDALDumpOpenDatasets(stderr);

//    return true;
}
