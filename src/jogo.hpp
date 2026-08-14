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
#include <sstream>

#endif

#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#endif

// Includes
#include "fase1.hpp"
#include "menu.hpp"

// Indica a função para utiliza-la antes de seu corpo
void aplicarLetterbox(int windowWidth, int windowHeight);
void criarJanela();

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
sf::View menuView;
Menu * menu;

// Modo de um monitor só. getDesktopMode() no X11 junta as 2 telas (ex.: 3840x1080).
sf::VideoMode modoDaTelaPrincipal()
{
    const std::vector<sf::VideoMode> modos = sf::VideoMode::getFullscreenModes();
    if (!modos.empty())
    {
        return modos.front();
    }
    return sf::VideoMode::getDesktopMode();
}

// Assets ficam em compilado/arquivos (caminho relativo ./arquivos/...).
void irParaPastaDoExecutavel()
{
#ifdef __linux__
    char buf[PATH_MAX];
    const ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (n <= 0)
    {
        return;
    }
    buf[n] = '\0';
    const std::string caminho(buf);
    const auto pos = caminho.find_last_of('/');
    if (pos != std::string::npos)
    {
        chdir(caminho.substr(0, pos).c_str());
    }
#endif
}

// Janela sem borda: cobre a tela sem trocar o modo do monitor (quebra no Wayland).
void criarJanela()
{
    irParaPastaDoExecutavel();

    desktopMode = modoDaTelaPrincipal();
    screenWidth = desktopMode.width;
    screenHeight = desktopMode.height;
    settings.antialiasingLevel = 8;

    window.create(desktopMode, "app", sf::Style::None, settings);
    window.setPosition(sf::Vector2i(0, 0));
}

int setupJogo()
{
    estaEm = 0;
    isCursorVisible = false;

    menu = new Menu();
    menu->setupMenu(screenWidth, screenHeight);

    menuView = sf::View(sf::FloatRect(0, 0, screenWidth, screenHeight));

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
            window.setView(menuView);
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
