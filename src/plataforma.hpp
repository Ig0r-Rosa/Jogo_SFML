#ifndef B273D5A6_C8AE_4472_A888_228D577DB63E
#define B273D5A6_C8AE_4472_A888_228D577DB63E


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

class Plataforma
{
private:
    bool work = false;
    sf::Texture texture; // Membro da classe para persistir a textura
    bool solido = false;

public:
    sf::Sprite sprite;
    sf::FloatRect spriteBounds;
    sf::FloatRect colisaoArea;

    // Construtor padrão que cria um bloco verde
    Plataforma()
    {
        solido = true;

        // Cria uma imagem verde opaca
        sf::Image image;
        image.create(100, 100, sf::Color::White); // Verde opaco

        // Carrega a imagem na textura e define a textura no sprite
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        // Define a transparência do sprite (50% transparente)
        sprite.setColor(sf::Color(255, 255, 255, 128)); // Alfa de 128 (50% transparente)

        // Define o tamanho e a escala do sprite
        sprite.setScale(1.0f, 1.0f); // A escala deve ser 1.0 inicialmente

        // Atualiza os limites do sprite e a área de colisão
        spriteBounds = sprite.getGlobalBounds();
        colisaoArea = spriteBounds;
    }

    // Método para definir a posição da plataforma
    void setPos(float posX, float posY)
    {
        sprite.setPosition(posX, posY);

        // Atualiza os limites do sprite e a área de colisão após mover
        spriteBounds = sprite.getGlobalBounds();
        colisaoArea = spriteBounds;
    }

    // Método para carregar uma textura mantendo o tamanho do sprite
    void carregarTextura(const std::string& caminho)
    {
        if (!texture.loadFromFile(caminho))
        {
            std::cerr << "Erro ao carregar a textura!" << std::endl;
            return;
        }

        // Defina a textura no sprite
        sprite.setTexture(texture);
        
        // Defina o retângulo da textura (caso queira manter a textura inteira)
        sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

        // Atualiza a escala para manter o tamanho anterior do sprite
        float scaleX = spriteBounds.width / texture.getSize().x;
        float scaleY = spriteBounds.height / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);

        return;
    }

    // Método para definir o tamanho da plataforma
    void setSize(float width, float height)
    {
        // Atualiza os limites do sprite e a área de colisão
        spriteBounds.width = width;
        spriteBounds.height = height;
        colisaoArea = spriteBounds;
        colisaoArea.top = (spriteBounds.top * 7.5) / 5;

        // Aplica a nova escala ao sprite com base no novo tamanho
        sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
    }

    // Método para verificar colisão com outro sprite
    bool verificarColisao(const sf::FloatRect& outro)
    {
        // Verifica se há interseção entre a área de colisão e o outro retângulo
        return colisaoArea.intersects(outro);
    }
};

#endif /* B273D5A6_C8AE_4472_A888_228D577DB63E */
