#ifndef LIPMAPCALCULATIONS_H
#define LIPMAPCALCULATIONS_H
#include <QRect>
#include <QObject>
#include <QApplication>
#include <QScreen>
#include "lipproject.h"

class LIPMapCalculations : public QObject
{
    Q_OBJECT
public:
    explicit LIPMapCalculations(QObject *parent = nullptr);
public:

  void setDpi( double dpi );

  double dpi() const;



  /**
   * mapExtent - видимая область графической сцены
   * canvasWidth - ширина виджета LIPMapHolder в пикселях
   */
  double calculate( const QRectF &mapExtent, double canvasWidth ) const;


  /**
   * mapExtent видимая область графической сцены
   */
  double calculateGeographicDistance( const QRectF &mapExtent ) const;

private:

  void calculateMetrics( const QRectF &mapExtent, double &delta, double &conversionFactor ) const;

  double mDpi;



};

#endif // LIPMAPCALCULATIONS_H
