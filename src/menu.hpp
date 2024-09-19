#ifndef D0A73387_D835_44B4_8E76_B08F62A18124
#define D0A73387_D835_44B4_8E76_B08F62A18124 

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

class Menu
{
    private:
    bool pressionado;
    bool selecionado;
    int itemSelecionado;

    sf::RectangleShape * winClose;
    sf::Font * font;
    sf::Texture * img;
    sf::Sprite * sprite;

    sf::Vector2i posMouse;
    sf::Vector2f coordMouse;
    std::vector<sf::String> options;
    std::vector<sf::Vector2f> coordenadas;
    std::vector<sf::Text> textos;
    std::vector<int> tamanhos;

    public:
    Audio * musica;
    
    Menu()
    {
        winClose = new sf::RectangleShape();
        font = new sf::Font();
        img = new sf::Texture();
        sprite = new sf::Sprite();
        musica = new Audio();
    };

    ~Menu()
    {
        delete winClose;
        delete font;
        delete img;
        delete sprite;
        delete musica;
    };

    void setupMenu(unsigned int screenWidth, unsigned int screenHeight)
    {
        pressionado = false;
        selecionado = false;
        itemSelecionado = 1;

        font->loadFromFile("./arquivos/fonte.ttf");
        img->loadFromFile("./arquivos/menu/menu_back.png");
        sprite->setTexture(*img);
        sprite->setScale(
        float(screenWidth) / img->getSize().x,
        float(screenHeight) / img->getSize().y);
        musica->setupAudio("./arquivos/menu/musica.ogg", true);

        posMouse = {0,0};
        coordMouse = {0,0};

        options = {L"Noctalis", L"Jogar", L"Opções", L"Sair"};
        textos.resize(4);
        coordenadas = {{((100 * screenWidth) / 1366.0f), ((50 * screenHeight) / 768.0f)},
               {((100 * screenWidth) / 1366.0f), ((250 * screenHeight) / 768.0f)},
               {((100 * screenWidth) / 1366.0f), ((375 * screenHeight) / 768.0f)},
               {((100 * screenWidth) / 1366.0f), ((500 * screenHeight) / 768.0f)}};
        tamanhos = {150, 55, 45, 45};

        for(int i = 0; i < textos.size(); i++)
        {
            textos[i].setFont(*font);
            textos[i].setString(options[i]);
            textos[i].setCharacterSize(tamanhos[i]);
            textos[i].setOutlineColor(sf::Color::White);
            textos[i].setPosition(coordenadas[i]);
        }
        textos[itemSelecionado].setOutlineThickness(1);
    };

    void eventosLoop(sf::RenderWindow &window, sf::Event &event, int &estaEm)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !pressionado)
            {
                if(itemSelecionado < 3)
                {
                    itemSelecionado++;
                    pressionado = true;
                    textos[itemSelecionado].setOutlineThickness(1);
                    textos[itemSelecionado -1].setOutlineThickness(0);
                    pressionado = false;
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !pressionado)
            {
                if(itemSelecionado > 1)
                {
                    itemSelecionado--;
                    pressionado = true;
                    textos[itemSelecionado].setOutlineThickness(1);
                    textos[itemSelecionado +1].setOutlineThickness(0);
                    pressionado = false;
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !pressionado)
            {
                pressionado = true;
                if(itemSelecionado == 1)
                {
                    estaEm = 1;
                }
                else if(itemSelecionado == 2)
                {
                    textos[0].setString("Vai se fuder");
                }
                else if(itemSelecionado == 3)
                {
                    window.close(); 
                }
                pressionado = false;
            }
        }
    };

    void desenha(sf::RenderWindow &window)
    {
        window.clear();
        window.draw(*sprite);
        for(auto t : textos)
        {
            window.draw(t);
        }

        window.display();
    };
};

#endif /* D0A73387_D835_44B4_8E76_B08F62A18124 */
