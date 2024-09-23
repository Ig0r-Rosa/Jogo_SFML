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
#include <sstream>

#endif

#include "plataforma.hpp"
#include "perso.hpp"
#include "musica.hpp"
#include "tela.hpp"

class Fase_1
{
    private:
    bool naFase;
    bool pausado;
    unsigned int screenWidth;
    unsigned int screenHeight;
        
    sf::Clock timer; // Timer global
    sf::Clock pausaTimer; // Novo timer para controlar os 3 segundos de pausa
    float tempoEmJogo; 
    bool iniciouFase;
    float deltaTime; // Guarda diferença do timer global
    bool movendoEsq;
    bool movendoDir;
    Personagem * perso;
    Audio * musica;
    Plataforma * chao;
    Tela * tela;

    // Função para encapsular cálculo de deltaTime
    void calcularDeltaTime() 
    {
        // Somente atualiza deltaTime se o jogo não estiver pausado
        if (!pausado) 
        {
            deltaTime = timer.restart().asSeconds();
            tempoEmJogo += deltaTime; // Acumula o tempo de jogo
        }
        else 
        {
            // Se o jogo estiver pausado, não atualiza o deltaTime e não reinicia o timer
            timer.restart(); // Impede o acúmulo de tempo durante a pausa
            deltaTime = 0;
        }
    }

    public:
    Fase_1()
    {
        naFase = true;
        pausado = true;
        tempoEmJogo = 0.0;
        iniciouFase = false;
        movendoEsq = false;
        movendoDir = false;
    };

    ~Fase_1(){
        delete perso;
        delete musica;
        delete chao;
        delete tela;
    };

    void setupFase(unsigned int screenWidth, unsigned int screenHeight) {

        musica = new Audio();
        musica->setupAudio("arquivos/fase_1/musica.ogg", true);

        tela = new Tela();
        tela->setupTela(screenWidth, screenHeight);

        perso = new Personagem();
        perso->setupPerso(screenWidth, screenHeight);

        perso->setScale(3.5f, 3.5f, screenWidth, screenHeight);
        perso->setPos(screenWidth, screenHeight);

        chao = new Plataforma();
        chao->setPos(screenWidth * -0.5f, screenHeight * 0.6f);
        chao->setSize(screenWidth * 1.75f, screenHeight * 0.8f);
        chao->carregarTextura("./arquivos/fase_1/fase1_chao.png");

    };

    void preEvento()
    {
        movendoEsq = false;
        movendoDir = false;
    };

    void inputFase(sf::Event &event)
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
    };

    void faseLoop(sf::RenderWindow& window) {
        calcularDeltaTime();  // Atualiza o deltaTime
        
        if(!pausado)
        {
            perso->aplicarGravidade(deltaTime);
            perso->verificarColisao(chao->colisaoArea, deltaTime);
            perso->atualizarAnimacao(deltaTime, movendoEsq, movendoDir);
        }
        
        float tempoDecorrido = pausaTimer.getElapsedTime().asSeconds();
        float tempoRestante = 3.0f - tempoDecorrido;
        
        // Verifica se já se passaram 3 segundos e muda o estado de "pausado"
        if (pausado && !iniciouFase) {
            // Verifica se o tempo restante é menor ou igual a zero
            if (tempoRestante <= 0) {
                pausado = false;  // Despausa após a contagem regressiva
                iniciouFase = true;
            }
        }

        tela->attCamera(window, perso->sprite.getGlobalBounds(), deltaTime);
        tela->loopTela(pausado, iniciouFase, tempoEmJogo, tempoRestante);
    };

    void desenha(sf::RenderWindow& window) {
        tela->desenhaBack(window);
        window.draw(chao->sprite);
        window.draw(perso->sprite);
        tela->desenhaInterface(window, pausado, deltaTime);
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
                    if (event.key.code == sf::Keyboard::Escape) 
                    {
                        naFase = false;
                    }
                    if (iniciouFase && pausaTimer.getElapsedTime().asSeconds() >= 0.5f 
                    && event.key.code == sf::Keyboard::P) 
                    {
                        pausaTimer.restart();
                        pausado = !pausado; // Alterna entre pausado e não pausado
                    }

                    if(!pausado)
                    {
                        inputFase(event);
                    }
                }
            }

            faseLoop(window);
            desenha(window);
        }
    };
    
};


#endif /* BFEF1C3C_AF72_463F_BF1C_8A661741373A */
