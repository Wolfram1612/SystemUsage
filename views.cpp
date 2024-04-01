#include "views.h"

CpuView::CpuView()
{
    setLayout(&baseLayout);
    baseLayout.addWidget(&cpuName);
    // baseLayout.addWidget(&chartview);
   baseLayout.addLayout(&stack);
    baseLayout.addWidget(&beginLabel);
    cpuName.setAlignment(Qt::AlignCenter);


   stack.addWidget(&chartview);
   chartview.setChart(&cpuChart);
   cpuChart.addAxis(new QValueAxis(), Qt::AlignBottom);

   // chartview
}

void CpuView::setCpuLoad(double load)
{
    updateBegintime();
}

void CpuView::setCpuName(QString name)
{
    cpuName.setText(name);
}

void CpuView::setBeginTime(QDateTime bt)
{
    // beginTime.setText("Включено:" + bt);
    begintime = bt;
    updateBegintime();
}

void CpuView::mouseReleaseEvent(QMouseEvent *e)
{

}

void CpuView::updateBegintime()
{
    QString timeFromStart;
    quint64 sec = begintime.secsTo(QDateTime::currentDateTime());
    int years = sec / (365 * 24 * 60 * 60);
    sec %= (365 * 24 * 60 * 60);
    if(years > 0) timeFromStart.append(QString("Лет: %1. ").arg(years));
    int months = sec / (30 * 24 * 60 * 60);
    sec %= (30 * 24 * 60 * 60);
    if(months > 0) timeFromStart.append(QString("Месяцев: %1. ").arg(months));
    int days = sec / (24 * 60 * 60);
    sec %= (24 * 60 * 60);
    if(days > 0) timeFromStart.append(QString("Дней: %1. ").arg(days));
    int hours = sec / (60 * 60);
    sec %= (60 * 60);
    if(hours > 0) timeFromStart.append(QString("Часов: %1. ").arg(hours));
    int minutes = sec / 60;
    sec %= 60;
    if(minutes > 0) timeFromStart.append(QString("Минут: %1. ").arg(minutes));
    timeFromStart.append(QString("Секунд: %1.").arg(sec));

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
