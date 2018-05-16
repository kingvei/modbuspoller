#ifndef MODBUSPOLLER_POLLER_P_H
#define MODBUSPOLLER_POLLER_P_H

#include <modbus.h>

#include "poller.h"
#include "poller_global.h"

#include <QtCore/QQueue>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>

#include <QtSerialBus/QModbusDataUnit>


class QModbusClient;

namespace ModbusPoller {

class ReadActionThread; // yeah, rename this class to something like "libmodbusreadthread". Long but I know what it does immediatly
class WriteActionThread;

class MODBUSPOLLERSHARED_EXPORT PollerPrivate
{
public:
    PollerPrivate(Poller::Backend backend)
        : pollTimer(new QTimer)
        , modbusClient(nullptr)
        , libModbusClient(nullptr)
        , readQueue(new QQueue<QModbusDataUnit>())
        , writeQueue(new QQueue<QModbusDataUnit>())
        , connectionState(Poller::UNCONNECTED)
        , state(Poller::IDLE)
        , backend(backend)
        , readActionThread(nullptr)
        , writeActionThread(nullptr)
    {}

    ~PollerPrivate()
    {
        if (pollTimer->isActive()) {
            pollTimer->stop();
        }

        delete pollTimer;
    }

    QTimer *pollTimer;

    // the modbus client objs
    QModbusClient *modbusClient;
    modbus_t *libModbusClient;

    // read / write queues
    QSharedPointer<QQueue<QModbusDataUnit>> readQueue;
    QSharedPointer<QQueue<QModbusDataUnit>> writeQueue;

    QModbusDataUnit defaultPollCommand;

    // poller gen data
    Poller::ConnectionState connectionState;
    Poller::State state;
    Poller::Backend backend;

    // libmodbus specific workers. As libmodbus is only synchronous, we need to make
    // the read and write commands work in a seperate thread
    ReadActionThread *readActionThread;
    WriteActionThread *writeActionThread;
};

}   // ModbusPoller


#endif  // MODBUSPOLLER_POLLER_P_H
