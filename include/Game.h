#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <Core/Screen.h>
#include <SFML/Audio.hpp>
#include <string.h>
#include "Joueur.hpp"
#include "Fantome.hpp"
#include "map.hpp"

class Map;
class Game: public Screen
{
    public:
        int x,y;
        Game(EventManager & eM);
        virtual void onEvent(sf::Event & event);
        void update(sf::Time &time);
        void notifyPacgumOn();
        void notifyPacgumOff();
        void adjustView(Vector2i screenSize=Vector2i(1024,768));
        vector<Fantome> & getFantomes();
        void notifyLose();
        void notifyWin();
        void nextMap();
        void looseFocus();
        sf::RectangleShape bgEcranWin;
        sf::Text youWin;
        sf::Text youLose;
        bool victoire;
        bool perdu;

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        Vector2i _sizeScreen;

        Map _map;
        Joueur _player;
        vector<Fantome> _ghosts;

        View vueGame;
        Time timer_pacgum;
        bool pacgum_on;
        int nbVies;
        sf::Text vie;
        int nbPoints;
        sf::Text point;
        int numMap;
        vector<string> nomMaps;


    private:

};

#endif // GAME_H
