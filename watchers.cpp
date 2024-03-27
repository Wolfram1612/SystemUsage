#include "watchers.h"

#include <QRegularExpression>
#include <QStorageInfo>
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

QString BasicWatcher::readFile(QFile &file)
{
    if(!file.open(QIODevice::ReadOnly)){
        error = -1;
        return QString();
    }
    QTextStream in(&file);
    QString fileData = in.readAll();
    file.close();
    return fileData;
}

CpuWatcher::CpuWatcher(QObject *parent) : BasicWatcher{parent}
{
    cpuStat.setFileName("/proc/stat");
    if(!cpuStat.exists()) error = -1;

    cpuInfo.setFileName("/proc/cpuinfo");
    if(!cpuInfo.exists()) error = -1;
    updateCpuInfo();
}

void CpuWatcher::updateData()
{
    updateCpuFreq();
    updateCpuLoad();
//    QString a;
//    for(auto i:coreFreq) a.append(QString("%1 ").arg(i));
//    qInfo() << a;
}

void CpuWatcher::updateCpuInfo()
{

    QString cpuString(readFile(cpuInfo));
    QRegularExpression modelRE("model name\\s*:\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch modelM(modelRE.match(cpuString));
    if(modelM.hasMatch()){
        cpuName = modelM.captured(1);
    }
    else {
        error = -2;
    }

    QRegularExpression coreRE("siblings\\s*:\\s*(.*)");
    QRegularExpressionMatch coreM(coreRE.match(cpuString));
    if(coreM.hasMatch()){
        coreFreq.resize(coreM.captured(1).toInt());
        coreLoad.resize(coreM.captured(1).toInt());
    }
    else {
        error = -2;
    }

    QRegularExpression btimeRE("btime\\s*(.*)");
    QRegularExpressionMatch btimeM(btimeRE.match(readFile(cpuStat)));
    if(btimeM.hasMatch()){
        beginTime.setMSecsSinceEpoch(btimeM.captured(1).toULongLong());
    }
    else {
        error = -2;
    }

}

void CpuWatcher::updateCpuFreq()
{
    QRegularExpression freqRE("cpu MHz\\s*:\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator freqM(freqRE.globalMatch(readFile(cpuInfo)));

    int coreIndex = 0;
    while (freqM.hasNext()) {
        QRegularExpressionMatch n = freqM.next();
        coreFreq[coreIndex] = n.captured(1).toDouble();
        coreIndex++;
    }
}

void CpuWatcher::updateCpuLoad()
{
    QString cpuData(readFile(cpuStat));

    QRegularExpression cpuRE("cpu\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch cpuM(cpuRE.match(cpuData));
    if(cpuM.hasMatch()){
        QVector<int> cpuValues;
        for(auto i:cpuM.captured(1).split(" ")) cpuValues.append(i.toInt());
        int total = cpuValues.at(1) + cpuValues.at(2) + cpuValues.at(3) + cpuValues.at(4);
        cpuLoad = 100.0 * (total - cpuValues.at(4)) / total;
    }
    else {
        error = -2;
    }

    QRegularExpression coreRE("cpu\\d+\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator coreM(coreRE.globalMatch(cpuData));
    int coreIndex = 0;
    while(coreM.hasNext()){
        QRegularExpressionMatch n = coreM.next();
        QVector<int> coreValues;
        for(auto i:n.captured(1).split(" ")) coreValues.append(i.toInt());
        int total = coreValues.at(1) + coreValues.at(2) + coreValues.at(3) + coreValues.at(4);
        coreLoad[coreIndex] = 100.0 * (total - coreValues.at(4)) / total;
        coreIndex++;
    }

    QRegularExpression processesRE("processes\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch processesM(processesRE.match(cpuData));
    if(cpuM.hasMatch()){
        processes = cpuM.captured(1).toInt();
    }
    else {
        error = -2;
    }
}

RamWatcher::RamWatcher(QObject *parent) : BasicWatcher{parent}
{
    ramInfo.setFileName("/proc/meminfo");
    if(!ramInfo.exists()) error = -1;
}

void RamWatcher::updateData()
{
    QString ramString = readFile(ramInfo);

    QRegularExpression totalRE("MemTotal:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch totalM(totalRE.match(ramString));
    if(totalM.hasMatch()){
        ramTotal = totalM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    QRegularExpression freeRE("MemFree:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch freeM(freeRE.match(ramString));
    if(freeM.hasMatch()){
        ramFree = freeM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    QRegularExpression swapTotalRE("MemFree:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch swapTotalM(swapTotalRE.match(ramString));
    if(swapTotalM.hasMatch()){
        swapTotal = swapTotalM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    QRegularExpression swapFreeRE("MemFree:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch swapFreeM(swapFreeRE.match(ramString));
    if(swapFreeM.hasMatch()){
        swapFree = swapFreeM.captured(1).toInt();
    }
    else {
        error = -2;
    }
}

WatchersController::WatchersController()
{
    watchers.append(new CpuWatcher(this));
}

WatchersController::~WatchersController()
{
    stopWatchers();
    for(auto i:watchers) {
        i->deleteLater();
    }
}



DiscWatcher::DiscWatcher()
{
    diskInfo.setFileName("/proc/diskstats");
    if(!diskInfo.exists()) error = -1;
}

DiscWatcher::~DiscWatcher()
{

}

void DiscWatcher::updateData()
{

}

void DiscWatcher::updateDiskList()
{
    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    diskList.clear();
    diskList.resize(drives.size());
    QListIterator<disk> it(diskList);
    for(auto i:drives) it.next().name = i.name()
}
