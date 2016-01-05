#ifndef _LIGHT_MANAGER_HPP_
#define _LIGHT_MANAGER_HPP_
#include <list>
#include "effects/light/Light.hpp"
class LightManager{
public:
    void applyLight(RenderTexture & texture);
    Light & getLight(int id);
    Light & back();
    void addLight(sf::Vector2f pos,Color color);
private:
    list<Light> lights;
};

#endif // _LIGHT_MANAGER_HPP_
