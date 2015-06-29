/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Game.h"
#include "PlayState.h"
#include "MenuState.h"
#include "InputManager.h"
#include "Collision.h"
#include <stdlib.h>
#ifndef DEBUG
#define DEBUG 0
#endif

PlayState PlayState::m_PlayState;

using namespace std;

void PlayState::init()
{
	quantidadeObjestos=4;
	tempoDeJogo=20;
	ended = 0;
	quantidadeObjetosEncontrados=0;
	tempoCorrido=0;
	
    if (!font.loadFromFile("data/fonts/arial.ttf")) {
        cout << "Cannot load arial.ttf font!" << endl;
        exit(1);
    }
    text.setFont(font);
    text.setCharacterSize(24); // in pixels
    text.setColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Bold);
    
    tempo.setPosition(0.0,32.0);
    tempo.setFont(font);
    tempo.setCharacterSize(24);
    tempo.setColor(sf::Color::Yellow);
    tempo.setStyle(sf::Text::Bold);
    map = new tmx::MapLoader("data/maps");       // all maps/tiles will be read from data/maps
    // map->AddSearchPath("data/maps/tilesets"); // e.g.: adding more search paths for tilesets
    map->Load("dungeon-tilesets2.tmx");
    createObjectives();

    walkStates[0] = "walk-right";
    walkStates[1] = "walk-left";
    walkStates[2] = "walk-up";
    walkStates[3] = "walk-down";
    currentDir = RIGHT;
    player.load("data/img/walterWhite.png",32,32,0,0,0,0,4,4,16);
    player.setPosition(40,100);
    player.loadAnimation("data/img/walterWhiteAnim.xml");
    player.setAnimation(walkStates[currentDir]);
    player.setAnimRate(10);
    player.setScale(1.5,1.5);
    player.play();

    visibleArea.load("data/img/visibility.png");
    visibleArea.setOrigin(800,600);
    visibleArea.setPosition(64,124);
    arrows = new cgf::Sprite[quantidadeObjestos];
    objectiveFound = new bool[quantidadeObjestos];
    for(int i = 0 ; i<quantidadeObjestos;i++){
		objectiveFound[i]=0;
		arrows[i].load("data/img/arrow.png");
		arrows[i].setOrigin(64,64);
		arrows[i].setPosition(player.getPosition().x+16, player.getPosition().y+16);
		arrows[i].setRotation(getArrowRotation(player.getPosition().x+16, player.getPosition().y+16, objectives[i].getPosition().x, objectives[i].getPosition().y));
	}

    objective.load("data/img/Char09.png");
    objective.setPosition(150,200);

    minimap.load("data/img/minimap_dungeon-tilesets2.png");
    minimap.setPosition(800-192, 0);

    pointMap.load("data/img/point-map.png");
    pointMap.setOrigin(194-800, 4);
    pointMap.setPosition((player.getPosition().x)/2048/192, (player.getPosition().y)/1024/96);

    dirx = 0; // sprite dir: right (1), left (-1)
    diry = 0; // down (1), up (-1)

    im = cgf::InputManager::instance();

    im->addKeyInput("left", sf::Keyboard::Left);
    im->addKeyInput("right", sf::Keyboard::Right);
    im->addKeyInput("up", sf::Keyboard::Up);
    im->addKeyInput("down", sf::Keyboard::Down);
    im->addKeyInput("escape", sf::Keyboard::Escape);
    im->addKeyInput("stats", sf::Keyboard::S);
    im->addMouseInput("rightclick", sf::Mouse::Right);

    // Camera control
    im->addKeyInput("zoomin", sf::Keyboard::Z);
    im->addKeyInput("zoomout", sf::Keyboard::X);

    cout << "PlayState: Init" << endl;
    time(&inicio);
}

void PlayState::cleanup()
{
    delete map;
    cout << "PlayState: Clean" << endl;
}

void PlayState::pause()
{
    cout << "PlayState: Paused" << endl;
}

void PlayState::resume()
{
    cout << "PlayState: Resumed" << endl;
}

