#include "lipmapcalculations.h"
#include "cmath"
#include <QDebug>
LIPMapCalculations::LIPMapCalculations(QObject *parent)
    : QObject{parent}
{

}






void LIPMapCalculations::setDpi( double dpi )
{
    mDpi = dpi;
}


double LIPMapCalculations::calculate( const QRectF &mapExtent, double canvasWidth )  const
{


    double conversionFactor = 0;
    double delta = 0;
    calculateMetrics( mapExtent, delta, conversionFactor );
    qDebug()<< "Extent";
    double screenRealWidth_sm = (static_cast< double >( canvasWidth ) / (qApp->primaryScreen()->physicalDotsPerInch() / 25.40))/10;
    const double scale = ( delta * conversionFactor ) / ( static_cast< double >( canvasWidth ) / mDpi );
    const double scale2 = ( delta * conversionFactor*100 ) /screenRealWidth_sm;
    return scale;

}


void LIPMapCalculations::calculateMetrics( const QRectF &mapExtent, double &delta, double &conversionFactor ) const
{
    QPointF rect_c= mapExtent.center();
    double xMin = rect_c.x()-mapExtent.width()/2.0;
    double xMax=xMin+mapExtent.width();
    delta = xMax - xMin;
    if (LIPProject::getInstance().getProjectCRS()->getOGRSpatialRef()->IsGeographic()) //если географическая ск вычисляем дистанцию по формуле Хаверсина
    {
        conversionFactor = 39.3700787;
        delta = calculateGeographicDistance( mapExtent );
    }
    else //для ПСК в метрах
    {
        conversionFactor=1.0;
    }
    //todo добавить км,

}

double LIPMapCalculations::calculateGeographicDistance( const QRectF &mapExtent ) const
{


    QPointF rect_c= mapExtent.center();
    double xMin = rect_c.x()-mapExtent.width()/2.0;
    double xMax=xMin+mapExtent.width();
    double yMax = rect_c.y()-mapExtent.height()/2.0;
    double yMin = yMax - mapExtent.height();
    qDebug()<<xMin;
    qDebug()<<xMax;
    qDebug()<<yMin;
    qDebug()<<yMax;
    //    xMin=141;
    //    xMax=315;
    //    yMax=113;
    //    yMin=81;

    const double lat = ( yMax + yMin) * 0.5; //среднее значение
    static const double RADS = ( 4.0 * std::atan( 1.0 ) ) / 180.0;
    const double a = std::pow( std::cos( lat * RADS ), 2 );
    const double c = 2.0 * std::atan2( std::sqrt( a ), std::sqrt( 1.0 - a ) );
    static const double RA = 6378000; // [m]
    // Эксцентриситет sqrt(1.0 - rb*rb/(ra*ra)) c rb в 6357000 m.
    static const double E = 0.0810820288;
    const double radius = RA * ( 1.0 - E * E ) /
            std::pow( 1.0 - E * E * std::sin( lat * RADS ) * std::sin( lat * RADS ), 1.5 );
    const double meters = ( xMax - xMin ) / 180.0 * radius * c;


    qDebug()<<mapExtent;
    return meters;
}
