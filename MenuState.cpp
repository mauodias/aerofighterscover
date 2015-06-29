/*
 *  MenuState.cpp
 *  Example "menu" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */


#include <iostream>
#include "Game.h"
#include "InputManager.h"
#include "MenuState.h"
#include "PlayState.h"

#ifndef DEBUG
#define DEBUG 0
#endif

MenuState MenuState::m_MenuState;

using namespace std;

void MenuState::init()
{
    menuSprite.load("data/img/titleScreen.png"); // load menu state bitmap
    menuText.load("data/img/titleOptions.png");
    cursor.load("data/img/cursor.png");
    cursor.setPosition(400,300);
    cout << "MenuState Init Successful" << endl;

    im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::Left);
    im->addKeyInput("right", sf::Keyboard::Right);
    im->addKeyInput("up", sf::Keyboard::Up);
    im->addKeyInput("down", sf::Keyboard::Down);
    im->addKeyInput("ret", sf::Keyboard::Return);
    im->addKeyInput("space", sf::Keyboard::Space);

    currentMenu = menu::STARTMENU;
    oldMenu = currentMenu;
    currentMode = gameMode::EASY;
    currentOption = startOptions::NEWGAME;
}

void MenuState::cleanup()
{
    cout << "MenuState Cleanup Successful" << endl;
}

void MenuState::pause()
{
    cout << "MenuState Paused" << endl;
}

void MenuState::resume()
{
    cout << "MenuState Resumed" << endl;
}

