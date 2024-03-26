#ifndef WATCHERS_H
#define WATCHERS_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>

#include <QDateTime>
#include <QFile>


class BasicWatcher : public QObject
{
    Q_OBJECT

public:
    BasicWatcher(QObject *parent = nullptr);
    ~BasicWatcher();

    void setTickRate(int tick) { updateTimer.setInterval(tick); }
    void startWatcher() { updateTimer.start(); };
    void stopWatcher() { updateTimer.stop(); }
    const int getError() { return error; }
private slots:
    virtual void updateData() = 0;
private:
    QTimer updateTimer;

protected:
    /**
     * @brief error - код ошибки
     *        -1 - файл не доступен
     *        -2 - значение не найдено
     */
    int error = 0;
};

class CpuWatcher : public BasicWatcher
{
    Q_OBJECT

public:
    CpuWatcher(QObject *parent = nullptr);

private slots:
    void updateData() override;
private:
    int updateCpuName();
    int updateCpuFreq();
private:
    double cpuLoad = 0;
    QList<double> coreLoad;
    QList<double> coreFreq;
    QDateTime beginTime;
    int processes = 0;
    QString cpuName;

    QFile cpuStat;
    QFile cpuInfo;
};

class WatchersController : public QObject
{
public:
    WatchersController();
    ~WatchersController();
public:
    void startWatchers() {for (auto i:watchers) i->startWatcher();}
    void stopWatchers(){for (auto i:watchers) i->stopWatcher();}
private:
    QList <BasicWatcher *> watchers;

};

#endif // WATCHERS_H
