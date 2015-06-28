/*
 *  MenuState.h
 *  Example "menu" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "GameState.h"
#include "Sprite.h"

class MenuState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();

    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    // Implement Singleton Pattern
    static MenuState* instance()
    {
        return &m_MenuState;
    }

    protected:

    MenuState() {}

    private:

    bool up, down, left, right, ret, space;

    enum class menu { STARTMENU, GAMESELECT };
    enum class startOptions { NEWGAME, EXIT };
    enum class gameMode { EASY, MEDIUM, HARD };
   
    menu currentMenu;
    menu oldMenu;
    gameMode currentMode;
    startOptions currentOption;

    static MenuState m_MenuState;

    cgf::Sprite menuSprite;
    cgf::Sprite menuText;
    cgf::Sprite cursor;
    cgf::InputManager* im;
};

#endif