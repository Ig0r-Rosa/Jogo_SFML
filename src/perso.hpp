#ifndef BE6AF02D_A530_4B7E_BB81_24DC18B14CA6
#define BE6AF02D_A530_4B7E_BB81_24DC18B14CA6

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

enum class AnimationState {
    Idle,
    Running
};

class Personagem
{
private:
    sf::Clock animClock; // Temporizador para animação de idle e run
    
    std::vector<sf::Texture> idle_texture;
    const float animDurationIdle = 0.20f;
    size_t currentIdleFrame; // Índice do quadro atual de idle
    // Definir um pequeno atraso antes de retornar para Idle
    const float idleDelay = 0.05f; // 200ms de atraso
    sf::Clock idleDelayClock; // Temporizador para controlar o atraso de Idle

    std::vector<sf::Texture> run_texture;
    const float animDurationRun = 0.15f;
    size_t currentRunFrame;  // Índice do quadro atual de run
    
    sf::Clock clock;
    bool work;
    bool noChao; // Indica se o personagem está no chão
    bool viradoParaDireita; // Indica a direção atual do sprite
    AnimationState animState; // Estado da animação

    // Movimento
    float aceY; // Aceleração gravitacional (pode ajustar conforme necessário)
    float velY; // Velocidade vertical do personagem
    float velX; // Velocidade de movimento do personagem (pixels por segundo)

public:
    sf::Sprite sprite;
    sf::FloatRect spriteBounds;

    Personagem()
    {
        work = false;
        noChao = false;
        viradoParaDireita = true;
        velY = 0.0f;
        aceY = 300.0f;
        velX = 750.0f;
        animState = AnimationState::Idle; // Inicialmente, a animação é Idle
    }

    ~Personagem() {}

    void setupPerso()
    {
        std::vector<std::string> idle_texture;
        idle_texture.push_back("./arquivos/persoAn/Idle/Idle_1.png");
        idle_texture.push_back("./arquivos/persoAn/Idle/Idle_2.png");
        idle_texture.push_back("./arquivos/persoAn/Idle/Idle_3.png");
        idle_texture.push_back("./arquivos/persoAn/Idle/Idle_4.png");
        carregarIdleTexturas(idle_texture);

        std::vector<std::string> run_texture;
        run_texture.push_back("./arquivos/persoAn/Run/run_1.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_2.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_3.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_4.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_5.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_6.png");
        run_texture.push_back("./arquivos/persoAn/Run/run_7.png");
        carregarRunTexturas(run_texture);
    }

    // Carrega Textura
    void carregarIdleTexturas(std::vector<std::string> link)
    {
        // carrega imagem pelo link
        for(int i = 0; i < link.size(); i++)
        {
            sf::Texture texture;
            if(!texture.loadFromFile(link.at(i)))
            {
                return;
            }
            // Adiciona a textura carregada no vetor
            idle_texture.push_back(texture);
        }
        work = true; 
        
        sprite.setTexture(idle_texture.at(0));
        sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(0, 0);
        spriteBounds = sprite.getLocalBounds();
        sprite.setOrigin((spriteBounds.width) / 2, (spriteBounds.height) / 2);
    }

    void carregarRunTexturas(std::vector<std::string> link)
    {
        // carrega imagem pelo link
        for(int i = 0; i < link.size(); i++)
        {
            sf::Texture texture;
            if(!texture.loadFromFile(link.at(i)))
            {
                return;
            }
            // Adiciona a textura carregada no vetor
            run_texture.push_back(texture);
        }
    }

    // Calcula o deltaTime
    sf::Time calcTime()
    {
        return clock.restart();
    }

    void setPos(int screenWidth, int screenHeight)
    {
        // Obtém as dimensões reais do sprite após a escala
        float imgWidth = sprite.getGlobalBounds().width;
        float imgHeight = sprite.getGlobalBounds().height;

        // Calcula a posição desejada para o sprite -> (screenWidth/2, (screenHeight * 3) / 5)
        float posX = screenWidth / 2;
        float posY = (screenHeight * 3) / 5;
        
        sprite.setPosition(posX - (imgWidth / 6), posY - (imgHeight / 6));
    }

