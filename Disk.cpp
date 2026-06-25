#include "Disk.h"
#include <string>

using namespace std;

Disk::Disk(int size) : m_size(size) {
    if (m_size < 1) m_size = 1;
}

int Disk::getSize() const {
    return m_size;
}

string Disk::getDisplayString(int maxWidth) const {
    string numStr = to_string(m_size);
    int fillCount = m_size;
    
    string diskStr = "[";
    
    for (int i = 0; i < fillCount; ++i) {
        diskStr += '=';
    }
    
    diskStr += numStr;
    
    for (int i = 0; i < fillCount; ++i) {
        diskStr += '=';
    }
    
    diskStr += ']';
    
    int actualWidth = static_cast<int>(diskStr.length());
    int padding = max(0, (maxWidth - actualWidth) / 2);
    return string(padding, ' ') + diskStr + string(padding, ' ');
}