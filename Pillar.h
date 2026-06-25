#pragma once
#include <vector>
#include "Disk.h"

class Pillar {
private:
    std::vector<Disk> m_disks;
    int m_id;
public:
    Pillar(int id = 1);

    bool pushDisk(const Disk& disk);
    Disk popDisk();
    int getTopSize() const;
    bool isEmpty() const;
    int getDiskCount() const;
    int getPillarId() const;
    const std::vector<Disk>& getAllDisks() const;
    void clear();
};