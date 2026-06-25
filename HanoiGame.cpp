#include "HanoiGame.h"
#include <cmath>

using namespace std;

HanoiGame::HanoiGame() : m_diskCount(3), m_moveCount(0), m_gameOver(false) {
    m_pillars[0] = Pillar(1);
    m_pillars[1] = Pillar(2);
    m_pillars[2] = Pillar(3);
}

void HanoiGame::init(int diskCount) {
    m_diskCount = (diskCount >= 1 && diskCount <= 10) ? diskCount : 3;
    reset();
}

void HanoiGame::reset() {
    for (int i = 0; i < 3; ++i) m_pillars[i].clear();
    for (int i = m_diskCount; i >= 1; --i) {
        m_pillars[0].pushDisk(Disk(i));
    }
    m_moveCount = 0;
    m_gameOver = false;
}

bool HanoiGame::isGameOver() const {
    return m_gameOver;
}

int HanoiGame::getMoveCount() const {
    return m_moveCount;
}

bool HanoiGame::checkMoveValid(int fromPillar, int toPillar) const {
    if (fromPillar < 1 || fromPillar > 3 || toPillar < 1 || toPillar > 3) return false;
    if (fromPillar == toPillar) return false;
    int fromIdx = fromPillar - 1, toIdx = toPillar - 1;
    if (m_pillars[fromIdx].isEmpty()) return false;
    int fromTop = m_pillars[fromIdx].getTopSize();
    int toTop = m_pillars[toIdx].getTopSize();
    return toTop == 0 || fromTop < toTop;
}

bool HanoiGame::moveDisk(int fromPillar, int toPillar) {
    if (m_gameOver || !checkMoveValid(fromPillar, toPillar)) return false;
    int fromIdx = fromPillar - 1, toIdx = toPillar - 1;
    m_pillars[toIdx].pushDisk(m_pillars[fromIdx].popDisk());
    m_moveCount++;
    if (m_pillars[2].getDiskCount() == m_diskCount) m_gameOver = true;
    return true;
}

int HanoiGame::getOptimalSteps() const {
    return static_cast<int>(pow(2, m_diskCount)) - 1;
}

const vector<Disk>& HanoiGame::getPillarDisks(int pillarId) const {
    return m_pillars[pillarId - 1].getAllDisks();
}

int HanoiGame::getDiskCount() const {
    return m_diskCount;
}

void HanoiGame::recursiveHelper(int n, int from, int to, int aux, vector<pair<int, int>>& moves) const {
    if (n == 0) return;
    recursiveHelper(n - 1, from, aux, to, moves);
    moves.emplace_back(from, to);
    recursiveHelper(n - 1, aux, to, from, moves);
}

vector<pair<int, int>> HanoiGame::generateAutoMoves(int mode) const {
    if (mode == 1) {
        return generateNonRecursiveMoves();
    } else {
        vector<pair<int, int>> moves;
        recursiveHelper(m_diskCount, 1, 3, 2, moves);
        return moves;
    }
}

vector<pair<int, int>> HanoiGame::generateNonRecursiveMoves() const {
    vector<pair<int, int>> moves;
    vector<int> pillars[3];
    for (int i = m_diskCount; i >= 1; --i) pillars[0].push_back(i);
    
    int minPos = 0;
    int totalSteps = getOptimalSteps();

    for (int i = 0; i < totalSteps; ++i) {
        if (i % 2 == 0) {
            int nextMinPos = (m_diskCount % 2 == 1) ? (minPos + 2) % 3 : (minPos + 1) % 3;
            pillars[minPos].pop_back();
            pillars[nextMinPos].push_back(1);
            moves.emplace_back(minPos + 1, nextMinPos + 1);
            minPos = nextMinPos;
        } else {
            int o1 = -1, o2 = -1;
            for (int j = 0; j < 3; ++j) {
                if (j != minPos) (o1 == -1) ? o1 = j : o2 = j;
            }
            int t1 = pillars[o1].empty() ? 0 : pillars[o1].back();
            int t2 = pillars[o2].empty() ? 0 : pillars[o2].back();
            int from = (t1 != 0 && (t2 == 0 || t1 < t2)) ? o1 : o2;
            int to = (from == o1) ? o2 : o1;
            int size = pillars[from].back();
            pillars[from].pop_back();
            pillars[to].push_back(size);
            moves.emplace_back(from + 1, to + 1);
        }
    }
    return moves;
}

bool HanoiGame::loadState(int diskCount, int moveCount, const vector<vector<int>>& pillarDisks) {
    if (diskCount < 1 || diskCount > 10 || pillarDisks.size() != 3) return false;
    m_diskCount = diskCount;
    m_moveCount = moveCount;
    m_gameOver = false;

    for (int i = 0; i < 3; ++i) {
        m_pillars[i].clear();
        for (int size : pillarDisks[i]) {
            if (size < 1 || size > diskCount) return false;
            m_pillars[i].pushDisk(Disk(size));
        }
    }
    m_gameOver = (m_pillars[2].getDiskCount() == m_diskCount);
    return true;
}