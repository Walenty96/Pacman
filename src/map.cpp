#include "menu.hpp"
#include "Core/Ressource.hpp"
#include <iostream>
#include <fstream>
#include <Core/functions/fn_string.hpp>
#include "map.hpp"
#include <sstream>

#include "Game.h"

using namespace std;
using namespace sf;

Map::Map(Game & game):_game(game)
{
    //GRAPHIQUE
    cases_sprite[0] =   Ressource::getSprite("tileTestPacman.png",IntRect(0,0,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[1] =   Ressource::getSprite("tileTestPacman.png",IntRect(32,0,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[2] =   Ressource::getSprite("tileTestPacman.png",IntRect(64,0,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[3] =   Ressource::getSprite("tileTestPacman.png",IntRect(96,0,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[4] =   Ressource::getSprite("tileTestPacman.png",IntRect(0,32,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[5] =   Ressource::getSprite("tileTestPacman.png",IntRect(0,32,TAILLE_CASE,TAILLE_CASE));
    cases_sprite[6] =   Ressource::getSprite("tileTestPacman.png",IntRect(0,32,TAILLE_CASE,TAILLE_CASE));

    coins_sprite    =   Ressource::getSprite("gum.png",IntRect(0,0,TAILLE_CASE,TAILLE_CASE));
    pacgum_sprite   =   Ressource::getSprite("gum.png",IntRect(32,0,TAILLE_CASE,TAILLE_CASE));
    fruit_sprite    =   Ressource::getSprite("tileTestPacman.png",IntRect(32,32,TAILLE_CASE,TAILLE_CASE));

    //LOGIQUES
    positionFruit   =   Vector2i(-1,-1);
    pointFruit      =   0;
    etape_fruit     =   0;
    time_fruit      =   seconds(0);
    need_fruit      =   true;
    nbGum           =   0;

}

void Map::update(Time delta)
{
    updateFruit(delta);
}

void Map::updateFruit(Time delta)
{
    Vector2i _size = getSize();
    time_fruit += delta; //on incrémente le compteur de temps

    if(time_fruit.asSeconds()>30){  //temps dépassé

        time_fruit=seconds(0);      //on remet le compteur à zéro
        takeFruit(positionFruit);   //on enleve le fruit si il est encore là
        etape_fruit++;              //on passe au fruit suivant
        need_fruit=true;            //on as pas encore généré le prochain fruit
    }
    else if(time_fruit.asSeconds()>15 && need_fruit      //Le fruit doit apparaître si il n'a pas encore été généré
        && positionFruit.x>=0 && positionFruit.x<_size.x
        && positionFruit.y>=0 && positionFruit.y<_size.y){ //Si sa position est bien dans la carte
            need_fruit=false;
            cases[positionFruit.x][positionFruit.y]=7;

            switch(etape_fruit){ //en fonction de l'étape où nous sommes, nous générons le fruit correspondants
                case 0: //Cerise
                    pointFruit=100;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(0,0,TAILLE_CASE,TAILLE_CASE));
                break;
                case 1: //Fraise
                    pointFruit=200;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(0,TAILLE_CASE,TAILLE_CASE,TAILLE_CASE));
                break;
                case 2: //Peche
                    pointFruit=300;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(0,TAILLE_CASE*etape_fruit,TAILLE_CASE,TAILLE_CASE));
                break;
                case 3: //Pomme
                    pointFruit=400;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(0,TAILLE_CASE*etape_fruit,TAILLE_CASE,TAILLE_CASE));
                break;

                case 4: //vert
                    pointFruit=500;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(TAILLE_CASE,0,TAILLE_CASE,TAILLE_CASE));
                break;

                case 5: //roumain
                    pointFruit=600;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(TAILLE_CASE,TAILLE_CASE*2,TAILLE_CASE,TAILLE_CASE));
                break;

                case 6: //cloche
                    pointFruit=700;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(TAILLE_CASE,TAILLE_CASE*3,TAILLE_CASE,TAILLE_CASE));
                break;

                case 7: //cloche
                    pointFruit=800;
                    fruit_sprite = Ressource::getSprite("fruits.png",IntRect(TAILLE_CASE,TAILLE_CASE*4,TAILLE_CASE,TAILLE_CASE));
                break;

                default://Dans le cas où le fruit n'a pas été trouvé (on les as déjà tous rammassés par exemple)
                    cases[positionFruit.x][positionFruit.y]=5;
                break;
            }

    }
}


void Map::loadFromFile(string str)
{
    str="res/map/"+str;
    cout << "Chargement de la map : \""<<str<<"\"..........";
    ifstream file(str);
    if(!file.good()){
        cout << "Échoué" << endl;
        cerr << "Impossible de lire la map : \""<<str<<"\"." << endl;
        return;
    }
    positionsFantomes.clear();
    vector<vector<int>> tmp_case;

    string line;
    vector<string> mots;
    while(getline(file,line)){
        mots = explode(line,',');
        if( tmp_case.size()!=0                 //DANS LE CAS OÙ ÇA N'EST PAS LA PREMIÈRE LIGNE
        &&  tmp_case[0].size() != mots.size()) //ON VÉRIFIE QU'ELLE FAIT BIEN LA MÊME TAILLE.
        {
            cout << "Échoué" << endl;
            cerr << "Mauvais format pour la map : \""<<str<<"\"." << endl;
            return;
        }
        vector<int> line_int;
        for(string mot_case:mots){

              int nb_case=string_to_int(mot_case);
              if(nb_case<0||nb_case>8){
                cout << "Échoué" << endl;
                cerr << "Mauvais type de case pour la map : \""<<str<<"\"." << endl;
                return;
              }else if(nb_case==7){
                positionFruit = Vector2i(line_int.size(),tmp_case.size());

                nb_case=5;
              }else if(nb_case==8){
                positionsFantomes.push_back(Vector2i(line_int.size(),tmp_case.size()));

                nb_case=5;
              }
              line_int.push_back(nb_case);


        }

        tmp_case.push_back(line_int);
    }

    file.close();

    if(positionsFantomes.size()<4){
        cout << "Échoué" << endl;
        cerr << "Mauvaise position des fantomes (pas assez de points de spawns) : \""<<str<<"\"." << endl;
        return;
    }
    //ROTATION DE LA MATRICE
    nbGum=0;
    cases.clear();
    if(tmp_case.size()>0)
    for(int x=0;x<tmp_case[0].size();x++){
        vector<int> line_int;
        for(int y=0;y<tmp_case.size();y++){
            line_int.push_back(tmp_case[y][x]);
            if (tmp_case[y][x]==6 || tmp_case[y][x]==2)
                addGum(1);

        }
        cases.push_back(line_int);
    }
    cout << "terminé !" << endl;

}

bool Map::playerCanWalkOn(int x, int y) const
{
    Vector2i _size = getSize();

    if(x>=0 && x<_size.x
    && y>=0 && y<_size.y        //DANS LA CARTE
        && cases[x][y] != 1     //PAS UN MUR
        && cases[x][y] != 4     //NI UN MUR FANTÔME
        && cases[x][y] != 3)    //NI UNE PORTE FANTÔME
        return true;

    return false;
}

bool Map::playerCanWalkOn(Vector2i position) const
{
    return playerCanWalkOn(position.x,position.y);
}

bool Map::ghostCanWalkOn(int x, int y) const
{
    Vector2i _size = getSize();

    if(x>=0 && x<_size.x
    && y>=0 && y<_size.y        //DANS LA CARTE
        && cases[x][y] != 1     //PAS UN MUR
        && cases[x][y] != 4)    //NI UN MUR FANTÔME
        return true;

    return false;

}

bool Map::ghostCanWalkOn(Vector2i position) const
{
    return ghostCanWalkOn(position.x,position.y);
}

bool Map::coinOn(int x, int y) const
{
    Vector2i _size = getSize();

    if(x>=0 && x<_size.x
    && y>=0 && y<_size.y
    && cases[x][y]==2)
        return true;

    return false;
}

bool Map::coinOn(Vector2i position)  const
{
    return coinOn(position.x,position.y);
}

bool Map::takeCoin(int x, int y)
{
    if(!coinOn(x,y))
        return false;
    else{
        cases[x][y]=5;
        addGum(-1);
        return true;
    }
}

bool Map::takeCoin(Vector2i position)
{
    return takeCoin(position.x,position.y);
}

Vector2i Map::getSize() const
{
    if(cases.size()==0)
        return Vector2i(0,0);
    return Vector2i(cases.size(),cases[0].size());
}


void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{


    Sprite tmp_case;
    Sprite tmp_coin=coins_sprite;
    Sprite tmp_pacgum = pacgum_sprite;
	Sprite tmp_fruit = fruit_sprite;


    Vector2i _size = getSize();

    for(int x=0;x<_size.x;x++)
    for(int y=0;y<_size.y;y++){

        Vector2f pos(x*TAILLE_CASE,y*TAILLE_CASE);
        tmp_case=cases_sprite[cases[x][y]-1];
        tmp_case.setPosition(pos);
        target.draw(tmp_case,states);
        switch(cases[x][y]){
            case 2:
                tmp_coin.setPosition(pos);
                target.draw(tmp_coin,states);
            break;
            case 6:
                tmp_pacgum.setPosition(pos);
                target.draw(tmp_pacgum,states);

            break;
            case 7:
                tmp_fruit.setPosition(pos);
                target.draw(tmp_fruit,states);
            break;
            default:
            break;
        }
    }



}

bool Map::bonusOn(int x, int y) const
{
    Vector2i _size = getSize();

    if(x>=0 && x<_size.x
    && y>=0 && y<_size.y
    && cases[x][y]==6)
        return true;

    return false;
}

bool Map::bonusOn(Vector2i position)  const
{
    return bonusOn(position.x,position.y);
}

bool Map::takeBonus(int x, int y)
{
    if(!bonusOn(x,y))
        return false;
    else{

        addGum(-1);
        cases[x][y]=5;
        return true;
    }
}

bool Map::takeBonus(Vector2i position)
{
    return takeBonus(position.x,position.y);
}

bool Map::fruitOn(int x, int y) const
{
    Vector2i _size = getSize();

    if(x>=0 && x<_size.x
    && y>=0 && y<_size.y
    && cases[x][y]==7)
        return true;

    return false;
}

bool Map::fruitOn(Vector2i position)  const
{
    return fruitOn(position.x,position.y);
}

int Map::takeFruit(int x, int y)
{
    if(!fruitOn(x,y))
        return 0;
    else{
        cases[x][y]=5;
        int point=pointFruit;
        pointFruit=0;

        return point;
    }
}

int Map::takeFruit(Vector2i position)
{
    return takeFruit(position.x,position.y);
}

int Map::getNbGum()
{

    return nbGum;

}
void Map::addGum(int nb)
{

    nbGum+=nb;
    if (nbGum==0)
        _game.notifyWin();

}
vector<Vector2i> Map::getPositionsFantomes()
{
    return positionsFantomes;
}



