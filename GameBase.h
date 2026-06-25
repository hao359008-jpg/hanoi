#pragma once

class GameBase {
public:
    virtual ~GameBase() = default;

    virtual void init(int diskCount = 3) = 0;
    virtual void reset() = 0;
    virtual bool isGameOver() const = 0;
    virtual int getMoveCount() const = 0;
};