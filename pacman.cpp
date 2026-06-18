#include <SFML/Graphics.hpp>
#include <iostream>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   não implementada
// Intenção de movimento: não implementado
// Atualizado para 3.1.0: André Gustavo   03/06/26

char mapa[31][29] = {
    "1111111111111111111111111111", // Linha 0
    "1000000000000110000000000001", // Linha 1
    "1011110111110110111110111101", // Linha 2
    "1011110111110110111110111101", // Linha 3
    "1011110111110110111110111101", // Linha 4
    "1000000000000000000000000001", // Linha 5
    "1011110110111111110110111101", // Linha 6
    "1011110110111111110110111101", // Linha 7
    "1000000110000110000110000001", // Linha 8
    "1111110111110110111110111111", // Linha 9
    "1111110111110110111110111111", // Linha 10
    "1111110110000000000110111111", // Linha 11
    "1111110110111001110110111111", // Linha 12
    "1111110110100000010110111111", // Linha 13
    "0000000000100000010000000000", // Linha 14: Túnel de teleporte (só zeros)
    "1111110110100000010110111111", // Linha 15
    "1111110110111111110110111111", // Linha 16
    "1111110110000000000110111111", // Linha 17
    "1111110110111111110110111111", // Linha 18
    "1111110110111111110110111111", // Linha 19
    "1000000000000110000000000001", // Linha 20
    "1011110111110110111110111101", // Linha 21
    "1011110111110110111110111101", // Linha 22
    "1000110000000000000000110001", // Linha 23
    "1110110110111111110110110111", // Linha 24
    "1110110110111111110110110111", // Linha 25
    "1000000110000110000110000001", // Linha 26
    "1011111111110110111111111101", // Linha 27
    "1011111111110110111111111101", // Linha 28
    "1000000000000000000000000001", // Linha 29
    "1111111111111111111111111111"  // Linha 30
};

const float SIZE = 34;      // Tamanho de cada célula do mapa

int posx = 9; // posicao do PacMan
int posy = 7;

float posxf = 9.0f; //posicao relativa fluida
float posyf = 7.0f; 
float xdeslocamento = 500;
float ydeslocamento = 0;

bool cima = false;  // direcao de movimento do PacMan
bool baixo = false;
bool esq = false;
bool dir = false;

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Pac-Man", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    // cria um quadrado de tamanho 50 (a parede)
    sf::RectangleShape quad({SIZE, SIZE});

    // sprites do PacMan
    sf::Texture texture;
    if (!texture.loadFromFile("./sprites/pacman.png")) {
        std::cout << "Erro lendo imagem pacman.png\n";
        return 0;
    }
    sf::Sprite sprite{texture};

    // cria um relogio para medir o tempo do PacMan
    sf::Clock relogioMovimento;
    sf::Clock relogioAnimacao;

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        while (const std::optional event = window.pollEvent()) {
            // evento "fechar" acionado: fecha a janela
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                  if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                      window.close();
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                      esq = true;   // left key: PacMan tem intenção de se mover para esquerda
                      dir = cima = baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                      dir = true;   // right key: PacMan tem intenção de se mover para direita
                      esq = cima = baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                      cima = true;   // up key: PacMan tem intenção de se mover para cima
                      esq = dir = baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                      baixo = true;   // down key: PacMan tem intenção de se mover para baixo
                      esq = dir = cima = false;
                  }
            }
        }

         posxf += (posx - posxf ) * 0.2f;
         posyf += (posy - posyf) * 0.2f;
         //posicao de renderização 

        // Muda a posição do PacMan a cada 0.17 segundos
        if(relogioAnimacao.getElapsedTime() > sf::seconds(0.17)) {
            relogioAnimacao.restart();
            if (cima && mapa[posy-1][posx] != '1') posy --;     
            if (baixo && mapa[posy+1][posx] != '1') posy ++;
            if (esq && mapa[posy][posx-1] != '1') posx --;
            if (dir && mapa[posy][posx+1] != '1') posx ++;

        }
        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<31;i++)
            for(int j=0;j<29;j++) {
                if(mapa[i][j] == '1') {
                if(mapa[i-1][j] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, i*SIZE});
                quad.setSize({34, 2});
                window.draw(quad);
                }
                if(mapa[i+1][j] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, i*SIZE + 32});
                quad.setSize({34, 2});
                window.draw(quad);
                } if(mapa[i][j-1] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, i*SIZE});
                quad.setSize({2, 34});
                window.draw(quad);
                } if (mapa[i][j+1] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE + 32, i*SIZE});
                quad.setSize({2,34});
                window.draw(quad);
                }
            }

            }
                

        // desenha PacMan
         sprite.setPosition({xdeslocamento + posxf*SIZE, ydeslocamento +posyf*SIZE}); //o que fizer no desenho tem que fazer aqui
        // para renderização dos espaços e a posição dele baterem
        window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
