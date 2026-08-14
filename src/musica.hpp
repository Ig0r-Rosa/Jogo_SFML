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

#ifdef __linux__
#include <AL/alc.h>
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

    bool audioDisponivel()
    {
#ifdef __linux__
        return alcGetCurrentContext() != nullptr;
#else
        return true;
#endif
    }

    void setupAudio(std::string link, bool loop)
    {
        if (!audioDisponivel())
        {
            return;
        }
        if (!backgroundMusic.openFromFile(link))
        {
            return;
        }
        backgroundMusic.setLoop(loop);
        backgroundMusic.play();
        achou = (backgroundMusic.getStatus() == sf::Music::Playing);
        if (!achou)
        {
            backgroundMusic.stop();
        }
    }

    void play(bool tocar)
    {
        if (!achou)
        {
            return;
        }
        if (tocar)
        {
            backgroundMusic.play();
        }
        else
        {
            backgroundMusic.stop();
        }
    }

    void pausar()
    {
        if (achou)
        {
            backgroundMusic.pause();
        }
    }

    void retomar()
    {
        if (achou)
        {
            backgroundMusic.play();
        }
    }

    bool rodou()
    {
        return achou;
    };

};


#endif /* DF4056F8_FB52_4F93_B5A4_25E0F704504E */
