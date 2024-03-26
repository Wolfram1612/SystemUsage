#include "watchers.h"

#include <QRegularExpression>
#include <QDebug>

BasicWatcher::BasicWatcher(QObject *parent) : QObject{parent}
{
    connect(&updateTimer, &QTimer::timeout, this, &BasicWatcher::updateData);
    updateTimer.setInterval(500);
}

BasicWatcher::~BasicWatcher()
{
    updateTimer.stop();
}

CpuWatcher::CpuWatcher(QObject *parent) : BasicWatcher{parent}
{
    cpuStat.setFileName("/proc/stat");
    if(!cpuStat.exists()) error = -1;

    cpuInfo.setFileName("/proc/cpuinfo");
    if(!cpuInfo.exists()) error = -1;
    updateCpuName();
}

void CpuWatcher::updateData()
{
    updateCpuFreq();
    QString a;
    for(auto i:coreFreq) a.append(QString("%1 ").arg(i));
    qInfo() << a;
}

int CpuWatcher::updateCpuName()
{
    if(!cpuInfo.open(QIODevice::ReadOnly)){
        error = -1;
        return error;
    }
    QTextStream in(&cpuInfo);
    QString cpuInfoText = in.readAll();
    cpuInfo.close();

    QRegularExpression modelRE("model name\\s*:\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch modelM = modelRE.match(cpuInfoText);
    if(modelM.hasMatch()){
        cpuName = modelM.captured(1);
    }
    else {
        error = -2;
        return error;
    }

    QRegularExpression coreRE("siblings\\s*:\\s*(.*)");
    QRegularExpressionMatch coreM = coreRE.match(cpuInfoText);
    if(coreM.hasMatch()){
        coreFreq.resize(coreM.captured(1).toInt());
        coreLoad.resize(coreM.captured(1).toInt());
    }
    else {
        error = -2;
        return error;
    }
    return 0;
}

int CpuWatcher::updateCpuFreq()
{
    if(!cpuInfo.open(QIODevice::ReadOnly)){
        error = -1;
        return error;
    }
    QTextStream in(&cpuInfo);
    QString cpuInfoText = in.readAll();
    cpuInfo.close();

    QRegularExpression freqRE("cpu MHz\\s*:\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator freqM = freqRE.globalMatch(cpuInfoText);
    int coreIndex = 0;
    while (freqM.hasNext()) {
        QRegularExpressionMatch n = freqM.next();
        coreFreq[coreIndex] = n.captured(1).toDouble();
        coreIndex++;
    }
    return 0;
}

WatchersController::WatchersController()
{
    watchers.append(new CpuWatcher(this));
}

WatchersController::~WatchersController()
{
    for(auto i:watchers) i->deleteLater();
}
