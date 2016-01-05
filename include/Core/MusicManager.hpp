#ifndef _MUSIC_MANAGER_HPP_
#define _MUSIC_MANAGER_HPP_
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <list>
#include "Core/Ressource.hpp"

using namespace std;
using namespace sf;

class MusicManager{
private:
    vector<shared_ptr<Music>> musics;
    int currentMusic;
    vector<int> order;
    bool isRandom;
    float volume;
public:
    MusicManager();
    void update();
    void playMusic(unsigned int i);
    void loadFromDir(string dir);
    void addMusic(string dir);
    void pause();
    void play();
    void stop();
    void setVolume(float newVolume);
    void setRandom(bool rnd);
    void checkOrder();
    void next();
};

#endif // _MUSIC_MANAGER_HPP_
