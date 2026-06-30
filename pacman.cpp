#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

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

int posxghostb =  16;
int posyghostb =  19;

int posxghosta =  18;
int posyghosta =  19;

int posxghostw =  20;
int posyghostw =  19;

int posxghostm =  22;
int posyghostm =  19;

bool isOutQuadradoB = false;
int dirBaidu = 0;

bool isOutQuadradoA = false;
int dirAvast = 0;

bool isOutQuadradoW = false;
int dirW = 0;

bool isOutQuadradoM = false;
int dirM = 0;

float posxf = 19.0f; //posicao relativa fluida
float posyf = 23.0f;
float posxf_ghostb = 16.0f;
float posyf_ghostb = 19.0f;
float posxf_ghosta = 18.0f;
float posyf_ghosta = 19.0f;
float posxf_ghostw = 20.0f;
float posyf_ghostw = 19.0f;
float posxf_ghostm = 22.0f;
float posyf_ghostm = 19.0f;
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
    quad.setFillColor({57, 255, 20});
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

    // sprites do PacMan
    sf::Texture texture = retornaTextura("./sprites/virusdireita.png");
    sf::Texture texture1 = retornaTextura("./sprites/virusdireitagiro.png");
    sf::Texture texture2 = retornaTextura("./sprites/virusdireitaapagado.png");
    sf::Sprite sprite(texture);

    float escalaPacman = tamanhoPac / texture.getSize().x;
    sprite.setScale({escalaPacman, escalaPacman});


     //FANTASMAS SPRITE

    sf::Texture baidutextura = retornaTextura("./sprites/baidu.png");
    sf::Texture baidutextura1 = retornaTextura("./sprites/baidu1.png");
    sf::Texture baidutextura2 = retornaTextura("./sprites/baidu2.png");
    sf::Sprite fantasmabaidu{baidutextura};

    sf::Texture avasttextura = retornaTextura("./sprites/avast.png");
    sf::Texture avasttextura1 = retornaTextura("./sprites/avast1.png");
    sf::Texture avasttextura2 = retornaTextura("./sprites/avast2.png");
    sf::Sprite fantasmaavast{avasttextura};

    sf::Texture wintextura = retornaTextura("./sprites/win.png");
    sf::Texture wintextura1 = retornaTextura("./sprites/win1.png");
    sf::Texture wintextura2 = retornaTextura("./sprites/win2.png");
    sf::Sprite fantasmawin{wintextura};

    sf::Texture mctextura = retornaTextura("./sprites/mc.png");
    sf::Texture mctextura1 = retornaTextura("./sprites/mc1.png");
    sf::Texture mctextura2 = retornaTextura("./sprites/mc2.png");
    sf::Sprite fantasmamc{mctextura};

    // Define o tamanho desejado em pixels
    float tamanhoFantasma = 42.0f;

    float escalaFantasma1x = tamanhoFantasma / mctextura.getSize().x;
    float escalaFantasma1y = tamanhoFantasma / mctextura.getSize().y;

    float escalaFantasma2x = tamanhoFantasma / avasttextura.getSize().x;
    float escalaFantasma2y = tamanhoFantasma / avasttextura.getSize().y;

    float escalaFantasma3x = tamanhoFantasma / wintextura.getSize().x;
    float escalaFantasma3y = tamanhoFantasma / wintextura.getSize().y;

    float escalaFantasma4x = tamanhoFantasma / baidutextura.getSize().x;
    float escalaFantasma4y = tamanhoFantasma / baidutextura.getSize().y;

    fantasmawin.setScale({escalaFantasma3x, escalaFantasma3y});

    fantasmabaidu.setScale({escalaFantasma4x, escalaFantasma4y});

    fantasmamc.setScale({escalaFantasma1x, escalaFantasma1y});

    fantasmaavast.setScale({escalaFantasma2x, escalaFantasma2y});

    fantasmamc.setOrigin({mctextura.getSize().x / 2.0f, mctextura.getSize().y / 2.0f});

    fantasmabaidu.setOrigin({baidutextura.getSize().x / 2.0f, baidutextura.getSize().y / 2.0f});
    
    fantasmaavast.setOrigin({avasttextura.getSize().x / 2.0f, avasttextura.getSize().y / 2.0f});
    
    fantasmawin.setOrigin({wintextura.getSize().x / 2.0f, wintextura.getSize().y / 2.0f});


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

    srand(time(NULL));


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
        sf::Time tempoFantasma = relogioAnimacaofantasma.getElapsedTime();

        if (tempoFantasma < sf::seconds(0.50f)) {
            fantasmamc.setTexture(mctextura);
            fantasmabaidu.setTexture(baidutextura);
            fantasmaavast.setTexture(avasttextura);
            fantasmawin.setTexture(wintextura);
        } 
        else if (tempoFantasma >= sf::seconds(0.50f) && tempoFantasma < sf::seconds(1.00f)) {
            fantasmamc.setTexture(mctextura1);
            fantasmabaidu.setTexture(baidutextura1);
            fantasmaavast.setTexture(avasttextura1);
            fantasmawin.setTexture(wintextura1);
        } 
        else if (tempoFantasma >= sf::seconds(1.00f) && tempoFantasma < sf::seconds(1.50f)) {
            fantasmamc.setTexture(mctextura2);
            fantasmabaidu.setTexture(baidutextura2);
            fantasmaavast.setTexture(avasttextura2);
            fantasmawin.setTexture(wintextura2);
        } 
        else if (tempoFantasma >= sf::seconds(1.50)) {
            // Reseta o relógio UMA ÚNICA VEZ para o loop recomeçar
            relogioAnimacaofantasma.restart();
        }

        // desenha fantasmabaidu
        fantasmabaidu.setPosition({xdeslocamento + posxf_ghostb*SIZE + SIZE/2, ydeslocamento + posyf_ghostb*SIZE + SIZE/2});
        window.draw(fantasmabaidu);

        fantasmawin.setPosition({xdeslocamento + posxf_ghostw*SIZE + SIZE/2, ydeslocamento + posyf_ghostw*SIZE + SIZE/2}); //o que fizer no desenho tem que fazer aqui
        // // para renderização dos espaços e a posição dele baterem
        window.draw(fantasmawin);

        fantasmaavast.setPosition({xdeslocamento + posxf_ghosta*SIZE + SIZE/2, ydeslocamento + posyf_ghosta*SIZE + SIZE/2});
        // // para renderização dos espaços e a posição dele baterem
        window.draw(fantasmaavast);

        fantasmamc.setPosition({xdeslocamento + posxf_ghostm*SIZE + SIZE/2, ydeslocamento + posyf_ghostm*SIZE + SIZE/2}); //o que fizer no desenho tem que fazer aqui
        // // para renderização dos espaços e a posição dele baterem
        window.draw(fantasmamc);

        bool noCentroXB = std::abs(posxf_ghostb - std::round(posxf_ghostb)) < tolerancia;
        bool noCentroYB = std::abs(posyf_ghostb - std::round(posyf_ghostb)) < tolerancia;

        bool noCentroXA = std::abs(posxf_ghosta - std::round(posxf_ghosta)) < tolerancia;
        bool noCentroYA = std::abs(posyf_ghosta - std::round(posyf_ghosta)) < tolerancia;

        bool noCentroXW = std::abs(posxf_ghostw - std::round(posxf_ghostw)) < tolerancia;
        bool noCentroYW = std::abs(posyf_ghostw - std::round(posyf_ghostw)) < tolerancia;

        bool noCentroXM = std::abs(posxf_ghostm - std::round(posxf_ghostm)) < tolerancia;
        bool noCentroYM = std::abs(posyf_ghostm - std::round(posyf_ghostm)) < tolerancia;