void PlayState::handleEvents(cgf::Game* game)
{
    screen = game->getScreen();
    sf::View view = screen->getView(); // gets the view
    sf::Event event;

    while (screen->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            game->quit();
    }

    dirx = diry = 0;
    int newDir = currentDir;

    if(im->testEvent("left")) {
        dirx = -1;
        newDir = LEFT;
    }

    if(im->testEvent("right")) {
        dirx = 1;
        newDir = RIGHT;
    }

    if(im->testEvent("up")) {
        diry = -1;
        newDir = UP;
    }

    if(im->testEvent("down")) {
        diry = 1;
        newDir = DOWN;
    }

    if(im->testEvent("escape") || im->testEvent("rightclick"))
    {
        game->changeState(MenuState::instance());
        cout << "Going back to menu" << endl;
    }

    if(im->testEvent("stats"))
        game->toggleStats();

    if(im->testEvent("zoomin")) {
        view.zoom(1.01);
        screen->setView(view);
    }
    else if(im->testEvent("zoomout")) {
        view.zoom(0.99);
        screen->setView(view);
    }

    if(dirx == 0 && diry == 0) {
        player.pause();
    }
    else {
        if(currentDir != newDir) {
            player.setAnimation(walkStates[newDir]);
            currentDir = newDir;
        }
        player.play();
    }

    player.setXspeed(100*dirx);
    player.setYspeed(100*diry);
    sf::Vector2f playerPos = player.getPosition();
    visibleArea.setPosition(playerPos.x+24, playerPos.y+24);    
    for(int i = 0 ; i<quantidadeObjestos;i++){
		arrows[i].setPosition(player.getPosition().x+16, player.getPosition().y+16);
		arrows[i].setRotation(getArrowRotation(player.getPosition().x+16, player.getPosition().y+16, objectives[i].getPosition().x, objectives[i].getPosition().y));
	}
    pointMap.setPosition(playerPos.x/(2048/192), playerPos.y/(1024/96));

}

void PlayState::update(cgf::Game* game)
{
	//cout<<ended<<endl;
	if(ended){
		sf::Time delayTime = sf::milliseconds(2000);
		sf::sleep(delayTime);
        game->changeState(MenuState::instance());
	}
	if(tempoDeJogo<tempoCorrido){
		text.setString("Perdeu playboy!");
		tempo.setString("");
		ended=1;
	}
	if(quantidadeObjetosEncontrados==quantidadeObjestos){
		text.setString("You'r the one who knocks!");
		tempo.setString("");
		ended=1;
	}
    screen = game->getScreen();
    checkCollision(2, game, &player);
    centerMapOnPlayer();
}
bool PlayState::objFound(int objIndex){
	sf::FloatRect obj = objectives[objIndex].getLocalBounds();
	sf::FloatRect personagem = player.getLocalBounds();
	bool notInSameLine = obj.top+obj.height<personagem.top || obj.top>personagem.height+personagem.top;
	bool notInSameCollum = obj.left+obj.width<personagem.left || obj.left>personagem.width+personagem.left;
	if(!notInSameLine && !notInSameCollum)
	{
		printf("Personagem %d %d %d %d \n",personagem.width, personagem.left,personagem.height,personagem.top);
		printf("Objeto %d %d %d %d \n",obj.width, obj.left,obj.height,obj.top);
	}
	return !notInSameLine && !notInSameCollum;
}
void PlayState::draw(cgf::Game* game)
{
    screen = game->getScreen();
    map->Draw(*screen);          // draw all layers
//    map->Draw(*screen, 1);     // draw only the second layer
    screen->draw(player);
    quantidadeObjetosEncontrados = 0;
    for(int i = 0;i<quantidadeObjestos;i++){
		if(objectiveFound[i]|| player.bboxCollision(objectives[i])){
			objectiveFound[i]=true;
			quantidadeObjetosEncontrados++;
		}
		else{
			screen->draw(arrows[i]);
			screen->draw(objectives[i]);
		}
	}
	if(!ended){
		char buff[100];
		std::string buffAsStdStr;
		sprintf(buff, "Objetos faltando %d", quantidadeObjestos-quantidadeObjetosEncontrados);
		buffAsStdStr = buff;
		text.setString(buffAsStdStr);
		time_t timer;
		time(&timer);
		tempoCorrido = difftime(timer,inicio);
		sprintf(buff, "Tempo restante %.f", tempoDeJogo-tempoCorrido);
		buffAsStdStr = buff;
		tempo.setString(buffAsStdStr);	
	}
    screen->draw(visibleArea);
    sf::View originalView = screen->getDefaultView();
    sf::View currentView = screen->getView();
    screen->setView(originalView);
    screen->draw(minimap);
    screen->draw(pointMap); 
    screen->draw(text);
    screen->draw(tempo);
    screen->setView(currentView);
}

