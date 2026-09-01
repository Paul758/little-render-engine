#pragma once

class IInput
{
public:
    virtual ~IInput() = default;

    virtual bool isKeyDown(int key) const = 0;
    virtual bool isKeyPressed(int key) const = 0;
    virtual bool isKeyReleased(int key) const = 0;
};