#ifndef MENU_HPP_
#define MENU_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include <Core/Screen.h>

#define MAX_NUMBER_OF_ITEM 4

using namespace std;
using namespace sf;

class Menu : public Screen
{
    public:
		Menu(EventManager & eM,float width, float height);
		~Menu();

		void MoveUp(float width, float height);
		void MoveDown(float width, float height);
		void Select(sf::RenderWindow &window);
        virtual void onEvent(sf::Event & event);
        virtual void update(sf::Time &delta) ;


    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const  ;


    private:
		int selectedItemIndex;
		sf::Font font;
		sf::Text menu[MAX_NUMBER_OF_ITEM];
		sf::RectangleShape title;
		sf::Texture textureTitle;
		sf::Image imageTitle;
		sf::RectangleShape titleBorder;
		sf::Music buffer[6];

		int idChanson;
        string chanson[6];
        sf::Text infoChanson;


};

#endif // MENU_HPP_
