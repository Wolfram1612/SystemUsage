#include "views.h"

CpuView::CpuView()
{
    baseLayout.insertWidget(0, &cpuName);
    baseLayout.addWidget(&beginLabel);
    cpuName.setAlignment(Qt::AlignCenter);
    view.setChart(&cpuChart);
}

void CpuView::setCpuLoad(QList<double> load)
{
    for(int i = 0; i < load.size(); i++){
        load[i] /=  load.size();
        if(i > 0) load[i] += load[i - 1];
    }
    cpuChart.appendValues(load);
    updateBegintime();
}

void CpuView::setCpuName(QString name)
{
    cpuName.setText(name);
}

void CpuView::setBeginTime(QDateTime bt)
{
    begintime = bt;
    updateBegintime();
}

void CpuView::mouseReleaseEvent(QMouseEvent *e)
{

}

void CpuView::updateBegintime()
{
    auto i = [](int val){
        if(val < 10) return QString("0%1").arg(val);
        else return QString("%1").arg(val);
    };
    QString timeFromStart;
    quint64 sec = begintime.secsTo(QDateTime::currentDateTime());
    int years = sec / (365 * 24 * 60 * 60);
    sec %= (365 * 24 * 60 * 60);
    if(years > 0) timeFromStart.append(i(years) + " ");
    int months = sec / (30 * 24 * 60 * 60);
    sec %= (30 * 24 * 60 * 60);
    if(months > 0) timeFromStart.append(i(months) + " ");
    int days = sec / (24 * 60 * 60);
    sec %= (24 * 60 * 60);
    if(days > 0) timeFromStart.append(i(days) + " ");
    int hours = sec / (60 * 60);
    sec %= (60 * 60);
    if(hours > 0) timeFromStart.append(i(hours) + ":");
    int minutes = sec / 60;
    sec %= 60;
    if(minutes > 0) timeFromStart.append(i(minutes) + ":");
    timeFromStart.append(i(sec));

    beginLabel.setText("Время включения: " + begintime.toString("hh:mm:ss dd.MM.yy") + "\r\n"
                       "Времени с включения: " + timeFromStart);
}

ControllerView::ControllerView()
{
    setLayout(&layout);
    layout.addWidget(&tab);
    resize(800, 600);
}

void ControllerView::addPage(QString name, QWidget *page)
{
    tab.addTab(page, name);
}

RamView::RamView()
{
    view.setChart(&chart);
}

void RamView::setMem(int memT, int memF, int swapT, int swapF)
{
    chart.appendValues(memT, memF, swapT, swapF);
}

NetView::NetView()
{
    view.setChart(&chart);
}

void NetView::setNetSpeed(int rc, int tm)
{
    chart.setNetSpeed(rc, tm);
}

BaseView::BaseView()
{
    setLayout(&baseLayout);
    baseLayout.addWidget(&view);
    view.setRenderHint(QPainter::Antialiasing);
}
