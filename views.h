#ifndef VIEWS_H
#define VIEWS_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedLayout>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <QTabWidget>
#include <QLabel>

class CpuView : public QWidget
{
public:
    CpuView();
    void setCpuLoad(double load);
    void setCpuName(QString name);
    void setBeginTime(QString bt);

protected slots:
    void mousePressEvent(QMouseEvent *e) override {}
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    QVBoxLayout baseLayout;
    QStackedLayout stack;
    QLabel cpuName;
    QChartView chartview;
//    QChart cpuChart;
    QLabel beginTime;
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

#endif // VIEWS_H
