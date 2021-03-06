/*
 *  PlayState.h
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

#include "GameState.h"
#include "Sprite.h"
#include "InputManager.h"
#include <tmx/MapLoader.h>
#include <string>
#include <time.h> 

#define PI 3.14159265

class PlayState : public cgf::GameState
{
    public:

    void init();
    void cleanup();

    void pause();
    void resume();
	bool objFound(int objIndex);
    void handleEvents(cgf::Game* game);
    void update(cgf::Game* game);
    void draw(cgf::Game* game);

    // Implement Singleton Pattern
    static PlayState* instance()
    {
        return &m_PlayState;
    }

    protected:

    PlayState() {}

    private:

    static PlayState m_PlayState;
    
    enum { RIGHT=0, LEFT, UP, DOWN };
    std::string walkStates[4];
    int currentDir;
    bool ended;
    bool* objectiveFound;
	cgf::Sprite* objectives;
	cgf::Sprite* arrows;
    int x, y;
    int dirx, diry;
	int quantidadeObjestos;
	int quantidadeObjetosEncontrados;
	int tempoDeJogo;
	double tempoCorrido;
    cgf::Sprite player;
    cgf::Sprite visibleArea;
    cgf::Sprite arrow;
    cgf::Sprite objective;
    cgf::Sprite minimap;
    cgf::Sprite pointMap;

    sf::RenderWindow* screen;
    cgf::InputManager* im;

    tmx::MapLoader* map;

    sf::Font font;
    sf::Text text;
    sf::Text tempo;
	time_t inicio;
    // Centers the camera on the player position
    void centerMapOnPlayer();

    // Checks collision between a sprite and a map layer
    bool checkCollision(uint8_t layer, cgf::Game* game, cgf::Sprite* obj);

    // get a cell GID from the map (x and y in world coords)
    sf::Uint16 getCellFromMap(uint8_t layernum, float x, float y);

    // Calculates the angle to rotate the arrow
    float getArrowRotation(int px, int py, int ox, int oy);
    
    void createObjectives();
};

#endif
