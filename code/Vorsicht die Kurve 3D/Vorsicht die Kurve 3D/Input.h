#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Input {
public:
    static bool isKeyDown(char m_SDL_key);
    static void PushKeyDown(char m_SDL_key);
    static bool isKeyUp(char m_SDL_key);
    static void PushKeyUp(char m_SDL_key);
    static void ClearKeys();
    //static int GetWheelDelta();
    //static void SetWheelDelta(int value);
    //static void SetMousePosition(b2Vec2 mp);
    //static bool isMouseDown();
    //static void SetMouseDown(bool state);
    //static b2Vec2 GetMousePosition();
};
