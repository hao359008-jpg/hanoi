#include "GameLogger.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace chrono;

GameLogger::GameLogger() : m_isTiming(false) {}

void GameLogger::startTimer() {
    m_startTime = steady_clock::now();
    m_isTiming = true;
}

double GameLogger::getElapsedSeconds() const {
    if (!m_isTiming) return 0.0;
    duration<double> elapsed = steady_clock::now() - m_startTime;
    return elapsed.count();
}

void GameLogger::stopTimer() {
    m_isTiming = false;
}

void GameLogger::reset() {
    m_isTiming = false;
}

bool GameLogger::saveGame(const HanoiGame& game, const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) return false;

    out << game.getDiskCount() << endl;
    out << game.getMoveCount() << endl;

    for (int i = 1; i <= 3; ++i) {
        const auto& disks = game.getPillarDisks(i);
        out << disks.size();
        for (const auto& disk : disks) out << " " << disk.getSize();
        out << endl;
    }
    out.close();
    return true;
}

bool GameLogger::loadGame(HanoiGame& game, const string& filename) const {
    ifstream in(filename);
    if (!in.is_open()) return false;

    int diskCount, moveCount;
    if (!(in >> diskCount >> moveCount)) { in.close(); return false; }

    vector<vector<int>> pillarDisks(3);
    for (int i = 0; i < 3; ++i) {
        int count;
        if (!(in >> count)) { in.close(); return false; }
        for (int j = 0; j < count; ++j) {
            int size;
            if (!(in >> size)) { in.close(); return false; }
            pillarDisks[i].push_back(size);
        }
    }
    in.close();
    return game.loadState(diskCount, moveCount, pillarDisks);
}