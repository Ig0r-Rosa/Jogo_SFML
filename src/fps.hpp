#ifndef FE79A794_5D72_4049_A656_4782AEC93079
#define FE79A794_5D72_4049_A656_4782AEC93079

#ifndef INCLUDES
#define INCLUDES

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string> 
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <sstream>

#endif

class FPS
{
    private:
    sf::Font font;
    bool achou;
    float fps;
    float frameCount;
    sf::Text fpsText;
    float fpsDeltaTime;

    public:

    FPS()
    {
        fps = 0;
        frameCount = 0;
        fpsDeltaTime = 0;
        achou = false;
    };

    ~FPS(){};

    void setupFPS(std::string link)
    {
        if (font.loadFromFile(link)) {
            achou = true;
        }
        fpsText.setFont(font);
        fpsText.setCharacterSize(24);
        fpsText.setFillColor(sf::Color(100, 255, 100));
        fpsText.setPosition(10, 10);
    }

    bool getAchou()
    {
        return achou;
    };

    sf::Text attFPS(float deltaTime)
    {
        frameCount++;
        fpsDeltaTime += deltaTime;
        if(fpsDeltaTime >= 1)
        {
            fps = std::round(frameCount / fpsDeltaTime);
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frameCount = 0;
            fpsDeltaTime = 0;
        }
        return fpsText;
    };

    void attPosFPS(float iniX, float iniY)
    {
        float posX = iniX + 10;
        float posY = iniY + 10;
        fpsText.setPosition(posX, posY);
    };

};

#endif /* FE79A794_5D72_4049_A656_4782AEC93079 */
