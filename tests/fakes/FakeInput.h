#pragma once

#include <GLFW/glfw3.h>

#include "IInput.h"

class FakeInput : public IInput
{
public:
    bool wDown = false;
    bool aDown = false;
    bool sDown = false;
    bool dDown = false;

    bool jumpPressed = false;

    bool isKeyDown(int key) const override
    {
        if (key == GLFW_KEY_W) return wDown;
        if (key == GLFW_KEY_A) return aDown;
        if (key == GLFW_KEY_S) return sDown;
        if (key == GLFW_KEY_D) return dDown;

        return false;
    }

    bool isKeyPressed(int key) const override
    {
        if (key == GLFW_KEY_SPACE)
        {
            return jumpPressed;
        }
        return false;
    }

    bool isKeyReleased(int key) const override
    {
        return false;
    }
};