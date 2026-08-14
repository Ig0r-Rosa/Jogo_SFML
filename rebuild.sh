#!/usr/bin/env bash
# Rebuild: instala pré-requisitos, limpa, recompila e executa o jogo.
# Precisa rodar na raiz do projeto (assets em ./arquivos).

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

BIN_DIR="$ROOT/compilado"
BIN="$BIN_DIR/app"
SRC="$ROOT/src/main.cpp"

# Pacotes Debian necessários para compilar (g++, SFML, GLFW, OpenGL).
PACOTES=(g++ libsfml-dev libglfw3-dev libgl1-mesa-dev libopenal1 libopenal-dev)

pacote_instalado() {
    dpkg -s "$1" >/dev/null 2>&1
}

apt_com_sudo() {
    if [[ "${EUID}" -eq 0 ]]; then
        DEBIAN_FRONTEND=noninteractive apt-get "$@"
    else
        sudo DEBIAN_FRONTEND=noninteractive apt-get "$@"
    fi
}

instalar_pre_requisitos() {
    local faltando=()
    local p
    for p in "${PACOTES[@]}"; do
        if ! pacote_instalado "$p"; then
            faltando+=("$p")
        fi
    done

    if [[ ${#faltando[@]} -eq 0 ]]; then
        echo "==> Pré-requisitos ok."
        return 0
    fi

    echo "==> Faltando: ${faltando[*]}"
    echo "==> Instalando pré-requisitos..."
    apt_com_sudo update -y
    apt_com_sudo install -y "${faltando[@]}"
}

limpar_build() {
    echo "==> Limpando build anterior..."
    rm -f "$BIN"
    mkdir -p "$BIN_DIR"
}

compilar() {
    echo "==> Compilando..."
    g++ -std=c++17 \
        -I/usr/include \
        -L/usr/lib \
        "$SRC" \
        -lglfw \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system \
        -lsfml-audio \
        -lopenal \
        -lGL \
        -ldl \
        -o "$BIN"
}

executar() {
    echo "==> Executando o jogo..."
    cd "$BIN_DIR"
    export ALSOFT_DRIVERS="${ALSOFT_DRIVERS:-pulse}"
    exec "$BIN"
}

instalar_pre_requisitos
limpar_build
compilar
executar
