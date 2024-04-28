#include "liprastertransform.h"

LIPRasterTransform::LIPRasterTransform()
{

}

bool LIPRasterTransform::reproject(LIPRasterLayer *layer, LIPCoordinateSystem *targetCRS, QString fileName)
{
    //OGRSpatialReference *oSRS = targetCRS->getOGRSpatialRef();
    GDALDriverH hDriver;
    GDALDataType eDT;
    GDALDatasetH hDstDS;
    GDALDatasetH hSrcDS;
    // Open the source file.
    hSrcDS = layer->getDataSet();
    // Create output with same datatype as first input band.
    eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS,1));
    // Get output driver (GeoTIFF format)
    hDriver = GDALGetDriverByName( "GTiff" );
    // Get Source coordinate system.
    const char *pszSrcWKT = NULL;
    char* pszDstWKT = NULL;
    pszSrcWKT = GDALGetProjectionRef( hSrcDS );
    qDebug()<< "Src proj:" <<pszSrcWKT;
    // Setup output coordinate system that is UTM 11 WGS84.
    OGRSpatialReference *oSRS = targetCRS->getOGRSpatialRef();
    oSRS->exportToWkt( &pszDstWKT );

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

    QByteArray ba = fileName.toLocal8Bit();
    const char *nameChar = ba.data();
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
    psWarpOptions->pTransformerArg =
            GDALCreateGenImgProjTransformer( hSrcDS,
                                             GDALGetProjectionRef(hSrcDS),
                                             hDstDS,
                                             GDALGetProjectionRef(hDstDS),
                                             FALSE, 0.0, 1 );
    psWarpOptions->pfnTransformer = GDALGenImgProjTransform;

    // Initialize and execute the warp operation.
    GDALWarpOperation oOperation;

    qDebug()<< oOperation.Initialize( psWarpOptions );
    int err = oOperation.ChunkAndWarpImage( 0, 0,
                                            GDALGetRasterXSize( hDstDS ),
                                            GDALGetRasterYSize( hDstDS ) );



    GDALDestroyGenImgProjTransformer( psWarpOptions->pTransformerArg );
    GDALDestroyWarpOptions( psWarpOptions );
    GDALClose( hDstDS );
    //GDALClose( hSrcDS );
    if (err==0)
        return true;
    return false;

}
