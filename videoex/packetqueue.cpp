#include "packetqueue.h"

PacketQueue::PacketQueue(QObject *parent):
    QObject(parent)
{
    this->cond = new QCondition();


}

void PacketQueue::setFirstPkt(AVPacketList *fPkt)
{
    firstPkt = fPkt;
}

void PacketQueue::setLastPkt(AVPacketList *lPkt)
{
    lastPkt = lPkt;
}

void PacketQueue::setNumPkts(int numberPkts)
{
    nb_packets = numberPkts;
}

void PacketQueue::setSize(int nSize)
{
    size = nSize;
}

void PacketQueue::lockMutex()
{
    mutex.lock();
}

void PacketQueue::unlockMutex()
{
    mutex.unlock();
}


