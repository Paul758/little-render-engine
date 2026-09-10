#pragma once

enum class MouseButton
{
    Left,
    Right,
    Middle,

    Count
};

class IInput
{
public:
    virtual ~IInput() = default;

    virtual bool isKeyDown(int key) const = 0;
    virtual bool isKeyPressed(int key) const = 0;
    virtual bool isKeyReleased(int key) const = 0;

    virtual bool isMouseButtonDown(MouseButton mouseButton) const = 0;
    virtual bool isMouseButtonPressed(MouseButton mouseButton) const = 0;
    virtual bool isMouseButtonReleased(MouseButton mouseButton) const = 0;
};