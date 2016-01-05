#ifndef _FLOU_HPP_
#define _FLOU_HPP_

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Flou{
    private:
        Vector2f position;
        Vector2f size;
    public :
        void setPosition(Vector2f newPosition);
        void setSize(Vector2f newSize);
        void applyFlou(RenderTexture & texture);
};

#endif // _FLOU_HPP_
