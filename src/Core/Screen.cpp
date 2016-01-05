#include "Core/Screen.h"


Screen::Screen(EventManager& eM):eventManager(eM){}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const {}
void Screen::onEvent(sf::Event & event){}
void Screen::update(sf::Time& tps){}
void Screen::gainFocus(){}

void Screen::looseFocus(){}
