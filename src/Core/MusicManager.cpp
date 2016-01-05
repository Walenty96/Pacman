#include "Core/MusicManager.hpp"
#include "Core/functions/fn_file.hpp"
MusicManager::MusicManager()
{
    currentMusic=-1;
    isRandom = true;
    volume=100;
    loadFromDir("res/musics/");
}

void MusicManager::update()
{
    if(currentMusic>0){
        if(musics[currentMusic]->getStatus()==Sound::Status::Stopped)
            next();
    }
}

void MusicManager::playMusic(unsigned int i)
{
    if(i<musics.size()){
        if(currentMusic>0)
        stop();
        currentMusic=i;
        musics[currentMusic]->setVolume(volume);
        play();
    }
}

void MusicManager::setVolume(float newVolume){
    volume = newVolume;
    if(currentMusic>=0)
        musics[currentMusic]->setVolume(volume);
}

void MusicManager::loadFromDir(string dir)
{
    vector<string> files = listOfFiles(dir);
    for(string s:files){
        addMusic(dir+s);
    }
}

void MusicManager::addMusic(string dir)
{
    musics.push_back(shared_ptr<Music>(new Music()));
    if(!musics.back()->openFromFile(dir))
        musics.pop_back();
    else{
        musics.back()->setLoop(false);
    }
}

void MusicManager::pause()
{
    if(currentMusic>=0)
        musics[currentMusic]->pause();
}
void MusicManager::stop()
{
    if(currentMusic>=0){
        musics[currentMusic]->stop();
        currentMusic=-1;
    }
}
void MusicManager::play()
{
    if(currentMusic>=0)
        musics[currentMusic]->play();
    else{
        next();
    }
}

void MusicManager::setRandom(bool rnd)
{
    isRandom = rnd;
}

void MusicManager::checkOrder()
{
    order.clear();
    for(unsigned int i=0;i<musics.size();i++){
        order.push_back(i);
    }
    for(unsigned int i=0;i<musics.size()*2;i++){
        swap(order[0],order[rand()%musics.size()]);
    }
}

void MusicManager::next()
{
    if(musics.size()==0)
        return;
    if(!isRandom){
        playMusic((currentMusic+1)%musics.size());
    }
    else{
        if(order.size()==0)
            checkOrder();
        if(order.size()>0){
            playMusic(order.back());
            order.pop_back();

        }
    }
}
