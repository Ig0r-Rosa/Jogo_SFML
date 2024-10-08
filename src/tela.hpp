#ifndef B273Z5A6_C8BE_4472_A888_278D577DB63E
#define B273Z5A6_C8BE_4472_A888_278D577DB63E

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

#include "fps.hpp"

class Tela
{
    private:
    unsigned int screenX;
    unsigned int screenY;
    bool backgroundStatus;
    sf::Color backgroundColor;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::View view;

    FPS * fps;

    // Exibir coisas na tela
    sf::Font * font;
    sf::Text textoTempoEmJogo;
    sf::Text MensagemTela;

    public:
    Tela()
    {
        backgroundStatus = false;
        backgroundColor = sf::Color(170, 170, 255);
    };

    ~Tela()
    {
        delete fps;
        delete font;
    };

    void setupTela(unsigned int screenWidth, unsigned int screenHeight)
    {
        view = sf::View(sf::FloatRect(0, 0, screenWidth, screenHeight));
        screenX = screenWidth;
        screenY = screenHeight;

        font = new sf::Font();
        font->loadFromFile("./arquivos/fonte.ttf");

        float escala = std::min(static_cast<float>(screenWidth) / 1366, static_cast<float>(screenHeight) / 768);

        // Texto do tempo do jogo
        textoTempoEmJogo.setFont(*font);
        textoTempoEmJogo.setCharacterSize(40 * escala);
        textoTempoEmJogo.setOutlineThickness(1);
        float xTempoEmJogo = ((1150 * screenWidth) / 1366.0f);
        float yTempoEmJogo = ((100 * screenHeight) / 768.0f);
        textoTempoEmJogo.setPosition(xTempoEmJogo, yTempoEmJogo);
        textoTempoEmJogo.setFillColor(sf::Color::White); // Cor do texto

        // Mensagem na tela
        MensagemTela.setFont(*font);
        MensagemTela.setCharacterSize(130 * escala);
        MensagemTela.setFillColor(sf::Color::White); // Cor do texto
        float xMensagemTela = ((485 * screenWidth) / 1366.0f);
        float yMensagemTela = ((200 * screenHeight) / 768.0f);
        MensagemTela.setPosition(xMensagemTela, yMensagemTela);

        fps = new FPS();
        fps->setupFPS("arquivos/fonte.ttf");

        if (backgroundTexture.loadFromFile("./arquivos/fase_1/fase1_back.png")) {
            backgroundStatus = true;
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setScale(
                float(screenWidth) / backgroundTexture.getSize().x,
                float(screenHeight) / backgroundTexture.getSize().y);
        }
    };
    
    void loopTela(bool pausado, bool naFase, float tempoEmJogo, float tempoRestante)
    {
        if(pausado)
        {
            MensagemTela.setString("PAUSE");

            if(!naFase && tempoRestante > 0)
            {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(0) << std::ceil(tempoRestante);
                MensagemTela.setString("    " + ss.str() + "  ");
            }
        }

        // Atualiza o texto do tempo de jogo
        int minutos = static_cast<int>(tempoEmJogo) / 60;
        int segundos = static_cast<int>(tempoEmJogo) % 60;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << minutos << ":"
        << std::setw(2) << std::setfill('0') << segundos;
        textoTempoEmJogo.setString(ss.str());
    };

    void attCamera(sf::RenderWindow& window, sf::FloatRect bounds, float deltaTime)
    {
        // Obtenha a posição alvo da câmera (centro do personagem)
        float targetX = bounds.left + bounds.width / 2.0f;
        float targetY = bounds.top + bounds.height / 2.0f;

        // Deslocamento para ajustar a câmera verticalmente (se necessário)
        targetY -= ((150 * screenY) / 768);

        // Pega a posição atual da câmera
        sf::Vector2f currentCenter = view.getCenter();

        // Suaviza o movimento da câmera com base no deltaTime (interpolação linear)
        fps->attFPS(deltaTime);
        float fpsAtual = fps->getFPS();
        if(fpsAtual > 60.0f * 1.05){fpsAtual = 60.0f * 1.05;}
        float smoothingFactor = fpsAtual; // Ajuste esse valor para mais suavidade

        float newX = currentCenter.x + (targetX - currentCenter.x) * deltaTime * smoothingFactor;
        float newY = currentCenter.y + (targetY - currentCenter.y) * deltaTime * smoothingFactor;

        // Atualize o centro da view com as coordenadas suavizadas
        view.setCenter(std::round(newX), std::round(newY));
        
        // Define a view
        window.setView(view);

        // Atualiza a posição do background e interface
        float novoIniX = view.getCenter().x - (screenX / 2);
        float novoIniY = view.getCenter().y - (screenY / 2);
        backgroundSprite.setPosition(novoIniX, novoIniY);
        fps->attPosFPS(novoIniX, novoIniY);
        textoTempoEmJogo.setPosition( novoIniX + ((1150 * screenX) / 1366.0f), 
                                      novoIniY + ((100 * screenY) / 768.0f));
        MensagemTela.setPosition( novoIniX + ((485 * screenX) / 1366.0f), 
                                  novoIniY + ((200 * screenY) / 768.0f));
    };

    void desenhaBack(sf::RenderWindow& window)
    {
        window.clear(backgroundColor);
        if (backgroundStatus) 
        {
            window.draw(backgroundSprite);
        }
    };

    void desenhaInterface(sf::RenderWindow& window, bool pausado, float deltaTime)
    {
        window.draw(textoTempoEmJogo);
        if(pausado)
        {
            window.draw(MensagemTela);
        }
        //window.draw(fps->getFPStext());
    };

};




#endif