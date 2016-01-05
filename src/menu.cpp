#include "menu.hpp"
#include "Core/Ressource.hpp"
#include "Core/ScreenManager.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace sf;



Menu::Menu(EventManager & eM,float width, float height):Screen(eM)
{

    for (int i=0 ; i<6 ;i++){
        switch(i){
        case 0:
            buffer[i].openFromFile("res/sounds/Ylvis.ogg");
            chanson[i]="Chanson en cours ->Ylvis : The fox 8bits version";
            break;
        case 1:
            buffer[i].openFromFile("res/sounds/cena.ogg");
            chanson[i]="Chanson en cours ->John Cena : My time is now 8 bits version ";
            break;
        case 2:
            buffer[i].openFromFile("res/sounds/funk.ogg");
            chanson[i]="Chanson en cours ->Mark Ronson : Uptown Funk 8 bits version ";
            break;
        case 3:
            buffer[i].openFromFile("res/sounds/lmfao.ogg");
            chanson[i]="Chanson en cours ->LMFAO : Party rock anthem 8 bits version ";
            break;
        case 4:
            buffer[i].openFromFile("res/sounds/timber.ogg");
            chanson[i]="Chanson en cours ->Pitbull ft Keisha : Timber 8 bits version ";
            break;
        case 5:
            buffer[i].openFromFile("res/sounds/clean.ogg");
             chanson[i]="Chanson en cours ->Clean Bandit : Rather Be 8 bits version ";
            break;
        }
    }
    srand(time(NULL));
    idChanson=rand()%6;
    buffer[idChanson].play();

    textureTitle.loadFromFile("res/img/title.gif");
    title.setTexture(&textureTitle);
    title.setSize((sf::Vector2f(600,272)));
    title.setOrigin(width/2,(height-(height-50)));
    title.setPosition(width/1.4,(height-(height-50)));

    //titleBorder.setOrigin(width/2,(height-(height-50)));
    titleBorder.setSize((sf::Vector2f(150,40)));
    titleBorder.setOutlineThickness(2);
    titleBorder.setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*2));
    titleBorder.setFillColor(sf::Color::Transparent);
    titleBorder.setOutlineColor(sf::Color::Green);



    menu[0].setFont(Ressource::getFont("pacman.ttf"));
    menu[0].setColor(sf::Color::Yellow);
    menu[0].setString("Play");
    menu[0].setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*2));

    menu[1].setFont(Ressource::getFont("pacman.ttf"));
    menu[1].setColor(sf::Color::Red);
    menu[1].setString("Options");
    menu[1].setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*3));

    menu[2].setFont(Ressource::getFont("pacman.ttf"));
    menu[2].setColor(sf::Color::Blue);
    menu[2].setString("Exit");
    menu[2].setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*4));

    infoChanson.setFont(Ressource::getFont("default.ttf"));
    infoChanson.setColor(sf::Color::White);
    infoChanson.setString(chanson[idChanson]);
    infoChanson.setPosition(30,700);
    infoChanson.setCharacterSize(18);

    selectedItemIndex=1;


}

Menu::~Menu()
{

}


void Menu::MoveUp(float width, float height)
{

    if(selectedItemIndex-1==0)
    {
        selectedItemIndex=MAX_NUMBER_OF_ITEM-1;
    }

    else selectedItemIndex--;
    if(selectedItemIndex==0)
    {
        selectedItemIndex=1;
    }


    if (selectedItemIndex==2)
    {
        titleBorder.setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*(selectedItemIndex+1)));
        titleBorder.setSize((sf::Vector2f(225,40)));
    }
    else
    {
        titleBorder.setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*(selectedItemIndex+1)));
        titleBorder.setSize((sf::Vector2f(150,40)));
    }

}





void Menu::MoveDown(float width, float height)
{

    if(selectedItemIndex+1>MAX_NUMBER_OF_ITEM-1)
    {
        selectedItemIndex=0;
    }

    else selectedItemIndex++;
    if(selectedItemIndex==0)
    {
        selectedItemIndex=1;
    }

    if (selectedItemIndex==2)
    {
        titleBorder.setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*(selectedItemIndex+1)));
        titleBorder.setSize((sf::Vector2f(225,40)));
    }
    else
    {
        titleBorder.setSize((sf::Vector2f(150,40)));
        titleBorder.setPosition(sf::Vector2f(width/2.3,height/(MAX_NUMBER_OF_ITEM+1)*(selectedItemIndex+1)));
    }
}

void Menu::Select(sf::RenderWindow &window)
{

    switch(selectedItemIndex)
    {
    case 1:
        window.clear(sf::Color::Black);




    }

}


void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(title,states);
    target.draw(titleBorder,states);
    for (int i=0 ; i<MAX_NUMBER_OF_ITEM ; i++ )
    {

        target.draw(menu[i],states);

    }
    target.draw(infoChanson,states);
}


void Menu::onEvent(sf::Event& event)
{
    switch(event.type){
        case Event::KeyPressed:
            switch(event.key.code){
                case Keyboard::Up:
                    MoveUp(1024,768);
                break;
                case Keyboard::Down:
                    MoveDown(1024,768);
                break;
                case Keyboard::Return:
                    switch(selectedItemIndex)
                    {
                        case 1:
                        ScreenManager::goToScreen("game");
                        buffer[idChanson].setVolume(30);
                        break;
                        case 2:
                            //Option
                        break;
                        case 3:
                            //exit


                        break;

                    }

                break;
                case Keyboard::F5:
                    buffer[idChanson].stop();
                    idChanson--;
                    if (idChanson<0)
                        idChanson=5;
                    buffer[idChanson].play();
                    break;

                case Keyboard::F6:
                 buffer[idChanson].stop();
                    idChanson++;
                    if (idChanson>5)
                        idChanson=0;
                    buffer[idChanson].play();
                    break;

                case Keyboard::F8:
                    buffer[idChanson].pause();
                    break;

                case Keyboard::F7:
                    buffer[idChanson].play();

                    break;
                default:
                break;
            }
        break;
        default:
        break;

    }
}

void Menu::update(sf::Time &delta)
{
    infoChanson.setString(chanson[idChanson]);
    infoChanson.setPosition(infoChanson.getPosition()+(Vector2f(200,0)*delta.asSeconds()));
    if(infoChanson.getPosition().x>1024)
        infoChanson.setPosition(-infoChanson.getGlobalBounds().width,700);


}