void MenuState::handleEvents(cgf::Game* game)
{
    sf::Event event;
    sf::RenderWindow* screen = game->getScreen();

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();
        /*
        // check the type of the event...
        switch (event.type)
        {
            // window closed
        case sf::Event::Closed:
            game->quit();
            break;

            // key pressed
        case sf::Event::KeyPressed:
            if(event.key.code == sf::Keyboard::Space || )
                game->changeState(PlayState::instance());
            if(event.key.code == sf::Keyboard::Escape)
                game->quit();
            //game->changeState(PlayMap::instance());
            //game->changeState(PlayMapTop::instance());
            //game->changeState(PlayMapAI::instance());
            //game->changeState(PlayPhysics::instance());
            //game->changeState(PlayMapPhysics::instance());
            break;

            // we don't process other types of events
        default:
            break;
        }
        */
    }

    if(im->testEvent("up")) {
        if(!up)
        {
            up = true;
            if (currentMenu==menu::STARTMENU)
            {
                if (currentOption==startOptions::NEWGAME)
                {
                    currentOption = startOptions::EXIT;
                    if (DEBUG)
                        cout << "currentOption = EXIT" << endl;
                }
                else
                {
                    currentOption = startOptions::NEWGAME;
                    if (DEBUG)
                        cout << "currentOption = NEWGAME" << endl;
                }
            }
            else
            {
                switch(currentMode)
                {
                    case gameMode::EASY:
                        currentMode = gameMode::HARD;
                        if (DEBUG)
                            cout << "currentMode = HARD" << endl;
                        break;
                    case gameMode::MEDIUM:
                        currentMode = gameMode::EASY;
                        if (DEBUG)
                            cout << "currentMode = EASY" << endl;
                        break;
                    case gameMode::HARD:
                        currentMode = gameMode::MEDIUM;
                        if (DEBUG)
                            cout << "currentMode = MEDIUM" << endl;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else
    {
        if(up)
            up = false;
    }

    if(im->testEvent("down")) {
        if(!down)
        {
            down = true;
            if (currentMenu==menu::STARTMENU)
            {
                if (currentOption==startOptions::NEWGAME)
                {
                    currentOption = startOptions::EXIT;
                    if(DEBUG)
                        cout << "currentOption = EXIT" << endl;
                }
                else
                {
                    currentOption = startOptions::NEWGAME;
                    if(DEBUG)
                        cout << "currentOption = NEWGAME" << endl;
                }
            }
            else
            {
                switch(currentMode)
                {
                    case gameMode::EASY:
                        currentMode = gameMode::MEDIUM;
                        if (DEBUG)
                            cout << "currentMode = MEDIUM" << endl;
                        break;
                    case gameMode::MEDIUM:
                        currentMode = gameMode::HARD;
                        if (DEBUG)
                            cout << "currentMode = HARD" << endl;
                        break;
                    case gameMode::HARD:
                        currentMode = gameMode::EASY;
                        if (DEBUG)
                            cout << "currentMode = EASY" << endl;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else
    {
        if(down)
            down = false;
    }

    if(im->testEvent("left")) {
        if(!left)
        {
            left = true;
            if (currentMenu==menu::GAMESELECT)
            {
                currentMenu = menu::STARTMENU;
                if (DEBUG)
                    cout << "currentMenu = STARTMENU" << endl;
            }
        }
    }
    else
    {
        if(left)
            left = false;
    }

    if(im->testEvent("right")) {
        if(!right)
        {
            right = true;
            if (currentMenu==menu::STARTMENU)
            {
                if (currentOption==startOptions::NEWGAME)
                {
                    currentMenu = menu::GAMESELECT;
                    if (DEBUG)
                        cout << "currentMenu = GAMESELECT" << endl;
                }
                else
                {
                    game->quit();
                }
            }
            else
            {
                // IMPLEMENT GAME DIFFICULTY CHOICE
                game->changeState(PlayState::instance());
            }
        }
    }
    else
    {
        if(right)
            right = false;
    }

    if(im->testEvent("ret")) {
        if(!ret)
        {
            ret = true;
            if (currentMenu==menu::STARTMENU)
            {
                if (currentOption==startOptions::NEWGAME)
                {
                    currentMenu = menu::GAMESELECT;
                    if (DEBUG)
                        cout << "currentMode = GAMESELECT" << endl;
                }
                else
                {
                    game->quit();
                }
            }
            else
            {
                // IMPLEMENT GAME DIFFICULTY CHOICE
                game->changeState(PlayState::instance());
            }
        }
    }
    else
    {
        if(ret)
            ret = false;
    }

    if(im->testEvent("space")) {
        if(!space)
        {
            space = true;
            if (currentMenu==menu::STARTMENU)
            {
                if (currentOption==startOptions::NEWGAME)
                {
                    currentMenu = menu::GAMESELECT;
                    if (DEBUG)
                        cout << "currentMode = GAMESELECT" << endl;
                }
                else
                {
                    game->quit();
                }
            }
            else
            {
                // IMPLEMENT GAME DIFFICULTY CHOICE
                game->changeState(PlayState::instance());
            }
        }
    }
    else
    {
        if(space)
            space = false;
    }
}

void MenuState::update(cgf::Game* game)
{
}

void MenuState::draw(cgf::Game *game)
{
    if(oldMenu != currentMenu)
    {
        if (currentMenu==menu::STARTMENU)
        {
            menuText.load("data/img/titleOptions.png");
        }
        else
        {
            menuText.load("data/img/gameModes.png");
        }
        oldMenu = currentMenu;
    }
    menuSprite.setPosition(0,0);
    menuText.setPosition(0,0);
    if (currentMenu==menu::STARTMENU)
    {
        if (currentOption==startOptions::NEWGAME)
        {
            cursor.setPosition(400,300);
        }
        else
        {
            cursor.setPosition(400,360);
        }
    }
    else
    {
        switch(currentMode)
        {
            case gameMode::EASY:
                cursor.setPosition(400,267);
                break;
            case gameMode::MEDIUM: 
                cursor.setPosition(400,337);
                break;
            case gameMode::HARD:
                cursor.setPosition(400,407);
                break;
            default:
                break;

        }
    }

    sf::View originalView = game->getScreen()->getDefaultView();
    sf::View currentView = game->getScreen()->getView();
    game->getScreen()->setView(originalView);
    game->getScreen()->draw(menuSprite);
    game->getScreen()->draw(menuText);
    game->getScreen()->draw(cursor);
    game->getScreen()->setView(currentView);

}