void PlayState::centerMapOnPlayer()
{
    sf::View view = screen->getView();
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2f viewsize = view.getSize();
    viewsize.x /= 2;
    viewsize.y /= 2;
    sf::Vector2f pos = player.getPosition();

    float panX = viewsize.x; // minimum pan
    if(pos.x >= viewsize.x)
        panX = pos.x;

    if(panX >= mapsize.x - viewsize.x)
        panX = mapsize.x - viewsize.x;

    float panY = viewsize.y; // minimum pan
    if(pos.y >= viewsize.y)
        panY = pos.y;

    if(panY >= mapsize.y - viewsize.y)
        panY = mapsize.y - viewsize.y;

    sf::Vector2f center(panX,panY);
    view.setCenter(center);
    screen->setView(view);
}

bool PlayState::checkCollision(uint8_t layer, cgf::Game* game, cgf::Sprite* obj)
{
    int i, x1, x2, y1, y2;
    bool bump = false;

    // Get the limits of the map
    sf::Vector2u mapsize = map->GetMapSize();
    // Get the width and height of a single tile
    sf::Vector2u tilesize = map->GetMapTileSize();

    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;
    mapsize.x--;
    mapsize.y--;

    // Get the height and width of the object (in this case, 100% of a tile)
    sf::Vector2u objsize = obj->getSize();
    objsize.x *= obj->getScale().x;
    objsize.y *= obj->getScale().y;

    float px = obj->getPosition().x;
    float py = obj->getPosition().y;

    double deltaTime = game->getUpdateInterval();

    sf::Vector2f offset(obj->getXspeed()/1000 * deltaTime, obj->getYspeed()/1000 * deltaTime);

    float vx = offset.x;
    float vy = offset.y;

    //cout << "px,py: " << px << " " << py << endl;

    //cout << "tilesize " << tilesize.x << " " << tilesize.y << endl;
    //cout << "mapsize " << mapsize.x << " " << mapsize.y << endl;

    // Test the horizontal movement first
    i = objsize.y > tilesize.y ? tilesize.y : objsize.y;

    for (;;)
    {
        x1 = (px + vx) / tilesize.x;
        x2 = (px + vx + objsize.x - 1) / tilesize.x;

        y1 = (py) / tilesize.y;
        y2 = (py + i - 1) / tilesize.y;

        if (x1 >= 0 && x2 < mapsize.x && y1 >= 0 && y2 < mapsize.y)
        {
            if (vx > 0)
            {
                // Trying to move right

                int upRight   = getCellFromMap(layer, x2*tilesize.x, y1*tilesize.y);
                int downRight = getCellFromMap(layer, x2*tilesize.x, y2*tilesize.y);
                if (upRight || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    px = x2 * tilesize.x;
                    px -= objsize.x;// + 1;
                    vx = 0;
                    bump = true;
                }
            }

            else if (vx < 0)
            {
                // Trying to move left

                int upLeft   = getCellFromMap(layer, x1*tilesize.x, y1*tilesize.y);
                int downLeft = getCellFromMap(layer, x1*tilesize.x, y2*tilesize.y);
                if (upLeft || downLeft)
                {
                    // Place the player as close to the solid tile as possible
                    px = (x1+1) * tilesize.x;
                    vx = 0;
                    bump = true;
                }
            }
        }

        if (i == objsize.y) // Checked player height with all tiles ?
        {
            break;
        }

        i += tilesize.y; // done, check next tile upwards

        if (i > objsize.y)
        {
            i = objsize.y;
        }
    }

    // Now test the vertical movement

    i = objsize.x > tilesize.x ? tilesize.x : objsize.x;

    for (;;)
    {
        x1 = (px / tilesize.x);
        x2 = ((px + i-1) / tilesize.x);

        y1 = ((py + vy) / tilesize.y);
        y2 = ((py + vy + objsize.y-1) / tilesize.y);

        if (x1 >= 0 && x2 < mapsize.x && y1 >= 0 && y2 < mapsize.y)
        {
            if (vy > 0)
            {
                // Trying to move down
                int downLeft  = getCellFromMap(layer, x1*tilesize.x, y2*tilesize.y);
                int downRight = getCellFromMap(layer, x2*tilesize.x, y2*tilesize.y);
                if (downLeft || downRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = y2 * tilesize.y;
                    py -= objsize.y;
                    vy = 0;
                    bump = true;
                }
            }

            else if (vy < 0)
            {
                // Trying to move up

                int upLeft  = getCellFromMap(layer, x1*tilesize.x, y1*tilesize.y);
                int upRight = getCellFromMap(layer, x2*tilesize.x, y1*tilesize.y);
                if (upLeft || upRight)
                {
                    // Place the player as close to the solid tile as possible
                    py = (y1 + 1) * tilesize.y;
                    vy = 0;
                    bump = true;
                }
            }
        }

        if (i == objsize.x)
        {
            break;
        }

        i += tilesize.x; // done, check next tile to the right

        if (i > objsize.x)
        {
            i = objsize.x;
        }
    }

    // Now apply the movement and animation

    obj->setPosition(px+vx,py+vy);
    px = obj->getPosition().x;
    py = obj->getPosition().y;

    obj->update(deltaTime, false); // only update animation

    // Check collision with edges of map
    if (px < 0)
        obj->setPosition(px,py);
    else if (px + objsize.x >= mapsize.x * tilesize.x)
        obj->setPosition(mapsize.x*tilesize.x - objsize.x - 1,py);

    if(py < 0)
        obj->setPosition(px,0);
    else if(py + objsize.y >= mapsize.y * tilesize.y)
        obj->setPosition(px, mapsize.y*tilesize.y - objsize.y - 1);

    return bump;
}

