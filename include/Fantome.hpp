#ifndef _FANTOME_HPP_
#define _FANTOME_HPP_

#include <SFML/Graphics.hpp>
#include "Joueur.hpp"
#include <memory>

#define VITESSE_GHOST 70.f
#define VITESSE_GHOST_AFRAID 50.f

class Game;
class Map;

using namespace sf;
using namespace std;

class Fantome : public Drawable
{
    public:
        Fantome(Game & g,Map & m,Vector2i position,int num);

		void update(Time delta);
		void updateGraphics(Time delta);

        void notifyPacgumOn();
        void notifyPacgumOff();

        bool isAfraid();

        FloatRect getHitBox();
        void die();

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const ;
        void nextTarget();

    private:
        Game & _game;
        Map & _map;

        //logique
        Vector2i target;
        Vector2i startPosition;

        Vector2i directionActuelle;
        Vector2i directionSuivante;

        bool afraidGhost;

        //graphique et logique
        Vector2f position;

        //graphique
        Sprite sprite_ghost[5][2]; //4 directions, mais la 5ème est pour le fantôme effrayé
        Time timer_marche;
        int etape_marche;
        //son
        shared_ptr <sf::Music> ghostDeath;

};

#endif // _FANTOME_HPP_
