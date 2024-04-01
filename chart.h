#ifndef CHART_H
#define CHART_H

#include <QChart>
#include <QValueAxis>
#include <QSplineSeries>
#include <QAbstractAxis>

class Chart : public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags);
private:
    QSplineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
};

#endif // CHART_H
