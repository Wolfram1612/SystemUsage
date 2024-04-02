#include "chart.h"

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    axisX(new QValueAxis()),
    axisY(new QValueAxis())
{
    series = new QSplineSeries(this);
    QPen p(Qt::black);
    series->setPen(p);
    addSeries(series);
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    axisX->setRange(0, 100);
    axisX->setTickCount(10);
    axisX->setVisible(false);
    axisY->setMax(100);
}

void Chart::appendValue(qreal val)
{
    QPointF p(x, val);
    series->append(p);
    x++;
    if(x > axisX->max()) scroll(plotArea().width() / (axisX->max() - axisX->min()), 0);
}
