#include "Game.h"
#include "Core/Ressource.hpp"

#include "Core/ScreenManager.h"
#include "map.hpp"
#define TIME_OF_PACGUM 8.f
#include <iostream>
#include "Core/functions/fn_string.hpp"
#include <string>
Game::Game(EventManager & eM):Screen(eM),_map(*this),_player(eM,*this,_map)
{
    _sizeScreen=Vector2i(1024,768);
    numMap=0;
    nomMaps.push_back("Map.txt");
    nomMaps.push_back("Map2.txt");
    nomMaps.push_back("Map3.txt");

    _map.loadFromFile(nomMaps[numMap]);
    vector<Vector2i> tmp_positions_fantomes=_map.getPositionsFantomes();
    if(tmp_positions_fantomes.size()>=4)
    for(int i=0;i<4;i++){
        _ghosts.push_back(Fantome(*this,_map,tmp_positions_fantomes[i],i));
    }
    adjustView();
    nbVies=_player.getLife();
    nbPoints=_player.getPoints();

    vie.setString("Vies : "+to_string(nbVies));
    vie.setPosition(30,725);
    vie.setFont(Ressource::getFont("default.ttf"));
    vie.setColor(sf::Color::White);


    point.setString("points : "+to_string(nbPoints));
    point.setPosition(200,725);
    point.setFont(Ressource::getFont("default.ttf"));
    point.setColor(sf::Color::White);

    bgEcranWin.setSize(Vector2f(1024,768));
    bgEcranWin.setPosition(0,0);
    bgEcranWin.setFillColor(sf::Color(0,0,0,200));

    youWin.setString("YOU WIN");
    youWin.setColor(sf::Color::Red);
    youWin.setFont(Ressource::getFont("pacman.ttf"));
    youWin.setCharacterSize(40);
    youWin.setPosition(1024/2-youWin.getGlobalBounds().width/2,768/2-youWin.getGlobalBounds().height/2);

    youLose.setString("YOU LOSE");
    youLose.setColor(sf::Color::Green);
    youLose.setFont(Ressource::getFont("pacman.ttf"));
    youLose.setCharacterSize(40);
    youLose.setPosition(1024/2-youWin.getGlobalBounds().width/2,768/2-youWin.getGlobalBounds().height/2);



    victoire=false;
    perdu=false;



}

void Game::adjustView(Vector2i screenSize){
    Vector2i sizeOfView = _map.getSize()*TAILLE_CASE;
    vueGame.reset(FloatRect(0,0,sizeOfView.x,sizeOfView.y));
    Vector2f ratio2f = Vector2f(screenSize.x/float(sizeOfView.x),screenSize.y/float(sizeOfView.y));

    if(ratio2f.x<ratio2f.y){
        float margeHaut = (1-((sizeOfView.y*ratio2f.x)/screenSize.y))/2.0;
        vueGame.setViewport(FloatRect(0.f,margeHaut,1.f,1.f-margeHaut*2));
    }else{
        float margeHaut = (1-((sizeOfView.x*ratio2f.y)/screenSize.x))/2.0;
        vueGame.setViewport(FloatRect(margeHaut,0,1.f-margeHaut*2,1.f));
    }

}

void Game::onEvent(sf::Event & event){
    if(event.type == Event::Resized){
    adjustView(Vector2i(event.size.width, event.size.height));
    _sizeScreen=Vector2i(event.size.width, event.size.height);
    }
    if(event.type==Event::KeyPressed)
    if(event.key.code==Keyboard::Escape)
        ScreenManager::goToScreen("menu");
    else if(event.key.code == Keyboard::F1){
        numMap=-1;
        nextMap();
    }else if(event.key.code == Keyboard::F2){
        numMap=0;
        nextMap();
    }else if(event.key.code == Keyboard::F3){
        numMap=1;
        nextMap();
    }else if(event.key.code == Keyboard::F12){
        vector<Vector2i> tmp_positions_fantomes=_map.getPositionsFantomes();
        if(tmp_positions_fantomes.size()>=4)
        for(int i=0;i<4;i++){
            _ghosts.push_back(Fantome(*this,_map,tmp_positions_fantomes[i],i));
        }
    }
    _player.onEvent(event);
}

void Game::update(sf::Time & time){
    nbVies=_player.getLife();
    nbPoints=_player.getPoints();
    if(!victoire&&!perdu){
    _map.update(time);
    for(Fantome & g:_ghosts){
        g.update(time);
    }

    _player.update(time);
    if(pacgum_on){
        timer_pacgum+=time;
        if(timer_pacgum.asSeconds()>TIME_OF_PACGUM){
            notifyPacgumOff();
        }
    }
    }
    vie.setString("Vies : "+to_string(nbVies));
    point.setString("Points : "+to_string(nbPoints));
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states)const {
    View lastView=target.getView();

    target.setView(vueGame);
    target.draw(_map,states);


    for(Fantome g:_ghosts){
        target.draw(g,states);
    }
    target.draw(_player,states);

    target.setView(lastView);

    target.draw(vie,states);
    target.draw(point,states);
    if (victoire==true){


                target.draw(bgEcranWin,states);
                target.draw(youWin,states);

    }
    if(perdu==true){
            target.draw(bgEcranWin,states);
            target.draw(youLose,states);

    }

}

vector<Fantome> & Game::getFantomes(){

    return _ghosts;
}

void Game::notifyPacgumOn(){
    timer_pacgum=seconds(0);
    _player.notifyPacgumOn();

    for(Fantome & g:_ghosts){
        g.notifyPacgumOn();
    }
    pacgum_on = true;

}

void Game::notifyPacgumOff(){
    _player.notifyPacgumOff();
    for(Fantome & g:_ghosts){
        g.notifyPacgumOff();
    }
    pacgum_on = false;
}

void Game::notifyWin()
{


    nextMap();



}

void Game::notifyLose()
{

    if(_player.getLife()<=0){
        perdu=true;
    }

}

void Game::nextMap()
{
    numMap++;
    if(numMap<=nomMaps.size()-1){
        _map.loadFromFile(nomMaps[numMap]);
        _player.setPosition();
        adjustView(_sizeScreen);
        _ghosts.clear();
        vector<Vector2i> tmp_positions_fantomes=_map.getPositionsFantomes();
        if(tmp_positions_fantomes.size()>=4)
        for(int i=0;i<4;i++){
            _ghosts.push_back(Fantome(*this,_map,tmp_positions_fantomes[i],i));
        }
    }
    else victoire=true;


}
void Game::looseFocus()
{
    if(victoire||perdu){
        victoire=false;
        perdu=false;

        numMap=0;
        _map.loadFromFile(nomMaps[numMap]);
        _ghosts.clear();
        vector<Vector2i> tmp_positions_fantomes=_map.getPositionsFantomes();
        if(tmp_positions_fantomes.size()>=4)
        for(int i=0;i<4;i++){
            _ghosts.push_back(Fantome(*this,_map,tmp_positions_fantomes[i],i));
        }
        _player.reinit();
        adjustView(_sizeScreen);


    }


}
