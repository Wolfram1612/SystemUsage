#ifndef VIEWS_H
#define VIEWS_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedLayout>

#include <QChartView>

#include <QTabWidget>
#include <QLabel>
#include <QDateTime>

#include "charts.h"

class BaseView : public QWidget
{
public:
    BaseView();

protected:
    QVBoxLayout baseLayout;
    QChartView view;
};

class CpuView : public BaseView
{
public:
    CpuView();
    void setCpuLoad(QList<double> load);
    void setCpuName(QString name);
    void setBeginTime(QDateTime bt);

protected slots:
    void mousePressEvent(QMouseEvent *e) override {}
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    void updateBegintime();
private:
    QDateTime begintime;
    QLabel cpuName;
    CpuChart cpuChart;
    QLabel beginLabel;
};

class RamView : public BaseView
{
public:
    RamView();
    void setMem(int memT, int memF, int swapT, int swapF);
private:
    RamChart chart;
};

class ControllerView : public QWidget
{
public:
    ControllerView();
    void addPage(QString name, QWidget *page);
private:
    QHBoxLayout layout;
    QTabWidget tab;

};

class NetView : public BaseView
{
public:
    NetView();
    void setNetSpeed(int rc, int tm);
private:
    NetChart chart;
};

#endif // VIEWS_H
