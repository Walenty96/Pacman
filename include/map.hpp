#ifndef MAP_HPP_
#define MAP_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>

#define TAILLE_CASE 32

//1 : MUR
//2 : SOL AVEC PIECE
//3 : PORTE FANTOME
//4 : MUR FANTOME
//5 : SOL SANS PIECE
//6 : PAC GUM (=Bonus)
//7 : FRUIT

using namespace std;
using namespace sf;

class Game;

class Map : public Drawable
{
    public:

		Map(Game & game);

		void update(Time delta);

        void loadFromFile(string file);

        bool playerCanWalkOn(int x, int y) const;
        bool playerCanWalkOn(Vector2i position) const;

        bool ghostCanWalkOn(int x, int y) const;
        bool ghostCanWalkOn(Vector2i position) const;

        bool coinOn(int x, int y) const;
        bool coinOn(Vector2i position) const;

        bool takeCoin(int x, int y);
        bool takeCoin(Vector2i position);

        bool bonusOn(int x, int y) const;
        bool bonusOn(Vector2i position) const;

        bool takeBonus(int x, int y);
        bool takeBonus(Vector2i position);

        bool fruitOn(int x, int y) const;
        bool fruitOn(Vector2i position) const;

        int takeFruit(int x, int y);
        int takeFruit(Vector2i position);

        int getNbGum();
        void addGum(int nb);

        Vector2i getSize() const;
        vector<sf::Vector2i> getPositionsFantomes();

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const  ;

        void updateFruit(Time delta);

    private:
		vector<vector<int>> cases;
		vector<sf::Vector2i> positionsFantomes;
		Sprite cases_sprite[7];
		Sprite coins_sprite;
		Sprite pacgum_sprite;
		Sprite fruit_sprite;

		Vector2i positionFruit;
        int pointFruit;
        int nbGum;
        int etape_fruit;
        Time time_fruit;
        bool need_fruit;
        int life;
        Game & _game;

};

#endif // MAP_HPP_
