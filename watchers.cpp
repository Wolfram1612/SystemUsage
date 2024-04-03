#include "watchers.h"

#include <QRegularExpression>
#include <QStorageInfo>
#include <QDebug>
#include <QString>
#include <QTextStream>

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

    uptime.setFileName("/proc/uptime");
    if(!uptime.exists()) error = -1;

    view = new CpuView;
    name.append("ЦПУ");
    updateCpuInfo();
}

void CpuWatcher::updateData()
{
    updateCpuFreq();
    updateCpuLoad();
    double av = 0;
    for(auto i:cpuLoad) av += i;
    av /= cpuLoad.size();
    dynamic_cast<CpuView *>(view)->setCpuLoad(av);
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
    dynamic_cast<CpuView *>(view)->setCpuName(cpuName);

    QRegularExpression coreRE("siblings\\s*:\\s*(.*)");
    QRegularExpressionMatch coreM(coreRE.match(cpuString));
    if(coreM.hasMatch()){
        const int coreCount = coreM.captured(1).toInt();
        coreFreq.resize(coreCount);
        cpuLoadRaw.resize(coreCount);
        prevCpuLoadRaw.resize(coreCount);
        cpuLoad.resize(coreCount);
        for(auto &i:cpuLoadRaw) i.resize(10);
        for(auto &i:prevCpuLoadRaw) i.resize(10);
    }
    else {
        error = -2;
    }

    QStringList ut(readFile(uptime).split(' '));
    beginTime.setSecsSinceEpoch(QDateTime::currentDateTime().currentSecsSinceEpoch() - ut.at(0).toDouble());

    dynamic_cast<CpuView *>(view)->setBeginTime(beginTime);
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

    for(int i = 0; i < cpuLoadRaw.size(); i++){
        for(int j = 0; j < cpuLoadRaw[i].size(); j++){
            prevCpuLoadRaw[i][j] = cpuLoadRaw[i][j];
        }
    }

    QRegularExpression coreRE("cpu\\d+\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator coreM(coreRE.globalMatch(cpuData));
    int coreIndex = 0;
    while(coreM.hasNext()){
        QRegularExpressionMatch n = coreM.next();
        int index = 0;
        for(auto i:n.captured(1).split(" ")) {
            cpuLoadRaw[coreIndex][index] = i.toInt();
            index++;
        }
        coreIndex++;
    }

    QRegularExpression processesRE("processes\\s*(.*)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch processesM(processesRE.match(cpuData));
    if(processesM.hasMatch()){
        processes = processesM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    for(int i = 0; i < cpuLoad.size(); i++){
        int idle = cpuLoadRaw[i][3] + cpuLoadRaw[i][4];
        int pIdle = prevCpuLoadRaw[i][3] + prevCpuLoadRaw[i][4];

        int noIdle = cpuLoadRaw[i][0] + cpuLoadRaw[i][1] + cpuLoadRaw[i][2] + cpuLoadRaw[i][5] + cpuLoadRaw[i][6] + cpuLoadRaw[i][7] + cpuLoadRaw[i][8] + cpuLoadRaw[i][9];
        int pNoIdle = prevCpuLoadRaw[i][0] + prevCpuLoadRaw[i][1] + prevCpuLoadRaw[i][2] + prevCpuLoadRaw[i][5] + prevCpuLoadRaw[i][6] + prevCpuLoadRaw[i][7] + prevCpuLoadRaw[i][8] + prevCpuLoadRaw[i][9];

        int total = idle + noIdle;
        int pTotal = pIdle + pNoIdle;

        double totald = total - pTotal;
        double idled = idle - pIdle;
        cpuLoad[i] = (totald-idled)/totald*100;
    }
}

RamWatcher::RamWatcher(QObject *parent) : BasicWatcher{parent}
{
    ramInfo.setFileName("/proc/meminfo");
    if(!ramInfo.exists()) error = -1;
    name.append("ОЗУ");
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

    QRegularExpression swapTotalRE("SwapTotal:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch swapTotalM(swapTotalRE.match(ramString));
    if(swapTotalM.hasMatch()){
        swapTotal = swapTotalM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    QRegularExpression swapFreeRE("SwapFree:\\s*(\\d+)\\s*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch swapFreeM(swapFreeRE.match(ramString));
    if(swapFreeM.hasMatch()){
        swapFree = swapFreeM.captured(1).toInt();
    }
    else {
        error = -2;
    }

    QString a;
    a.append("RAM: " + QString::number(ramFree) + "/" + QString::number(ramTotal) + "\r\n"
             "SWAP: " + QString::number(swapFree) + "/" + QString::number(swapTotal));
}

WatchersController::WatchersController()
{
    watchers.append(new CpuWatcher(this));
    watchers.append(new RamWatcher(this));
    watchers.append(new DiscWatcher(this));
    view.addPage(watchers[0]->getName(), watchers[0]->getView());
    view.show();
}

WatchersController::~WatchersController()
{
    stopWatchers();
    for(auto i:watchers) {
        i->deleteLater();
    }
}



DiscWatcher::DiscWatcher(QObject *parent) : BasicWatcher{parent}
{
    diskInfo.setFileName("/proc/diskstats");
    if(!diskInfo.exists()) error = -1;
    updateDiskList();
}

void DiscWatcher::updateData()
{
    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    QString diskString(readFile(diskInfo));
    QString a("Disk load:");
    for(int i = 0; i < diskList.size(); i++){
        diskList[i]->free = drives.at(i).bytesFree();
        a.append(diskList[i]->name + ": " + QString::number(diskList[i]->free) +
                 "/" + QString::number(diskList[i]->total) + "\r");
    }
    qInfo() << a;
}

void DiscWatcher::updateDiskList()
{
    // QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    diskList.clear();
    // for(int i = 0; i < drives.size(); i++){
    //     diskList.append(new disk);
    //     diskList[i]->name = drives.at(i).displayName();
    //     diskList[i]->total = drives.at(i).bytesTotal();
    // }

}