// Get a cell GID from the map (x and y in global coords)
sf::Uint16 PlayState::getCellFromMap(uint8_t layernum, float x, float y)
{
    auto layers = map->GetLayers();
    tmx::MapLayer& layer = layers[layernum];
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2u tilesize = map->GetMapTileSize();
    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;
    int col = floor(x / tilesize.x);
    int row = floor(y / tilesize.y);
    return layer.tiles[row*mapsize.x + col].gid;
}

// Calculates the angle to rotate the arrow
float PlayState::getArrowRotation(int px, int py, int ox, int oy)
{

    int signy = (oy-py > 0) - (oy-py < 0);
    float distance = sqrt(pow((ox-px),2) + pow((oy-py),2));
    float base = (ox-px);

    float angle = acos(base / distance) * 180 / PI * signy;
    return angle;
}

void PlayState::createObjectives(){
    auto layers = map->GetLayers();
    tmx::MapLayer& layer = layers[3];
    sf::Vector2u mapsize = map->GetMapSize();
    sf::Vector2u tilesize = map->GetMapTileSize();
    mapsize.x /= tilesize.x;
    mapsize.y /= tilesize.y;

    if(DEBUG)
    {
        cout << "mapsize.x=" << mapsize.x << " mapsize.y=" << mapsize.y << endl;
        cout << "tilesize.x=" << tilesize.x << " tilesize.y=" << tilesize.y << endl;
    }

    int x, y, k;
    k = 0;
    sf::Vector2u* possiblePositions = new sf::Vector2u[16];
    int* usedPositions = new int[16];
    for (y = 0; y < mapsize.y; ++y)
    {
        for (x = 0; x < mapsize.x; ++x)
        {
            if(layer.tiles[y*mapsize.x + x].gid)
            {
                sf::Vector2u pos;
                pos.x = x*tilesize.x;
                pos.y = y*tilesize.y;
                possiblePositions[k] = pos;
                usedPositions[k] = 0;
                ++k;
            }
        }
    }

    objectives = new cgf::Sprite[quantidadeObjestos];
    cgf::Sprite objective;
    for(int i = 0; i<quantidadeObjestos; i++){
        objective.load("data/img/Char09.png");
        int position = rand()%k;
        while(usedPositions[position])
            position = rand()%k;
        if(DEBUG)
            cout << position << endl;
        objective.setPosition(possiblePositions[position].x,possiblePositions[position].y);
        objectives[i]=objective;
        usedPositions[position] = 1;
    }

    /*
	objectives =  new cgf::Sprite[10];
	cgf::Sprite objective;
	for(int i = 0; i<10; i++){
		    objective.load("data/img/Char09.png");
			objective.setPosition(rand()%2000,rand()%1000);
			objectives[i]=objective;
			
	}
    */	
};
