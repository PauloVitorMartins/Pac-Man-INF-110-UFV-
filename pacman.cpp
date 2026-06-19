#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <iostream>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   não implementada
// Intenção de movimento: não implementado
// Atualizado para 3.1.0: André Gustavo   03/06/26
// Mapa Clássico do Pac-Man (Recriado com Padding)
// Dimensões: 31 linhas x 29 colunas
// 1 = Parede Azul (Tile 34x34)
// 5 = Preenchimento Preto (Borda visual)
// 0 = Caminho transitável (Conectado, não renderizado)

sf::Texture retornaTextura(std::string s) {
    sf::Texture texture;
    if (!texture.loadFromFile(s)) {
        std::cout << "Erro lendo imagem: " << s << "\n";
    }
    return texture;
}

const int linhas = 42;
const int colunas = 39;

char mapa[42][40] = {
  "111111111111111111111111111111111111111",
  "155555555555555555515555555555555555551",
  "153000000000000000515000000000000000351",
  "150555555055555550515055555550555555051",
  "150511115051111150515051111150511115051",
  "150511115051111150515051111150511115051",
  "150555555055555550555055555550555555051",
  "150000000000000000000000000000000000051",
  "150555555055505555555555505550555555051",
  "150511115051505111111111505150511115051",
  "150555555051505555515555505150555555051",
  "153000000051500000515000005150000000351",
  "155555555051555550515055555150555555551",
  "111111115051111150515051111150511111111",
  "111111115051555550555055555150511111111",
  "111111115051500000000000005150511111111",
  "111111115051505555555555505150511111111",
  "111111115051505111555111505150511111111",
  "555555555055505155555551505550555555555",
  "988000000000005152222251500000000000889",
  "555555555055505155555551505550555555555",
  "111111115051505111111111505150511111111",
  "111111115051505555555555505150511111111",
  "111111115051500000000000005150511111111",
  "111111115051505555555555505150511111111",
  "111111115051505111111111505150511111111",
  "155555555055505555515555505550555555551",
  "153000000000000000515000000000000000351",
  "150555555055555550515055555550555555051",
  "150511115051111150515051111150511115051",
  "150555515055555550555055555550515555051",
  "150000515000000000000000000000515000051",
  "155550515055505555555555505550515055551",
  "111150515051505111111111505150515051111",
  "155550555051505555515555505150555055551",
  "150000000051500000515000005150000000051",
  "150555555551555550515055555155555555051",
  "150511111111111150515051111111111115051",
  "150555555555555550555055555555555555051",
  "153000000000000000000000000000000000351",
  "155555555555555555555555555555555555551",
  "111111111111111111111111111111111111111"
};
const float larguraTela = 1920.0f;
const float alturaTela = 1080.0f;
const float SIZE = 22;
const float tamanhoBolinhaPequena = SIZE * 0.15f; 
const float tamanhoBolinhaGrande = SIZE * 0.35f;
const float tamanhoPac = SIZE * 2.4f;
float velocidade = 0.2f;

float sizeQuadBlack = SIZE*3;

bool passouTunel = false;

int posxghost =  15;
int posyghost =  10;

float posxf = 9.0f; //posicao relativa fluida
float posyf = 7.0f; 
float xdeslocamento = (larguraTela - (colunas*SIZE))/2.0f;
float ydeslocamento = (alturaTela - (linhas*SIZE))/2.0f;

float espessuraParede = SIZE * 0.1f;

bool cima = false;  // direcao de movimento do PacMan
bool baixo = false;
bool esq = false;
bool dir = false;

bool isMoving = false;

