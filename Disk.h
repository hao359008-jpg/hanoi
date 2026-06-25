#pragma once
#include <string>

class Disk {
private:
    int m_size;
public:
    Disk(int size = 1);
    int getSize() const;
    std::string getDisplayString(int maxWidth) const;
};