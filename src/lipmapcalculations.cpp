#include "lipmapcalculations.h"
#include "cmath"
LIPMapCalculations::LIPMapCalculations(QObject *parent)
    : QObject{parent}
{

}



//#include <cmath>
//#include "qgslogger.h"
//#include "qgsrectangle.h"
//#include "qgsscalecalculator.h"


void LIPMapCalculations::setDpi( double dpi )
{
  mDpi = dpi;
}
//double QgsScaleCalculator::dpi() const
//{
//  return mDpi;
//}

//void QgsScaleCalculator::setMapUnits( QgsUnitTypes::DistanceUnit mapUnits )
//{
//  QgsDebugMsgLevel( QStringLiteral( "Map units set to %1" ).arg( QString::number( mapUnits ) ), 3 );
//  mMapUnits = mapUnits;
//}

//QgsUnitTypes::DistanceUnit QgsScaleCalculator::mapUnits() const
//{
//  QgsDebugMsgLevel( QStringLiteral( "Map units returned as %1" ).arg( QString::number( mMapUnits ) ), 4 );
//  return mMapUnits;
//}

double LIPMapCalculations::calculate( const QRectF &mapExtent, double canvasWidth )  const
{
//  if ( qgsDoubleNear( canvasWidth, 0. ) || qgsDoubleNear( mDpi, 0.0 ) )
//  {
//    QgsDebugMsg( QStringLiteral( "Can't calculate scale from the input values" ) );
//    return 0;
//  }

  double conversionFactor = 0;
  double delta = 0;
  calculateMetrics( mapExtent, delta, conversionFactor );

  const double scale = ( delta * conversionFactor ) / ( static_cast< double >( canvasWidth ) / mDpi );
  return scale;

}

//QSizeF QgsScaleCalculator::calculateImageSize( const QgsRectangle &mapExtent, double scale )  const
//{
//  if ( qgsDoubleNear( scale, 0.0 ) || qgsDoubleNear( mDpi, 0.0 ) )
//  {
//    QgsDebugMsg( QStringLiteral( "Can't calculate image size from the input values" ) );
//    return QSizeF();
//  }
//  double conversionFactor = 0;
//  double delta = 0;

//  calculateMetrics( mapExtent, delta, conversionFactor );
//  const double imageWidth = ( delta * conversionFactor ) / ( static_cast< double >( scale ) ) * mDpi;
//  const double deltaHeight = ( mapExtent.yMaximum() - mapExtent.yMinimum() ) * delta / ( mapExtent.xMaximum() - mapExtent.xMinimum() );
//  const double imageHeight = ( deltaHeight * conversionFactor ) / ( static_cast< double >( scale ) ) * mDpi;

//  QgsDebugMsgLevel( QStringLiteral( "imageWidth = %1 imageHeight = %2 conversionFactor = %3" )
//                    .arg( imageWidth ).arg( imageHeight ).arg( conversionFactor ), 4 );

//  return QSizeF( imageWidth, imageHeight );
//}

void LIPMapCalculations::calculateMetrics( const QRectF &mapExtent, double &delta, double &conversionFactor ) const
{
    QPointF rect_c= mapExtent.center();
    double xMin = rect_c.x()-mapExtent.width()/2.0;
    double xMax=xMin+mapExtent.width();
 //  double xMin = mapExtent.x();
//  double xMax = xMin + mapExtent.width();
    delta = xMax - xMin;
//    xMax=315;
//    xMin=141;

//  switch ( mMapUnits )
//  {
//    case QgsUnitTypes::DistanceMeters:
//      // convert meters to inches
      //conversionFactor = 39.3700787;
//      break;
//    case QgsUnitTypes::DistanceFeet:
//      conversionFactor = 12.0;
//      break;
//    case QgsUnitTypes::DistanceNauticalMiles:
//      // convert nautical miles to inches
//      conversionFactor = 72913.4;
//      break;
//    default:
//    case QgsUnitTypes::DistanceDegrees:
//      // degrees require conversion to meters first
     conversionFactor = 39.3700787;
     delta = calculateGeographicDistance( mapExtent );
//      break;
//  }
}

double LIPMapCalculations::calculateGeographicDistance( const QRectF &mapExtent ) const
{
  // need to calculate the x distance in meters
  // We'll use the middle latitude for the calculation
  // Note this is an approximation (although very close) but calculating scale
  // for geographic data over large extents is quasi-meaningless

  // The distance between two points on a sphere can be estimated
  // using the Haversine formula. This gives the shortest distance
  // between two points on the sphere. However, what we're after is
  // the distance from the left of the given extent and the right of
  // it. This is not necessarily the shortest distance between two
  // points on a sphere.
  //
  // The code below uses the Haversine formula, but with some changes
  // to cope with the above problem, and also to deal with the extent
  // possibly extending beyond +/-180 degrees:
  //
  // - Use the Halversine formula to calculate the distance from -90 to
  //   +90 degrees at the mean latitude.
  // - Scale this distance by the number of degrees between
  //   mapExtent.xMinimum() and mapExtent.xMaximum();
  // - For a slight improvemnt, allow for the ellipsoid shape of earth.


  // For a longitude change of 180 degrees

    QPointF rect_c= mapExtent.center();
    double xMin = rect_c.x()-mapExtent.width()/2.0;
    double xMax=xMin+mapExtent.width();
    double yMin = rect_c.y()-mapExtent.height()/2.0;
    double yMax = yMin + mapExtent.height();

//    xMin=141;
//    xMax=315;
//    yMax=113;
//    yMin=81;

  const double lat = ( yMax + yMin) * 0.5; //среднее значение
  static const double RADS = ( 4.0 * std::atan( 1.0 ) ) / 180.0;
  const double a = std::pow( std::cos( lat * RADS ), 2 );
  const double c = 2.0 * std::atan2( std::sqrt( a ), std::sqrt( 1.0 - a ) );
  static const double RA = 6378000; // [m]
  // The eccentricity. This comes from sqrt(1.0 - rb*rb/(ra*ra)) with rb set
  // to 6357000 m.
  static const double E = 0.0810820288;
  const double radius = RA * ( 1.0 - E * E ) /
                        std::pow( 1.0 - E * E * std::sin( lat * RADS ) * std::sin( lat * RADS ), 1.5 );
  const double meters = ( xMax - xMin ) / 180.0 * radius * c;

  //QgsDebugMsgLevel( "Distance across map extent (m): " + QString::number( meters ), 4 );

  return meters;
}
