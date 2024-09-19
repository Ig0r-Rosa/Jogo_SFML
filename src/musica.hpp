#ifndef DF4056F8_FB52_4F93_B5A4_25E0F704504E
#define DF4056F8_FB52_4F93_B5A4_25E0F704504E

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

class Audio
{
    private:
    bool achou;

    public:
    sf::Music backgroundMusic;
    
    Audio()
    {
        achou = false;
    };

    ~Audio()
    {
        
    };

    void setupAudio(std::string link, bool loop)
    {
        if (backgroundMusic.openFromFile(link)) {
            achou = true;
        }
        // Inicia a reprodução da música em loop
        backgroundMusic.setLoop(loop);
        backgroundMusic.play();
    }

    void play(bool esc)
    {
        if(esc)
        {
            backgroundMusic.play();
        }
        else
        {
            backgroundMusic.stop();
        }
    };

    bool rodou()
    {
        return achou;
    };

};


#endif /* DF4056F8_FB52_4F93_B5A4_25E0F704504E */
