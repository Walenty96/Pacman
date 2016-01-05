#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;


class Light{
    private:
        Vector2f position;
        Color color;
        float range;
    public :
        Light();
        void applyLight(RenderTexture & window) const;
        void setPosition(Vector2f newPos);
        void setColor(Color newColor);
};

#endif // _LIGHT_HPP_
