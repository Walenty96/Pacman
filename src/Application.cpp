#include "menu.hpp"
#include "Core/Ressource.hpp"
#include <iostream>
#include "map.hpp"
#include "Application.hpp"
#include "menu.hpp"
#include "Core/ScreenManager.h"
#include "Game.h"

Application::Application():
window(sf::VideoMode(1024,768),"Pacman")
{
    ScreenManager::add(new Menu(eventManager,1024,768),"menu");
    ScreenManager::goToScreen("menu");
    ScreenManager::add(new Game(eventManager),"game");
    adjustView(Vector2i(1024,768));
}
void Application::run()
{
        Clock delta;
		do
		{
			processEvent();
			update(delta.restart());
			draw();
		}while(window.isOpen());
}


void Application::draw()
{
    View lastView=window.getView();
    window.setView(vueGenerale);

	window.clear(sf::Color::Black);
    window.draw(*(ScreenManager::screeCourant));
    window.display();

    window.setView(lastView);
}

void Application::processEvent()
{




        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == Event::Resized)
                adjustView(Vector2i(event.size.width, event.size.height));

                ScreenManager::screeCourant->onEvent(event);
        }


}

void Application::update(Time delta)
{
    ScreenManager::screeCourant->update(delta);



	}
void Application::adjustView(Vector2i screenSize){
    Vector2i sizeOfView = Vector2i(1024,768);
    vueGenerale.reset(FloatRect(0,0,sizeOfView.x,sizeOfView.y));
    Vector2f ratio2f = Vector2f(screenSize.x/float(sizeOfView.x),screenSize.y/float(sizeOfView.y));

    if(ratio2f.x<ratio2f.y){
        float margeHaut = (1-((sizeOfView.y*ratio2f.x)/screenSize.y))/2.0;
        vueGenerale.setViewport(FloatRect(0.f,margeHaut,1.f,1.f-margeHaut*2));
    }else{
        float margeHaut = (1-((sizeOfView.x*ratio2f.y)/screenSize.x))/2.0;
        vueGenerale.setViewport(FloatRect(margeHaut,0,1.f-margeHaut*2,1.f));
    }
}

