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

class CpuView : public QWidget
{
public:
    CpuView();
    void setCpuLoad(double load);
    void setCpuName(QString name);
    void setBeginTime(QDateTime bt);

protected slots:
    void mousePressEvent(QMouseEvent *e) override {}
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    void updateBegintime();
private:
    QDateTime begintime;
    QVBoxLayout baseLayout;
    QStackedLayout stack;
    QLabel cpuName;
    QChartView chartview;
    CpuChart cpuChart;
    // QCharAxi
    QLabel beginLabel;
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

class RamView : public QWidget
{
public:
    RamView();
private:
    QVBoxLayout layout;
};

#endif // VIEWS_H
