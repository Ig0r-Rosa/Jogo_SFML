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
#include "jogo.hpp"

// Main
int main()
{
    criarJanela();
    aspectRatio = static_cast<float>(window.getSize().x) / window.getSize().y;

    // Esconde o mouse e aparece apenas quando aperta "."
    window.setMouseCursorVisible(false);

    // Tenta definir o foco na janela após criá-la
    window.requestFocus();

    // Ativa o VSync
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Chama o setup do jogo //
    if(setupJogo() != 0){return -1;}

    // Loop principal
    while(window.isOpen())
    {   
        // Ação do loop
        loopJogo();
    }

    return fechaJogo();
}