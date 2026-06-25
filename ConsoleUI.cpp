#include "ConsoleUI.h"
#include <iostream>
#include <thread>
#include <limits>
#include <algorithm>

using namespace std;

ConsoleUI::ConsoleUI() : m_running(true) {
    m_game.init(3);
}

void ConsoleUI::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int ConsoleUI::inputInt(int min, int max, const string& prompt) const {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "输入无效，请输入 " << min << "~" << max << " 之间的整数。" << endl;
    }
}

void ConsoleUI::printMainMenu() const {
    cout << "==================== 汉诺塔游戏 ====================" << endl;
    cout << "1. 手动游戏模式" << endl;
    cout << "2. 自动演示模式" << endl;
    cout << "3. 难度设置 (当前: " << m_game.getDiskCount() << " 层)" << endl;
    cout << "4. 保存游戏进度" << endl;
    cout << "5. 加载游戏进度" << endl;
    cout << "6. 退出游戏" << endl;
    cout << "==================================================" << endl;
}

void ConsoleUI::printGameScreen() const {
    int maxDisk = m_game.getDiskCount();
    int minWidth = 7;
    int maxWidth = max(minWidth, maxDisk * 2 + 3);
    int height = maxDisk + 2;

    cout << "当前步数: " << m_game.getMoveCount() << " / 最优步数: " << m_game.getOptimalSteps() << endl;
    cout << "游戏时间: " << m_logger.getElapsedSeconds() << " 秒" << endl << endl;

    for (int h = 0; h < height; ++h) {
        for (int p = 1; p <= 3; ++p) {
            const auto& disks = m_game.getPillarDisks(p);
            int availableHeight = height - disks.size();
            if (h >= availableHeight && h < height) {
                int diskIdx = disks.size() - 1 - (h - availableHeight);
                cout << disks[diskIdx].getDisplayString(maxWidth) << " ";
            } else if (h == height - 1) {
                cout << string(maxWidth, '-') << " ";
            } else {
                string pad(maxWidth / 2, ' ');
                cout << pad << "|" << pad << " ";
            }
        }
        cout << endl;
    }

    for (int p = 1; p <= 3; ++p) {
        string label = to_string(p) + "号柱";
        int labelSize = static_cast<int>(label.size());
        int padLeft = (maxWidth - labelSize) / 2;
        int padRight = maxWidth - labelSize - padLeft;
        cout << string(padLeft, ' ') << label << string(padRight, ' ') << " ";
    }
    cout << endl << endl;
}

void ConsoleUI::printVictory() const {
    cout << "==================================================" << endl;
    cout << "                  恭喜通关！                       " << endl;
    cout << "==================================================" << endl;
    cout << "总移动步数: " << m_game.getMoveCount() << " 步" << endl;
    cout << "最优步数: " << m_game.getOptimalSteps() << " 步" << endl;
    cout << "用时: " << m_logger.getElapsedSeconds() << " 秒" << endl;
    cout << "==================================================" << endl;
}

void ConsoleUI::handleMainMenu() {
    printMainMenu();
    int choice = inputInt(1, 6, "请输入选项: ");
    switch (choice) {
        case 1: runManualMode(); break;
        case 2: runAutoMode(); break;
        case 3: handleSettings(); break;
        case 4: handleSave(); break;
        case 5: handleLoad(); break;
        case 6: m_running = false; break;
    }
}

void ConsoleUI::runManualMode() {
    m_game.reset();
    m_logger.startTimer();

    while (!m_game.isGameOver()) {
        clearScreen();
        printGameScreen();
        cout << "输入 0 可返回主菜单" << endl;
        int from = inputInt(0, 3, "起始柱子编号(1-3): ");
        if (from == 0) break;
        int to = inputInt(0, 3, "目标柱子编号(1-3): ");
        if (to == 0) break;

        if (!m_game.moveDisk(from, to)) {
            cout << "移动非法！大盘不能叠在小盘上，起始柱不能为空。" << endl;
            cout << "按回车继续...";
            cin.get();
        }
    }

    if (m_game.isGameOver()) {
        m_logger.stopTimer();
        clearScreen();
        printGameScreen();
        printVictory();
        cout << "按回车返回主菜单...";
        cin.get();
    }
}

void ConsoleUI::runAutoMode() {
    clearScreen();
    cout << "==================== 自动演示 ====================" << endl;
    cout << "1. 递归算法演示" << endl;
    cout << "2. 非递归算法演示" << endl;
    int mode = inputInt(1, 2, "请选择算法: ") - 1;
    int speed = inputInt(1, 10, "演示速度(1-10，数字越快越快): ");
    int delayMs = 1000 / speed;

    auto moves = m_game.generateAutoMoves(mode);
    m_game.reset();

    clearScreen();
    cout << (mode == 0 ? "递归算法" : "非递归算法") << "演示，共" << moves.size() << "步" << endl;
    printGameScreen();
    cout << "按回车开始...";
    cin.get();

    for (size_t i = 0; i < moves.size(); ++i) {
        m_game.moveDisk(moves[i].first, moves[i].second);
        clearScreen();
        cout << (mode == 0 ? "递归算法" : "非递归算法") << " 第" << i + 1 << "/" << moves.size() << "步" << endl;
        printGameScreen();
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }

    cout << "演示完成！按回车返回...";
    cin.get();
}

void ConsoleUI::handleSettings() {
    clearScreen();
    cout << "==================== 难度设置 ====================" << endl;
    int count = inputInt(1, 10, "设置盘子数量(1-10层): ");
    m_game.init(count);
    cout << "难度已设置为 " << count << " 层。" << endl;
    cout << "按回车返回...";
    cin.get();
}

void ConsoleUI::handleSave() {
    cout << (m_logger.saveGame(m_game) ? "存档成功！保存为 save.txt" : "保存失败！") << endl;
    cout << "按回车返回...";
    cin.get();
}

void ConsoleUI::handleLoad() {
    cout << (m_logger.loadGame(m_game) ? "存档加载成功！" : "加载失败，请检查存档文件。") << endl;
    cout << "按回车返回...";
    cin.get();
}

void ConsoleUI::run() {
    while (m_running) {
        clearScreen();
        handleMainMenu();
    }
    cout << "感谢游玩，再见！" << endl;
}