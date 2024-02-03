#ifndef LIPMAPCALCULATIONS_H
#define LIPMAPCALCULATIONS_H
#include <QRect>
#include <QObject>

class LIPMapCalculations : public QObject
{
    Q_OBJECT
public:
    explicit LIPMapCalculations(QObject *parent = nullptr);
public:

  /**
   * Constructor
   * \param dpi Monitor resolution in dots per inch
   * \param mapUnits Units of the data on the map
   */


  /**
   * Sets the \a dpi (dots per inch) for the output resolution, to be used in scale calculations.
   * \see dpi()
   */
  void setDpi( double dpi );

  /**
   * Returns the DPI (dots per inch) used in scale calculations.
   * \see setDpi()
   */
  double dpi() const;

  /**
   * Set the map units
   * \param mapUnits Units of the data on the map. Must match a value from the
   */
  //void setMapUnits( QgsUnitTypes::DistanceUnit mapUnits );

  //! Returns current map units
  //QgsUnitTypes::DistanceUnit mapUnits() const;

  /**
   * Calculate the scale denominator
   * \param mapExtent QgsRectangle containing the current map extent
   * \param canvasWidth Width of the map canvas in pixel (physical) units
   * \returns scale denominator of current map view, e.g. 1000.0 for a 1:1000 map.
   */
  double calculate( const QRectF &mapExtent, double canvasWidth ) const;

  /**
   * Calculate the image size in pixel (physical) units
   * \param mapExtent QgsRectangle containing the current map extent
   * \param scale Scale denominator, e.g. 1000.0 for a 1:1000 map
   * \returns image size
   * \since QGIS 3.24
   */
  QSizeF calculateImageSize( const QRectF &mapExtent, double scale ) const;

  /**
   * Calculate the distance between two points in geographic coordinates.
   * Used to calculate scale for map views with geographic (decimal degree)
   * data.
   * \param mapExtent QgsRectangle containing the current map extent
   */
  double calculateGeographicDistance( const QRectF &mapExtent ) const;

private:

  //! Calculate the \a delta and \a conversionFactor values based on the provided \a mapExtent.
  void calculateMetrics( const QRectF &mapExtent, double &delta, double &conversionFactor ) const;

  //! dpi member
  double mDpi;

  //! map unit member
  //QgsUnitTypes::DistanceUnit mMapUnits;
signals:

};

#endif // LIPMAPCALCULATIONS_H
