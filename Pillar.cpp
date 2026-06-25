#include "Pillar.h"

using namespace std;

Pillar::Pillar(int id) : m_id(id) {}

bool Pillar::pushDisk(const Disk& disk) {
    if (!isEmpty() && disk.getSize() > getTopSize()) {
        return false;
    }
    m_disks.push_back(disk);
    return true;
}

Disk Pillar::popDisk() {
    if (isEmpty()) return Disk(0);
    Disk top = m_disks.back();
    m_disks.pop_back();
    return top;
}

int Pillar::getTopSize() const {
    return isEmpty() ? 0 : m_disks.back().getSize();
}

bool Pillar::isEmpty() const {
    return m_disks.empty();
}

int Pillar::getDiskCount() const {
    return m_disks.size();
}

int Pillar::getPillarId() const {
    return m_id;
}

const vector<Disk>& Pillar::getAllDisks() const {
    return m_disks;
}

void Pillar::clear() {
    m_disks.clear();
}