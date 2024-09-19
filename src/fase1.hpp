#ifndef BFEF1C3C_AF72_463F_BF1C_8A661741373A
#define BFEF1C3C_AF72_463F_BF1C_8A661741373A

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

#include "plataforma.hpp"
#include "perso.hpp"
#include "musica.hpp"
#include "fps.hpp"

class Fase_1
{
    private:
    bool naFase;
    unsigned int screenWidth;
    unsigned int screenHeight;
    bool backgroundStatus;
    sf::Color backgroundColor;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::View view;
        
    sf::Clock timer; // Timer global
    float deltaTime; // Guarda diferença do timer global
    bool movendoEsq;
    bool movendoDir;
    Personagem * perso;
    Audio * musica;
    FPS * fps;
    Plataforma * chao;

    // Função para encapsular cálculo de deltaTime
    void calcularDeltaTime() {
        deltaTime = timer.restart().asSeconds();
    }

    public:
    Fase_1()
    {
        naFase = true;
        movendoEsq = false;
        movendoDir = false;
        backgroundStatus = false;
        backgroundColor = sf::Color(170, 170, 255);
    };

    ~Fase_1(){
        delete perso;
        delete musica;
        delete fps;
        delete chao;
    };

    void setupFase(unsigned int screenWidth, unsigned int screenHeight) {
        view = sf::View(sf::FloatRect(0, 0, screenWidth, screenHeight));

        musica = new Audio();
        musica->setupAudio("arquivos/fase_1/musica.ogg", true);

        fps = new FPS();
        fps->setupFPS("arquivos/fonte.ttf");

        perso = new Personagem();

        std::vector<std::string> idle_texture;
        idle_texture.push_back("./arquivos/persoAn/idle/Idle 1.png");
        idle_texture.push_back("./arquivos/persoAn/idle/Idle 2.png");
        idle_texture.push_back("./arquivos/persoAn/idle/Idle 3.png");
        idle_texture.push_back("./arquivos/persoAn/idle/Idle 4.png");
        perso->carregarIdleTexturas(idle_texture);

        std::vector<std::string> run_texture;
        run_texture.push_back("./arquivos/persoAn/Run/run_1.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_2.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_3.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_4.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_5.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_6.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_7.png");
        perso->carregarRunTexturas(run_texture);

        perso->setScale(3.5f, 3.5f, screenWidth, screenHeight);
        perso->setPos(screenWidth, screenHeight);

        chao = new Plataforma();
        chao->setPos(screenWidth * -0.5f, screenHeight * 0.6f);
        chao->setSize(screenWidth * 1.75f, screenHeight * 0.8f);
        chao->carregarTextura("./arquivos/fase_1/fase1_chao.png");

        if (backgroundTexture.loadFromFile("./arquivos/fase_1/fase1_back.png")) {
            backgroundStatus = true;
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setScale(
                float(screenWidth) / backgroundTexture.getSize().x,
                float(screenHeight) / backgroundTexture.getSize().y);
        }
    };

    void preEvento()
    {
        movendoEsq = false;
        movendoDir = false;
    };

    void inputFase(sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            // Movimento do personagem
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                perso->andarEsq(deltaTime);
                movendoEsq = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                perso->andarDir(deltaTime);
                movendoDir = true;
            }
        }
    };

    void faseLoop(sf::RenderWindow& window) {
        calcularDeltaTime();  // Atualiza o deltaTime
        perso->aplicarGravidade(deltaTime);
        perso->verificarColisao(chao->colisaoArea, deltaTime);
        perso->atualizarAnimacao(deltaTime, movendoEsq, movendoDir);
    };

    void desenha(sf::RenderWindow& window) {
        window.clear(backgroundColor);
        if (backgroundStatus) {
            window.draw(backgroundSprite);
        }
        window.draw(chao->sprite);
        window.draw(perso->sprite);
        sf::Text fpsText = fps->attFPS(deltaTime);
        window.draw(fpsText);
        window.setView(view);
        window.display();
    };

    void run(sf::RenderWindow& window) {
        int isCursorVisible = false;
        window.setMouseCursorVisible(isCursorVisible);
        while (naFase) {
            preEvento();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) 
                {
                    naFase = false;
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) 
                {
                    if (event.key.code == sf::Keyboard::Period) 
                    {
                        isCursorVisible = !isCursorVisible;
                        window.setMouseCursorVisible(isCursorVisible);
                    } 
                    else if (event.key.code == sf::Keyboard::Escape) 
                    {
                        naFase = false;
                    }
                }
                inputFase(event);
            }

            faseLoop(window);
            desenha(window);
        }
    };
    
};


#endif /* BFEF1C3C_AF72_463F_BF1C_8A661741373A */
