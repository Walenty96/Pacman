#include "Joueur.hpp"
#include "Core/Ressource.hpp"
#include "Game.h"
#include "map.hpp"
#include "Core/EventManager.hpp"
#include "Fantome.hpp"
#include <iostream>

#define VITESSE_PACMAN 70.f //pixel par secondes sans boost
#define VITESSE_PACMAN_BOOST 120.f //pixel par secondes avec boost

Joueur::Joueur(EventManager & eM,Game & g,Map & m):_game(g),_map(m),eventManager(eM)
{
    for(int i=0;i<4;i++){
        sprite_pacman[i][0]=Ressource::getSprite("pacman.png",IntRect(0,i*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
        sprite_pacman[i][1]=Ressource::getSprite("pacman.png",IntRect(TAILLE_CASE,i*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
        sprite_pacman[i][2]=Ressource::getSprite("pacman.png",IntRect(TAILLE_CASE*2,0,TAILLE_CASE,TAILLE_CASE));
    }

    superPacman = false; //boost ou non


    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);

    startPosition=Vector2i(1,1);


    position =Vector2f(startPosition.x*TAILLE_CASE,startPosition.y*TAILLE_CASE);
     target=startPosition;

    life=3;
    points=0;
    palierPoints=5000;


}

void Joueur::update(Time delta)
{

    timer_marche+=delta;
    if(!superPacman)
        position+=VITESSE_PACMAN*Vector2f(directionActuelle.x,directionActuelle.y)*delta.asSeconds();
    else
        position+=VITESSE_PACMAN_BOOST*Vector2f(directionActuelle.x,directionActuelle.y)*delta.asSeconds();

    if(directionActuelle==Vector2i(0,0)){//Si il n'avance pas
        nextTarget();//On essais de passer à la prochaine direction voulue
    }else{

        Vector2f diff=position-Vector2f(target.x*TAILLE_CASE,target.y*TAILLE_CASE);
        float distActuelle = diff.x*diff.x+diff.y*diff.y;
                 diff=(position+Vector2f(directionActuelle.x,directionActuelle.y))-Vector2f(target.x*TAILLE_CASE,target.y*TAILLE_CASE);
        float distSuivante = diff.x*diff.x+diff.y*diff.y;
        if(distActuelle<distSuivante)//Si on s'éloigne de la cible
        {
            nextTarget();//On passe à la suivante
        }
    }
    vector<Fantome> & ghosts(_game.getFantomes());
    for(Fantome & f:ghosts){

        if(f.getHitBox().intersects(getHitBox())){
            if(superPacman && f.isAfraid()){
                f.die();
                addPoints(500);
            }else{
                die();
            }
        }
    }
    updateGraphics(delta);
}

void Joueur::nextTarget()
{
    walkOnTarget();
    position=Vector2f(target.x*TAILLE_CASE,target.y*TAILLE_CASE);
    if(_map.playerCanWalkOn(target+directionSuivante)){
        directionActuelle=directionSuivante;

    }else if(!_map.playerCanWalkOn(target+directionActuelle)){
        directionActuelle=Vector2i(0,0);
    }
    target += directionActuelle;
}

void Joueur::walkOnTarget(){
    if      (_map.bonusOn(target)){
        _map.takeBonus(target);
        _game.notifyPacgumOn();
        //notifier map
    }else if(_map.coinOn(target)){
        _map.takeCoin(target);
        addPoints(10);
        //score à faire
    }else if(_map.fruitOn(target)){
        addPoints(_map.takeFruit(target));
        //score à faire
    }
}


void Joueur::updateGraphics(Time delta)
{
    if(timer_marche.asSeconds()>0.15){
        timer_marche=seconds(0);
        etape_marche=(etape_marche+1)%4;
    }
}


void Joueur::onEvent(sf::Event& event)
{
    if(event.type==sf::Event::KeyPressed){
        if      (eventManager.getEventKey("Gauche")==event.key.code){
            directionSuivante=Vector2i(-1,0);
        }else if(eventManager.getEventKey("Droite")==event.key.code){
            directionSuivante=Vector2i(1,0);
        }else if(eventManager.getEventKey("Bas")   ==event.key.code){
            directionSuivante=Vector2i(0,1);
        }else if(eventManager.getEventKey("Haut")  ==event.key.code){
            directionSuivante=Vector2i(0,-1);
        }
    }
}

void Joueur::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    int direction=-1;

    if      (directionActuelle.x<0){
        direction=0;
    }else if(directionActuelle.x>0){
        direction=1;
    }else if(directionActuelle.y<0){
        direction=2;
    }else if(directionActuelle.y>0){
        direction=3;
    }

    Sprite tmp_sprite;

    if(direction==-1){ // Si il n'avance pas, on affiche simplement le sprite où il regare vers le haut
        tmp_sprite=sprite_pacman[2][0];
    }else{
        int graphicEtape=0;
        if(etape_marche==0||etape_marche==2){
            graphicEtape=0;
        }else if(etape_marche==1){
            graphicEtape=1;
        }else if(etape_marche==3){
            graphicEtape=2;
        }
        tmp_sprite=sprite_pacman[direction][graphicEtape];
    }
    tmp_sprite.setPosition(position);

    target.draw(tmp_sprite,states);
}


void Joueur::notifyPacgumOn()
{
    superPacman = true;
}

void Joueur::notifyPacgumOff()
{
    superPacman = false;
}

FloatRect Joueur::getHitBox(){
    return FloatRect(position.x+5,position.y+5,TAILLE_CASE-10,TAILLE_CASE-10);
}

void Joueur::die()
{
    setLife(-1);
    superPacman = false; //boost ou non
    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);
    fatality.openFromFile("res/sounds/fatality.ogg");
    fatality.play();
    srand(time(NULL));
    posRandom=rand()%4;
    switch(posRandom){
        case 0:
            startPosition=Vector2i(1,1);

            break;
        case 1:
            startPosition=Vector2i(1,_map.getSize().y-2);
            break;
        case 2:
            startPosition=Vector2i(_map.getSize().x-2,1);
            break;
        case 3:
            startPosition=Vector2i(_map.getSize().x-2,_map.getSize().y-2);
            break;
    }
     position=Vector2f(startPosition.x*TAILLE_CASE,startPosition.y*TAILLE_CASE);
     target=startPosition;
     addPoints(-200);


}

int Joueur::getLife()
{
    return life;

}
int Joueur::getPoints()
{

    return points;

}
void Joueur::setLife(int addLife)
{
    life+=addLife;

    if (life<=0){
        life=0;
         _game.notifyLose();
    }
}

void Joueur::addPoints(int addPoints)
{
    points+=addPoints;
    if(points>=palierPoints)
    {
        palierPoints+=5000;
        life++;

    }
    if (points<0)
        points=0;



}


void Joueur::setPosition()
{
    position=Vector2f(TAILLE_CASE,TAILLE_CASE);
    target=Vector2i(1,1);
    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);

}


void Joueur::reinit()
{
    superPacman = false; //boost ou non


    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);

    startPosition=Vector2i(1,1);


    position =Vector2f(startPosition.x*TAILLE_CASE,startPosition.y*TAILLE_CASE);
     target=startPosition;

    life=3;
    points=0;
    palierPoints=5000;
}

