#ifndef WATCHERS_H
#define WATCHERS_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>

#include <QDateTime>
#include <QFile>

#include "views.h"

class BasicWatcher : public QObject
{
    Q_OBJECT

public:
    BasicWatcher(QObject *parent = nullptr);
    ~BasicWatcher();

    void setTickRate(int tick) { updateTimer.setInterval(tick); }
    void startWatcher() { updateTimer.start(); };
    void stopWatcher() { updateTimer.stop(); }
    int getError() const { return error; }
    QWidget *getView() const { return view;}
    QString getName() const {return name;}

private slots:
    virtual void updateData() = 0;
private:
    QTimer updateTimer;
protected:
    QString readFile(QFile &file);
protected:
    /**
     * @brief error - код ошибки
     *        -1 - файл не доступен
     *        -2 - значение не найдено
     */
    int error = 0;
    QWidget *view = nullptr;
    QString name;
};

class CpuWatcher : public BasicWatcher
{
    Q_OBJECT
public:
    CpuWatcher(QObject *parent = nullptr);

private slots:
    void updateData() override;
private:
    void updateCpuInfo();
    void updateCpuFreq();
    void updateCpuLoad();
//    void update
private:
    double cpuLoad = 0;
    QList<double> coreLoad;
    QList<double> coreFreq;
    QDateTime beginTime;
    ulong processes = 0;
    QString cpuName;

    QFile cpuStat;
    QFile cpuInfo;
    QFile uptime;
};

class RamWatcher : public BasicWatcher
{
    Q_OBJECT
public:
    RamWatcher(QObject *parent = nullptr);
private slots:
    void updateData() override;
private:
    ulong ramTotal = 0;
    ulong ramFree = 0;
    ulong swapTotal = 0;
    ulong swapFree = 0;

    QFile ramInfo;
};

class DiscWatcher : public BasicWatcher
{
    Q_OBJECT
public:
    DiscWatcher(QObject *parent = nullptr);
private slots:
    void updateData() override;
private:
    void updateDiskList();
private:
    struct disk{
        QString name;
        quint64 reads = 0;
        quint64 writes = 0;
        quint64 total = 0;
        quint64 free = 0;
    };

    QList<disk *> diskList;
    QFile diskInfo;
};

class WatchersController : public QObject
{
    Q_OBJECT
public:
    WatchersController();
    ~WatchersController();
public:
    void startWatchers() {for (auto i:watchers) i->startWatcher();}
    void stopWatchers(){for (auto i:watchers) i->stopWatcher();}
private:
    QList <BasicWatcher *> watchers;
    ControllerView view;
};



#endif // WATCHERS_H
