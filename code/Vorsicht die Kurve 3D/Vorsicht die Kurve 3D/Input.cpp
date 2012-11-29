#include "Input.h"
vector<char> m_keysdown;
vector<char> m_keysup;
//int wheel;
//b2Vec2 m_mouseposition;
//bool m_ismousedown;

bool Input::isKeyDown(char m_SDL_key) {

    if (count(m_keysdown.begin(), m_keysdown.end(), m_SDL_key) > 0) {
        return true;
    }

    return false;

}



void Input::PushKeyDown(char m_SDL_key) {

    m_keysdown.push_back(m_SDL_key);

}

void Input::ClearKeys() {

    m_keysdown.clear();
    m_keysup.clear();
    //wheel = 0;
    //m_ismousedown = false;

}

void Input::PushKeyUp(char m_SDL_key) {

    m_keysup.push_back(m_SDL_key);
}

bool Input::isKeyUp(char m_SDL_key) {

    if (count(m_keysup.begin(), m_keysup.end(), m_SDL_key) > 0) {

        return true;
    }

    return false;

}

//int Input::GetWheelDelta() {
    //return wheel;
//}

//void Input::SetWheelDelta(int value) {
    //wheel = value;
//}

//void Input::SetMousePosition(b2Vec2 mp) {
    //m_mouseposition = mp;
//}

//b2Vec2 Input::GetMousePosition() {
    //return m_mouseposition;
//}

//void Input::SetMouseDown(bool state){
    //m_ismousedown = state;
//}

//bool Input::isMouseDown(){
    //return m_ismousedown;
//}