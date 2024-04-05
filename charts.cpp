#include "charts.h"

CpuChart::CpuChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    axisX(new QValueAxis()),
    axisY(new QValueAxis())
{
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisY,Qt::AlignLeft);
    axisX->setRange(0, 100);
    axisX->setTickCount(10);
    axisX->setVisible(false);
    axisY->setMax(100);
    axisY->setTitleText("Загрузка ЦП, %");
}

void CpuChart::appendValues(QList<double> &values)
{
    if(values.size() != coreSeries.size()) {
        coreSeries.resize(values.size());
        coreAreas.resize(values.size());
        resetSeries();
    }

    for(int i = 0; i < coreSeries.size(); i++){
        QPointF p(x, values[i]);
        coreSeries[i]->append(p);
        coreAreas[i]->setName(QString("ЦП %1").arg(i));
    }
    x++;
    if(x > axisX->max()) scroll(plotArea().width() / (axisX->max() - axisX->min()), 0);

}

void CpuChart::resetSeries()
{
    for(int i = 0; i < coreSeries.size(); i++){
        coreSeries[i] = new QSplineSeries;
        coreAreas[i] = new QAreaSeries;
        addSeries(coreAreas[i]);
        coreAreas[i]->attachAxis(axisX);
        coreAreas[i]->attachAxis(axisY);
        coreAreas[i]->setUpperSeries(coreSeries[i]);
        if(i > 0) coreAreas[i]->setLowerSeries(coreSeries[i - 1]);
        QColor c;
        // c.setRgbF(((double)1 - (double)1 / (double)coreSeries.size()) * (double)i,
        //           (double)1 - ((double)1 / (double)coreSeries.size()) * (double)i,
        //           (double)1 - ((double)1 / (double)coreSeries.size()) * (double)i);
        int dr = (255 / coreSeries.size());
        int r = (dr + dr * i * 230) % 255;
        int g = (dr + dr * i * 60) % 255;
        int b = (dr + dr * i * 168) % 255;
        c.setRgb(r, g, b);
        QPen pe(c);
        pe.setWidth(0);
        QBrush br(c);
        coreAreas[i]->setPen(pe);
        coreAreas[i]->setBrush(br);
    }
}

RamChart::RamChart(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    axisX(new QValueAxis()),
    axisRamY(new QValueAxis()),
    axisSwapY(new QValueAxis())
{
    addAxis(axisX,Qt::AlignBottom);
    addAxis(axisRamY,Qt::AlignLeft);
    addAxis(axisSwapY,Qt::AlignRight);
    axisX->setRange(0, 100);
    axisX->setTickCount(10);
    axisX->setVisible(false);
    axisRamY->setTitleText("RAM, GB");
    axisSwapY->setTitleText("Swap, GB");

    ramSeries = new QSplineSeries(this);
    addSeries(ramSeries);
    ramSeries->attachAxis(axisRamY);
    ramSeries->attachAxis(axisX);
    ramSeries->setName("RAM");

    swapSeries = new QSplineSeries(this);
    addSeries(swapSeries);
    swapSeries->attachAxis(axisSwapY);
    swapSeries->attachAxis(axisX);
    swapSeries->setName("Swap");

}

void RamChart::appendValues(int memT, int memF, int swapT, int swapF)
{
    axisRamY->setMax(memT/1000000);
    axisSwapY->setMax(swapT/1000000);
    ramSeries->append(x,(double)(memT - memF)/(double)1000000);
    swapSeries->append(x,(double)(swapT - swapF)/(double)1000000);
    x++;
    if(x > axisX->max()) scroll(plotArea().width() / (axisX->max() - axisX->min()), 0);

}
