#ifndef _JOUEUR_HPP_
#define _JOUEUR_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Core/EventManager.hpp"
class Map;
class Game;

using namespace sf;
using namespace std;

class Joueur : public Drawable
{
    public:
        Joueur(EventManager & eM,Game & g,Map & m);
		void update(Time delta);
		void updateGraphics(Time delta);
        void onEvent(sf::Event & event);
        void notifyPacgumOn();
        void notifyPacgumOff();
        void die();
        void reinit();
        void setPosition();
        FloatRect getHitBox();
        int getLife();
        int getPoints();

        void setLife(int addLife);
        void addPoints(int addLife);


    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const  ;
        void nextTarget();
        void walkOnTarget();

    private:
        int life;
        int points;
        int posRandom;
        int palierPoints;
        Game & _game;
        Map & _map;


        //logique
        Vector2i target;
        Vector2i startPosition;
        Vector2i directionActuelle;
        Vector2i directionSuivante;

        bool superPacman;

        //graphique et logique
        Vector2f position;

        //graphique
        Sprite sprite_pacman[4][3];
        Time timer_marche;
        int etape_marche;
        sf::Music fatality;
        EventManager & eventManager;
};

#endif // _JOUEUR_HPP_