int score=0; //pontuação
bool intencao_cima = false;
bool intencao_baixo = false;
bool intencao_esq = false;
bool intencao_dir = false;

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Pac-Man", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    // cria um quadrado de tamanho 50 (a parede)
    sf::RectangleShape quad({SIZE, SIZE});
    sf::RectangleShape quadBlack({sizeQuadBlack, sizeQuadBlack});
    sf::RectangleShape quadBlack1({sizeQuadBlack, sizeQuadBlack});
    quadBlack.setFillColor({0, 0, 0});
    quadBlack1.setFillColor({0, 0, 0});
    quadBlack.setOrigin({sizeQuadBlack/2, sizeQuadBlack/2});
    quadBlack1.setOrigin({sizeQuadBlack/2, sizeQuadBlack/2});

    // cria bolinhas
    sf::CircleShape bolinha({tamanhoBolinhaPequena});
    bolinha.setFillColor({255, 255, 255});
    //cria bolinha maior
    sf::CircleShape BOLA({tamanhoBolinhaGrande});
    BOLA.setFillColor({255, 255, 255});


    sf::Texture fantasmaRedTextura = retornaTextura("./sprites/red.png");
    sf::Sprite fantasmaRed{fantasmaRedTextura};

    // sprites do PacMan
    sf::Texture texture = retornaTextura("./sprites/pacman.png");
    sf::Texture texture1 = retornaTextura("./sprites/pacman1.png");
    sf::Texture texture2 = retornaTextura("./sprites/pacman2.png");
    sf::Sprite sprite(texture);

    float escalaPacman = tamanhoPac / texture.getSize().x;
    sprite.setScale({escalaPacman, escalaPacman});

    sf::Font font; //fonte
    if(!font.openFromFile("emulogic.ttf")){
        std::cout << "Erro lendo fonte emulogic\n";
        return 0;
    }
    sf::Text text(font);
    text.setPosition({0, 0});
    text.setFillColor({255, 255, 255});


    // cria um relogio para medir o tempo do PacMan
    sf::Clock relogioMovimento;
    sf::Clock relogioAnimacao;
    sf::Clock relogioMovimentofantasma;
    sf::Clock relogioAnimacaofantasma;

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
                      intencao_esq = true;   // left key: PacMan tem intenção de se mover para esquerda
                      intencao_dir = intencao_cima = intencao_baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                      intencao_dir = true;   // right key: PacMan tem intenção de se mover para direita
                      intencao_esq = intencao_cima = intencao_baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                      intencao_cima = true;   // up key: PacMan tem intenção de se mover para cima
                      intencao_esq = intencao_dir = intencao_baixo = false;
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                      intencao_baixo = true;   // down key: PacMan tem intenção de se mover para baixo
                      intencao_esq = intencao_dir = intencao_cima = false;
                  }
            }
        }

            float tolerancia = velocidade * 0.55f;
            bool noCentroX = std::abs(posxf - std::round(posxf)) < tolerancia;
            bool noCentroY = std::abs(posyf - std::round(posyf)) < tolerancia;

            if (noCentroX && noCentroY) {

            posxf = std::round(posxf);
            posyf = std::round(posyf);

            int x = (int)posxf;
            int y = (int)posyf;

            if (intencao_cima && mapa[y-1][x] != '5') {
                cima = true; baixo = esq = dir = false;
                isMoving = true;
            } else if (intencao_baixo && mapa[y+1][x] != '5') {
                baixo = true; cima = esq = dir = false;
                isMoving = true;
            } else if (intencao_esq && mapa[y][x-1] != '5') {
                esq = true; cima = baixo = dir = false;
                isMoving = true;
            } else if (intencao_dir && mapa[y][x+1] != '5') {
                dir = true; cima = baixo = esq = false;
                isMoving = true;
            }

            if(y == 19 && x == 38 && passouTunel == false) {
                posyf = 19;
                posxf = 0;
                x = 0;
                passouTunel = true;
            }

            if(y == 19 && x == 0 && passouTunel == false) {
                posyf = 19;
                posxf = 38;
                passouTunel = true;
            }

            if (y == 19 && x == 37) {
                passouTunel = false;
            }

            if (y == 19 && x == 1) {
                passouTunel = false;
            }

            if(mapa[y][x]=='0'){
                mapa[y][x]='2';
                score+=10;
            }
            else if(mapa[y][x]=='3'){
                mapa[y][x]='2';
                score+=50;
            }
            
            if (cima && mapa[y-1][x] == '5') { cima = false; isMoving = false;}
            if (baixo && mapa[y+1][x] == '5') { baixo = false; isMoving = false;}
            if (esq && mapa[y][x-1] == '5') { esq = false; isMoving = false;}
            if (dir && mapa[y][x+1] == '5') { dir = false; isMoving = false;}

            }
         
            if (cima) { posyf -= velocidade; sprite.setRotation(sf::degrees(270));}
            if (baixo) { posyf += velocidade; sprite.setRotation(sf::degrees(90));}
            if (esq) { posxf -= velocidade; sprite.setRotation(sf::degrees(180));}
            if (dir) { posxf += velocidade; sprite.setRotation(sf::degrees(0));}
            
        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        // desenha fantasmaRed
        // fantasmaRed.setPosition({xdeslocamento + posxghost*SIZE + SIZE/2, ydeslocamento + posyghost*SIZE + SIZE/2}); //o que fizer no desenho tem que fazer aqui
        // // para renderização dos espaços e a posição dele baterem
        // window.draw(fantasmaRed);

        // desenha paredes
        for(int i=0;i<linhas;i++)
            for(int j=0;j<colunas;j++) {
            if(mapa[i][j] == '1') {
                if(mapa[i-1][j] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, ydeslocamento+ i*SIZE});
                quad.setSize({SIZE, espessuraParede});
                window.draw(quad);
                }
                if(mapa[i+1][j] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, ydeslocamento+ i*SIZE + SIZE - espessuraParede});
                quad.setSize({SIZE, espessuraParede});
                window.draw(quad);
                } if(mapa[i][j-1] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE, ydeslocamento+ i*SIZE});
                quad.setSize({espessuraParede, SIZE});
                window.draw(quad);
                } if (mapa[i][j+1] != '1') {
                quad.setPosition({xdeslocamento + j*SIZE + SIZE - espessuraParede, ydeslocamento+ i*SIZE});
                quad.setSize({espessuraParede,SIZE});
                window.draw(quad);
                }
            }
            if(mapa[i][j] == '0'){
                bolinha.setOrigin({tamanhoBolinhaPequena, tamanhoBolinhaPequena}); 
                bolinha.setPosition({xdeslocamento + j*SIZE + SIZE/2, ydeslocamento + i*SIZE + SIZE/2});
                window.draw(bolinha);
            }
            else if(mapa[i][j] == '3'){
                BOLA.setOrigin({tamanhoBolinhaGrande, tamanhoBolinhaGrande});
                BOLA.setPosition({xdeslocamento + j*SIZE + SIZE/2, ydeslocamento + i*SIZE + SIZE/2});
                window.draw(BOLA);
            }
            }
        
        if(isMoving == true) {
            if(relogioAnimacao.getElapsedTime() < sf::seconds(0.08)) {
                sprite.setTexture(texture1);
            } else if (relogioAnimacao.getElapsedTime() >= sf::seconds(0.16) && relogioAnimacao.getElapsedTime() < sf::seconds(0.24)) {
                sprite.setTexture(texture2);
            }  else if (relogioAnimacao.getElapsedTime() >= sf::seconds(0.24)) {
                sprite.setTexture(texture);
                relogioAnimacao.restart();
            }
       } else {
         sprite.setTexture(texture);
       }
                   

        // desenha PacMan
        sprite.setOrigin({texture.getSize().x / 2.0f, texture.getSize().y / 2.0f});
        sprite.setPosition({xdeslocamento + posxf*SIZE + SIZE/2, ydeslocamento +posyf*SIZE + SIZE/2}); //o que fizer no desenho tem que fazer aqui
        // para renderização dos espaços e a posição dele baterem
        window.draw(sprite);
        //desenha o score
        text.setString(std::to_string(score));
        window.draw(text);

        quadBlack.setPosition({xdeslocamento + 38*SIZE, ydeslocamento + 19*SIZE + SIZE / 2.0f});
        window.draw(quadBlack);
        quadBlack1.setPosition({xdeslocamento + SIZE, ydeslocamento + 19*SIZE + SIZE / 2.0f });
        window.draw(quadBlack1);


        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
