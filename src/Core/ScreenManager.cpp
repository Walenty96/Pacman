#include "Core/ScreenManager.h"

map <string,Screen*> ScreenManager::_screen;

Screen* ScreenManager::screeCourant=nullptr;

void ScreenManager::goToScreen(string str)
{

    if(_screen.find(str)!=_screen.end()){
        if(screeCourant!=nullptr)
            screeCourant->looseFocus();
        screeCourant=_screen[str];
        screeCourant->gainFocus();
    }
}

void ScreenManager::add(Screen* s,string str){
    _screen[str]=s;
}


