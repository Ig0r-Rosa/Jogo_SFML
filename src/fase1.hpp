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
#include "fps.hpp"

class Fase_1
{
    private:
    bool naFase;
    bool pausado;
    unsigned int screenWidth;
    unsigned int screenHeight;
    bool backgroundStatus;
    sf::Color backgroundColor;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::View view;
        
    sf::Clock timer; // Timer global
    sf::Clock pausaTimer; // Novo timer para controlar os 3 segundos de pausa
    float tempoEmJogo; 
    bool iniciouFase;
    float deltaTime; // Guarda diferença do timer global
    bool movendoEsq;
    bool movendoDir;
    Personagem * perso;
    Audio * musica;
    FPS * fps;
    Plataforma * chao;

    // Exibir coisas na tela
    sf::Font * font;
    sf::Text textoTempoEmJogo;
    sf::Text MensagemTela;

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
        backgroundStatus = false;
        backgroundColor = sf::Color(170, 170, 255);
    };

    ~Fase_1(){
        delete perso;
        delete musica;
        delete fps;
        delete chao;
        delete font;
    };

    void setupFase(unsigned int screenWidth, unsigned int screenHeight) {
        view = sf::View(sf::FloatRect(0, 0, screenWidth, screenHeight));

        musica = new Audio();
        musica->setupAudio("arquivos/fase_1/musica.ogg", true);

        fps = new FPS();
        fps->setupFPS("arquivos/fonte.ttf");

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
        float xMensagemTela = ((475 * screenWidth) / 1366.0f);
        float yMensagemTela = ((200 * screenHeight) / 768.0f);
        MensagemTela.setPosition(xMensagemTela, yMensagemTela);

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
        else if(iniciouFase)
        {
            MensagemTela.setString("PAUSE");
        }

        // Verifica se já se passaram 3 segundos e muda o estado de "pausado"
        if (pausado && !iniciouFase) {
            float tempoDecorrido = pausaTimer.getElapsedTime().asSeconds();
            float tempoRestante = 3.0f - tempoDecorrido;

            // Verifica se o tempo restante é menor ou igual a zero
            if (tempoRestante <= 0) {
                pausado = false;  // Despausa após a contagem regressiva
                iniciouFase = true;
            } else {
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

    void desenha(sf::RenderWindow& window) {
        window.clear(backgroundColor);
        if (backgroundStatus) 
        {
            window.draw(backgroundSprite);
        }
        window.draw(chao->sprite);
        window.draw(perso->sprite);
        window.draw(textoTempoEmJogo);
        if(pausado)
        {
            window.draw(MensagemTela);
        }
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
