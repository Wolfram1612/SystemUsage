#ifndef CHARTS_H
#define CHARTS_H

#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QAbstractAxis>
#include <QAreaSeries>

class CpuChart : public QChart
{
    Q_OBJECT
public:
    CpuChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    void appendValues(QList<double> values);
    void appendValue(qreal val);

private:
    QList<QSplineSeries *> coreSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal x = 0;
};



#endif // CHARTS_H
