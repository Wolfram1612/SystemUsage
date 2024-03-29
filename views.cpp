#include "views.h"

CpuView::CpuView()
{
    setLayout(&baseLayout);
    baseLayout.addWidget(&cpuName);
    baseLayout.addLayout(&stack);
    baseLayout.addWidget(&beginTime);
    cpuName.setAlignment(Qt::AlignCenter);
}

void CpuView::setCpuLoad(double load)
{

}

void CpuView::setCpuName(QString name)
{
    cpuName.setText(name);
}

void CpuView::setBeginTime(QString bt)
{
    beginTime.setText("Включено:" + bt);
}

void CpuView::mouseReleaseEvent(QMouseEvent *e)
{

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
