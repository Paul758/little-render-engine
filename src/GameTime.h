#pragma once

class GameTime
{
public:
    GameTime();
    void update();
    float deltaTime() const;

private:
    float deltaTime_;
    float lastFrameTime_;
    float currentTime_;
};