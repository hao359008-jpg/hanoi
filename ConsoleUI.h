#pragma once
#include "HanoiGame.h"
#include "GameLogger.h"
#include <string>

class ConsoleUI {
private:
    HanoiGame m_game;
    GameLogger m_logger;
    bool m_running;

    void clearScreen() const;
    void printMainMenu() const;
    void printGameScreen() const;
    void printVictory() const;
    int inputInt(int min, int max, const std::string& prompt) const;

    void handleMainMenu();
    void runManualMode();
    void runAutoMode();
    void handleSettings();
    void handleSave();
    void handleLoad();

public:
    ConsoleUI();
    void run();
};