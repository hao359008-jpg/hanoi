#pragma once
#include <chrono>
#include <string>
#include "HanoiGame.h"

class GameLogger {
private:
    std::chrono::steady_clock::time_point m_startTime;
    bool m_isTiming;

public:
    GameLogger();
    void startTimer();
    double getElapsedSeconds() const;
    void stopTimer();
    void reset();

    bool saveGame(const HanoiGame& game, const std::string& filename = "save.txt") const;
    bool loadGame(HanoiGame& game, const std::string& filename = "save.txt") const;
};