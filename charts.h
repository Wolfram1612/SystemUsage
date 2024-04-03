#ifndef CHARTS_H
#define CHARTS_H

#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QAbstractAxis>

class CpuChart : public QChart
{
    Q_OBJECT
public:
    CpuChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    void appendValue(qreal val);
private:
    QSplineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal x = 0;
};

#endif // CHARTS_H
