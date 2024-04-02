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
    Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    void appendValue(qreal val);
private:
    QSplineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    qreal x = 0;
};

#endif // CHART_H