    void setScale(float scaX, float scaY, int screenWidth, int screenHeight)
    {
        float imgWidth = sprite.getTexture()->getSize().x;
        float imgHeight = sprite.getTexture()->getSize().y;
        
        // Resolução base (ajuste conforme necessário)
        float resBaseX = 1366;
        float resBaseY = 768;

        // Calcula a proporção da escala para manter a consistência do sprite
        float scaleX = (screenWidth / resBaseX) * scaX;  // Ajuste da escala em X
        float scaleY = (screenHeight / resBaseY) * scaY; // Ajuste da escala em Y

        // Ajusta o sprite com a nova escala
        sprite.setScale(scaleX, scaleY);
    }

    bool carregouImg()
    {
        return work;
    }

    // Verifica colisão com as bordas da janela e ajusta a posição se necessário
    void verificarColisao(const sf::FloatRect& plataformaBounds, float deltaTime)
    {
        spriteBounds = sprite.getGlobalBounds();

        // Detecta colisão com a plataforma
        if (spriteBounds.intersects(plataformaBounds)) {
            // Verifica a direção da colisão (da parte inferior do sprite)
            if (spriteBounds.top + spriteBounds.height > plataformaBounds.top &&
                spriteBounds.left < plataformaBounds.left + plataformaBounds.width &&
                spriteBounds.left + spriteBounds.width > plataformaBounds.left) {
                // Ajusta a posição do sprite para estar no topo da plataforma
                sprite.setPosition(sprite.getPosition().x, plataformaBounds.top - (spriteBounds.height / 2));

                // Zera a velocidade vertical e marca como estando no chão
                velY = 0;
                noChao = true;
            }
        }
        else {
            noChao = false; // Não está no chão
        }
    }

    // Atualiza a posição do personagem aplicando a gravidade
    void aplicarGravidade(float deltaTime)
    {
        if (!noChao) {
            // Se o personagem não estiver no chão, aplica gravidade
            velY += aceY * deltaTime; // Aceleração aumenta a velocidade
            sprite.move(0, velY * deltaTime); // Atualiza a posição vertical
        }
    }

    // Atualiza o estado de animação com base na entrada
    void atualizarAnimacao(float deltaTime, bool movendoEsq, bool movendoDir)
    {
        if (movendoEsq || movendoDir)
        {
            if (animState != AnimationState::Running)
            {
                animState = AnimationState::Running; // Troca para animação de correr
                currentRunFrame = 0; // Garante que a animação comece do primeiro frame ao correr
            }
            idleDelayClock.restart(); // Reseta o temporizador de delay do Idle
            atualizarRunAnimation(deltaTime);
        }
        else
        {
            if (animState != AnimationState::Idle)
            {
                // Somente troca para Idle após um pequeno atraso
                if (idleDelayClock.getElapsedTime().asSeconds() > idleDelay)
                {
                    animState = AnimationState::Idle; // Troca para animação de idle
                    currentIdleFrame = 0; // Reseta o quadro de idle
                    idleDelayClock.restart(); // Reseta o temporizador para evitar trocas rápidas
                }
            }
            if (animState == AnimationState::Idle)
            {
                atualizarIdleAnimation();
            }
        }
    }

    // Atualiza a animação de idle
    void atualizarIdleAnimation()
    {
        if (animClock.getElapsedTime().asSeconds() > animDurationIdle)
        {
            currentIdleFrame = (currentIdleFrame + 1) % idle_texture.size();
            sprite.setTexture(idle_texture.at(currentIdleFrame));
            animClock.restart();
        }
    }

    // Atualiza a animação de correr
    void atualizarRunAnimation(float deltaTime)
    {
        if (animClock.getElapsedTime().asSeconds() > animDurationRun)
        {
            currentRunFrame = (currentRunFrame + 1) % run_texture.size();
            sprite.setTexture(run_texture.at(currentRunFrame));
            animClock.restart();
        }
    }

    // Movimento do personagem para a esquerda
    void andarEsq(float deltaTime)
    {
        if (viradoParaDireita)
        {
            inverterDirecao();
        }
        sprite.move(-velX * deltaTime, 0);
    }

    // Movimento do personagem para a direita
    void andarDir(float deltaTime)
    {
        if (!viradoParaDireita)
        {
            inverterDirecao();
        }
        sprite.move(velX * deltaTime, 0);
    }

    // Método para inverter a direção do sprite
    void inverterDirecao()
    {
        sprite.scale(-1.f, 1.f);
        viradoParaDireita = !viradoParaDireita;
    }
};

#endif /* BE6AF02D_A530_4B7E_BB81_24DC18B14CA6 */
