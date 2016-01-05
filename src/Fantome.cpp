#include "Fantome.hpp"
#include "Game.h"
#include "map.hpp"
#include "Core/Ressource.hpp"
#include <iostream>

Fantome::Fantome(Game& g, Map& m, Vector2i _p, int type):_game(g),_map(m)
{
    for(int i=0;i<4;i++){
        sprite_ghost[i][0]=Ressource::getSprite("ghosts.png",IntRect(i*2*TAILLE_CASE,type*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
        sprite_ghost[i][1]=Ressource::getSprite("ghosts.png",IntRect((i*2+1)*TAILLE_CASE,type*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
    }
    sprite_ghost[4][0]=Ressource::getSprite("ghosts.png",IntRect(0,4*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
    sprite_ghost[4][1]=Ressource::getSprite("ghosts.png",IntRect(TAILLE_CASE,4*TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));

    afraidGhost = false; //boost ou non

    position=Vector2f(_p.x*TAILLE_CASE,_p.y*TAILLE_CASE);
    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);
    target=_p;
    startPosition=_p;

    etape_marche=0; // Quel sprite d'animation du fantome
    ghostDeath=shared_ptr<sf::Music>(new Music());
    ghostDeath->openFromFile("res/sounds/ghostbuster.ogg");



}

void Fantome::update(Time delta)
{
    timer_marche+=delta;
    if(!afraidGhost)
        position+=VITESSE_GHOST*Vector2f(directionActuelle.x,directionActuelle.y)*delta.asSeconds();
    else
        position+=VITESSE_GHOST_AFRAID*Vector2f(directionActuelle.x,directionActuelle.y)*delta.asSeconds();

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

    updateGraphics(delta);
}

void Fantome::updateGraphics(Time delta)
{
    if(timer_marche.asSeconds()>0.15){
        timer_marche=seconds(0);
        etape_marche=(etape_marche+1)%2;
    }
}

void Fantome::notifyPacgumOn()
{
    afraidGhost=true;
}

void Fantome::notifyPacgumOff()
{
    afraidGhost = false;
}

void Fantome::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    int direction=-1;
    if      (afraidGhost){
        direction=4;
    }else if(directionActuelle.y<0){
        direction=0;
    }else if(directionActuelle.y>0){
        direction=1;
    }else if(directionActuelle.x<0){
        direction=2;
    }else if(directionActuelle.x>0){
        direction=3;
    }

    Sprite tmp_sprite;

    if(direction==-1){ // Si il n'avance pas et n'est pas effrayé, on affiche simplement le sprite où il regare vers le haut
        tmp_sprite=sprite_ghost[0][0];
    }else{
        tmp_sprite=sprite_ghost[direction][etape_marche];
    }
    tmp_sprite.setPosition(position);

    target.draw(tmp_sprite,states);
}

void Fantome::nextTarget()
{
    vector<Vector2i> possibleDirection;
    Vector2i allDirection[4]={Vector2i(0,1),Vector2i(0,-1),Vector2i(1,0),Vector2i(-1,0)};
    for(int i=0;i<4;i++){
        if(allDirection[i]!=directionActuelle*-1 && _map.ghostCanWalkOn(target+allDirection[i])){ //pas de demi-tour
            possibleDirection.push_back(allDirection[i]);
        }
    }
    if(possibleDirection.size()>0){
        directionSuivante=possibleDirection[rand()%possibleDirection.size()];
    }else{
        if(_map.ghostCanWalkOn(target+(directionActuelle*-1))){
            directionSuivante=directionActuelle*-1;
        }else{
            directionSuivante=Vector2i(0,0);
        }
    }

    position=Vector2f(target.x*TAILLE_CASE,target.y*TAILLE_CASE);
    if(_map.ghostCanWalkOn(target+directionSuivante)){
        directionActuelle=directionSuivante;

    }else if(!_map.ghostCanWalkOn(target+directionActuelle)){
        directionActuelle=Vector2i(0,0);
    }
    target += directionActuelle;
}

bool Fantome::isAfraid(){
    return afraidGhost;
}

void Fantome::die()
{


    ghostDeath->play();
    afraidGhost = false;

    position=Vector2f(startPosition.x*TAILLE_CASE,startPosition.y*TAILLE_CASE);
    directionActuelle=Vector2i(0,0);
    directionSuivante=Vector2i(0,0);

    target=startPosition;

    etape_marche=0;
}


FloatRect Fantome::getHitBox(){
    return FloatRect(position.x+5,position.y+5,TAILLE_CASE-10,TAILLE_CASE-10);
}

