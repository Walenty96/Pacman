#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include "menu.hpp"
#include "Core/EventManager.hpp"


using namespace std;
using namespace sf;



class Application
{
    public:
		Application();
		void processEvent();
		void run();
		void draw();
		void update(Time delta);



    protected:


    private:
     void adjustView(Vector2i screenSize);
     sf::RenderWindow window;
     sf::RenderWindow jeu;
     sf::View vueGenerale;
	 bool MenuClosed=false;
	 EventManager eventManager;






};

#endif // MAP_HPP_
