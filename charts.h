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
    void appendValues(QList<double> &values);
private:
    void resetSeries();
private:
    QList<QSplineSeries *> coreSeries;
    QList<QAreaSeries *> coreAreas;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal x = 0;
};

class RamChart : public QChart
{
public:
    RamChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    void appendValues(int memT, int memF, int swapT, int swapF);
private:
    QValueAxis *axisX;
    QValueAxis *axisRamY;
    QValueAxis *axisSwapY;
    QSplineSeries *ramSeries = nullptr;
    QSplineSeries *swapSeries = nullptr;
    qreal x = 0;
};

#endif // CHARTS_H
