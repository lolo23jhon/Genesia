#ifndef HSL_COLOR_H
#define HSL_COLOR_H

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>

struct HSL
{
    double Hue;
    double Saturation;
    double Luminance;

    HSL();
    HSL(int H, int S, int L);

    sf::Color TurnToRGB();

    static HSL TurnToHSL(const sf::Color& C);


private:

    double HueToRGB(double arg1, double arg2, double H);


};


#endif // !HSL_COLOR_H