if (!isOutQuadradoW) {
            posyf_ghostw -= velocidade; 
            if (posyf_ghostw <= 15.0f) {
                posyf_ghostw = 15.0f;     
                isOutQuadradoW = true;    
                dirW = 2;             
            }
        } else {
            // Avalia as opções no centro do bloco
            if (noCentroXW && noCentroYW) {
        
                posxf_ghostw = std::round(posxf_ghostw);
                posyf_ghostw = std::round(posyf_ghostw);
            
                int xw = (int)posxf_ghostw;
                int yw = (int)posyf_ghostw;
                
                // CORREÇÃO DO TÚNEL PARA O WIN
                if (yw == 19 && xw <= 0 && dirW == 2) {
                    posxf_ghostw = 38.0f;
                    xw = 38;
                } else if (yw == 19 && xw >= 38 && dirW == 3) {
                    posxf_ghostw = 0.0f;
                    xw = 0;
                }

                int direcaoOposta = -1;
                if (dirW == 0) direcaoOposta = 1; 
                if (dirW == 1) direcaoOposta = 0; 
                if (dirW == 2) direcaoOposta = 3; 
                if (dirW == 3) direcaoOposta = 2; 

                int direcoesValidas[4];
                int numDirecoes = 0; 
            
                if (mapa[yw-1][xw] != '5' && direcaoOposta != 0) {
                    direcoesValidas[numDirecoes] = 0; 
                    numDirecoes++;
                }
                if (mapa[yw+1][xw] != '5' && direcaoOposta != 1) {
                    direcoesValidas[numDirecoes] = 1; 
                    numDirecoes++;
                }
                if (mapa[yw][xw-1] != '5' && direcaoOposta != 2) {
                    direcoesValidas[numDirecoes] = 2; 
                    numDirecoes++;
                }
                if (mapa[yw][xw+1] != '5' && direcaoOposta != 3) {
                    direcoesValidas[numDirecoes] = 3; 
                    numDirecoes++;
                }

                if (numDirecoes == 0) {
                    direcoesValidas[0] = direcaoOposta;
                    numDirecoes = 1;
                }

                int indiceSorteado = rand() % numDirecoes;
                dirW = direcoesValidas[indiceSorteado];
            }

            // Aplica o movimento
            if (dirW == 0) posyf_ghostw -= velocidade;
            if (dirW == 1) posyf_ghostw += velocidade;
            if (dirW == 2) posxf_ghostw -= velocidade;
            if (dirW == 3) posxf_ghostw += velocidade;
        }

        if (!isOutQuadradoM) {
            posyf_ghostm -= velocidade; 
            if (posyf_ghostm <= 15.0f) {
                posyf_ghostm = 15.0f;     
                isOutQuadradoM = true;    
                dirM = 2;             
            }
        } else {
        // TODA A VEZ que ele entra no centro de um novo bloco, ele avalia as opções
            if (noCentroXM && noCentroYM) {
        
            // Trava no centro exato para evitar desvios igual a logica q fiz do pacman
            posxf_ghostm = std::round(posxf_ghostm);
            posyf_ghostm = std::round(posyf_ghostm);
        
            int xm = (int)posxf_ghostm;
            int ym = (int)posyf_ghostm;
            
            if (ym == 19 && xm <= 0 && dirM == 2) {
                posxf_ghostb = 38.0f;
                xm = 38;
            } else if (ym == 19 && xm >= 38 && dirM == 3) {
                posxf_ghostw = 0.0f;
                xm = 0;
            }

            // ta memorizando a direcao oposta (de onde ele veio) para NUNCA dar meia-volta
            int direcaoOposta = -1;
            if (dirM == 0) direcaoOposta = 1; // Se vai para cima, não pode voltar para baixo
            if (dirM == 1) direcaoOposta = 0; // Se vai para baixo, não pode voltar para cima
            if (dirM == 2) direcaoOposta = 3; // Se vai para a esq, não pode voltar para a dir
            if (dirM == 3) direcaoOposta = 2; // Se vai para a dir, não pode voltar para a esq

            int direcoesValidas[4];
            int numDirecoes = 0; // Contador de quantas rotas estão livres
        

            if (mapa[ym-1][xm] != '5' && direcaoOposta != 0) {
                direcoesValidas[numDirecoes] = 0; // Cima
                numDirecoes++;
            }
            if (mapa[ym+1][xm] != '5' && direcaoOposta != 1) {
                direcoesValidas[numDirecoes] = 1; // Baixo
                numDirecoes++;
            }
            if (mapa[ym][xm-1] != '5' && direcaoOposta != 2) {
                direcoesValidas[numDirecoes] = 2; // Esquerda
                numDirecoes++;
            }
            if (mapa[ym][xm+1] != '5' && direcaoOposta != 3) {
                direcoesValidas[numDirecoes] = 3; // Direita
                numDirecoes++;
            }

            if (numDirecoes == 0) {
                direcoesValidas[0] = direcaoOposta;
                numDirecoes = 1;
            }


            int indiceSorteado = rand() % numDirecoes;
            dirM = direcoesValidas[indiceSorteado];
            }

            if (dirM == 0) posyf_ghostm -= velocidade;
            if (dirM == 1) posyf_ghostm += velocidade;
            if (dirM == 2) posxf_ghostm -= velocidade;
            if (dirM == 3) posxf_ghostm += velocidade;
    }


        if (!isOutQuadradoB) {
            posyf_ghostb -= velocidade; 
            if (posyf_ghostb <= 15.0f) {
                posyf_ghostb = 15.0f;     
                isOutQuadradoB = true;    
                dirBaidu = 2;             
            }
        } else {
        // TODA A VEZ que ele entra no centro de um novo bloco, ele avalia as opções
            if (noCentroXB && noCentroYB) {
        
            // Trava no centro exato para evitar desvios igual a logica q fiz do pacman
            posxf_ghostb = std::round(posxf_ghostb);
            posyf_ghostb = std::round(posyf_ghostb);
        
            int xb = (int)posxf_ghostb;
            int yb = (int)posyf_ghostb;
            
            if (yb == 19 && xb <= 0 && dirBaidu == 2) {
                posxf_ghostb = 38.0f;
                xb = 38;
            } else if (yb == 19 && xb >= 38 && dirBaidu == 3) {
                posxf_ghostb = 0.0f;
                xb = 0;
            }

            // ta memorizando a direcao oposta (de onde ele veio) para NUNCA dar meia-volta
            int direcaoOposta = -1;
            if (dirBaidu == 0) direcaoOposta = 1; // Se vai para cima, não pode voltar para baixo
            if (dirBaidu == 1) direcaoOposta = 0; // Se vai para baixo, não pode voltar para cima
            if (dirBaidu == 2) direcaoOposta = 3; // Se vai para a esq, não pode voltar para a dir
            if (dirBaidu == 3) direcaoOposta = 2; // Se vai para a dir, não pode voltar para a esq

            int direcoesValidas[4];
            int numDirecoes = 0; // Contador de quantas rotas estão livres
        

            if (mapa[yb-1][xb] != '5' && direcaoOposta != 0) {
                direcoesValidas[numDirecoes] = 0; // Cima
                numDirecoes++;
            }
            if (mapa[yb+1][xb] != '5' && direcaoOposta != 1) {
                direcoesValidas[numDirecoes] = 1; // Baixo
                numDirecoes++;
            }
            if (mapa[yb][xb-1] != '5' && direcaoOposta != 2) {
                direcoesValidas[numDirecoes] = 2; // Esquerda
                numDirecoes++;
            }
            if (mapa[yb][xb+1] != '5' && direcaoOposta != 3) {
                direcoesValidas[numDirecoes] = 3; // Direita
                numDirecoes++;
            }

            if (numDirecoes == 0) {
                direcoesValidas[0] = direcaoOposta;
                numDirecoes = 1;
            }


            int indiceSorteado = rand() % numDirecoes;
            dirBaidu = direcoesValidas[indiceSorteado];
            }

            if (dirBaidu == 0) posyf_ghostb -= velocidade;
            if (dirBaidu == 1) posyf_ghostb += velocidade;
            if (dirBaidu == 2) posxf_ghostb -= velocidade;
            if (dirBaidu == 3) posxf_ghostb += velocidade;
    }

    if (!isOutQuadradoA) {
            posyf_ghosta -= velocidade; 
            if (posyf_ghosta <= 15.0f) {
                posyf_ghosta = 15.0f;     
                isOutQuadradoA = true;    
                dirAvast = 2;             
            }
        } else {
        // TODA A VEZ que ele entra no centro de um novo bloco, ele avalia as opções
            if (noCentroXA && noCentroYA) {
        
            // Trava no centro exato para evitar desvios igual a logica q fiz do pacman
            posxf_ghosta = std::round(posxf_ghosta);
            posyf_ghosta = std::round(posyf_ghosta);
        
            int xa = (int)posxf_ghosta;
            int ya = (int)posyf_ghosta;

            if (ya == 19 && xa <= 0 && dirAvast == 2) {
                posxf_ghosta = 38.0f;
                xa = 38;
            } else if (ya == 19 && xa >= 38 && dirAvast == 3) {
                posxf_ghosta = 0.0f;
                xa = 0;
            }

            // ta memorizando a direcao oposta (de onde ele veio) para NUNCA dar meia-volta
            int direcaoOposta = -1;
            if (dirAvast == 0) direcaoOposta = 1; // Se vai para cima, não pode voltar para baixo
            if (dirAvast == 1) direcaoOposta = 0; // Se vai para baixo, não pode voltar para cima
            if (dirAvast == 2) direcaoOposta = 3; // Se vai para a esq, não pode voltar para a dir
            if (dirAvast == 3) direcaoOposta = 2; // Se vai para a dir, não pode voltar para a esq

            int direcoesValidas[4];
            int numDirecoes = 0; // Contador de quantas rotas estão livres
        

            if (mapa[ya-1][xa] != '5' && direcaoOposta != 0) {
                direcoesValidas[numDirecoes] = 0; // Cima
                numDirecoes++;
            }
            if (mapa[ya+1][xa] != '5' && direcaoOposta != 1) {
                direcoesValidas[numDirecoes] = 1; // Baixo
                numDirecoes++;
            }
            if (mapa[ya][xa-1] != '5' && direcaoOposta != 2) {
                direcoesValidas[numDirecoes] = 2; // Esquerda
                numDirecoes++;
            }
            if (mapa[ya][xa+1] != '5' && direcaoOposta != 3) {
                direcoesValidas[numDirecoes] = 3; // Direita
                numDirecoes++;
            }

            if (numDirecoes == 0) {
                direcoesValidas[0] = direcaoOposta;
                numDirecoes = 1;
            }


            int indiceSorteado = rand() % numDirecoes;
            dirAvast = direcoesValidas[indiceSorteado];
            }

            if (dirAvast== 0) posyf_ghosta -= velocidade;
            if (dirAvast == 1) posyf_ghosta += velocidade;
            if (dirAvast == 2) posxf_ghosta -= velocidade;
            if (dirAvast == 3) posxf_ghosta += velocidade;
    } 
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
