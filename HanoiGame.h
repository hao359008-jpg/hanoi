#pragma once
#include "GameBase.h"
#include "Pillar.h"
#include <vector>
#include <utility>

class HanoiGame : public GameBase {
private:
    Pillar m_pillars[3];
    int m_diskCount;
    int m_moveCount;
    bool m_gameOver;

    void recursiveHelper(int n, int from, int to, int aux, std::vector<std::pair<int, int>>& moves) const;

public:
    HanoiGame();

    void init(int diskCount = 3) override;
    void reset() override;
    bool isGameOver() const override;
    int getMoveCount() const override;

    bool moveDisk(int fromPillar, int toPillar);
    bool checkMoveValid(int fromPillar, int toPillar) const;
    int getOptimalSteps() const;
    const std::vector<Disk>& getPillarDisks(int pillarId) const;
    int getDiskCount() const;

    std::vector<std::pair<int, int>> generateAutoMoves(int mode = 0) const;
    std::vector<std::pair<int, int>> generateNonRecursiveMoves() const;
    bool loadState(int diskCount, int moveCount, const std::vector<std::vector<int>>& pillarDisks);
};