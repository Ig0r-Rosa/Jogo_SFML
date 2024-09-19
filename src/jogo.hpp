#ifndef F3F4E2AE_EF7C_4ED9_8E06_896FEA969271
#define F3F4E2AE_EF7C_4ED9_8E06_896FEA969271

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

#endif

// Includes
#include "fase1.hpp"
#include "menu.hpp"

// Indica a função para utiliza-la antes de seu corpo
void aplicarLetterbox(int windowWidth, int windowHeight);

// Variaveis //
// Window
sf::ContextSettings settings;
sf::RenderWindow window;
sf::VideoMode desktopMode;
float aspectRatio;
unsigned int screenWidth;
unsigned int screenHeight;
bool isCursorVisible;

// Jogo
int estaEm;
Menu * menu;

int setupJogo()
{
    estaEm = 0;
    isCursorVisible = false;

    menu = new Menu();
    menu->setupMenu(screenWidth, screenHeight);

    return 0;
}

void loopJogo()
{
    // Eventos
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close(); 
        }

        // Detecta redimensionamento da janela
        if (event.type == sf::Event::Resized)
        {
            // Aplicar "letterbox" para manter a proporção
            aplicarLetterbox(event.size.width, event.size.height);
        }

        if(estaEm == 1)
        {
            estaEm = 0; // Volta ao menu;
            menu->musica->play(false);

            Fase_1 * fase1 = new Fase_1();
            fase1->setupFase(screenWidth, screenHeight);
            fase1->run(window);
            delete fase1;

            menu->musica->play(true);
        }

        menu->eventosLoop(window, event, estaEm);

        if (event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Period)
            {
                isCursorVisible = !isCursorVisible;
                window.setMouseCursorVisible(isCursorVisible);
            }
        }
    }

    menu->desenha(window);
}

// Fecha o jogo
int fechaJogo()
{
    // Deleta ponteiros
    delete menu;

    return 0;
}

// Função para manter a proporção da view com "letterboxing"
void aplicarLetterbox(int windowWidth, int windowHeight)
{
    sf::View view(sf::FloatRect(0, 0, 1280, 720));
    float windowRatio = (float)windowWidth / (float)windowHeight;
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    if (windowRatio > viewRatio)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}


#endif /* F3F4E2AE_EF7C_4ED9_8E06_896FEA969271 */
