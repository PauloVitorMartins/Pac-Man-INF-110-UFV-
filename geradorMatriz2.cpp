#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    // Matriz base 21x22 corrigida.
    vector<string> base = {
        "111111111111111111111", // 0: Parede superior
        "100000000010000000001", // 1: Caminho
        "101101111010111101101", // 2: 4 Blocos superiores (Externo estreito, Interno largo)
        "101101111010111101101", // 3: Linha DUPLICADA apenas aqui, para dar espessura aos blocos de cima
        "100000000000000000001", // 4: Caminho
        "101101011111110101101", // 5: Blocos laterais e T-shape (Linha ÚNICA = Parede fina)
        "100001000010000100001", // 6: Caminho
        "111101111010111101111", // 7: Base do T
        "111101000000000101111", // 8: Caminho ao redor da base
        "111101011101110101111", // 9: Teto da casa dos fantasmas
        "000001000000000100000", // 10: Túnel de teletransporte (com saída para as bordas)
        "111101011111110101111", // 11: Chão da casa dos fantasmas
        "111101000000000101111", // 12: Caminho
        "111101011111110101111", // 13: T-shape inferior
        "100000000010000000001", // 14: Caminho
        "101101111010111101101", // 15: Blocos inferiores (Linha ÚNICA = Parede fina)
        "100001000000000100001", // 16: Caminho
        "111101011111110101111", // 17: Paredes em L e base
        "100001000010000100001", // 18: Caminho
        "101111111010111111101", // 19: Parede inferior longa (Linha ÚNICA)
        "100000000000000000001", // 20: Caminho final
        "111111111111111111111"  // 21: Borda de baixo
    };

    int R = base.size();
    int C = base[0].size();
    int nR = 2 * R - 1;
    int nC = 2 * C - 1;

    vector<string> nova(nR, string(nC, ' '));

    for (int r = 0; r < nR; r++) {
        for (int c = 0; c < nC; c++) {
            if (r % 2 == 0 && c % 2 == 0) {
                nova[r][c] = base[r/2][c/2];
            } else if (r % 2 == 0 && c % 2 != 0) {
                char esq = base[r/2][c/2];
                char dir = base[r/2][c/2 + 1];
                if (esq == dir) nova[r][c] = esq;
                else nova[r][c] = '5';
            } else if (r % 2 != 0 && c % 2 == 0) {
                char cima = base[r/2][c/2];
                char baixo = base[r/2 + 1][c/2];
                if (cima == baixo) nova[r][c] = cima;
                else nova[r][c] = '5';
            } else {
                char c1 = base[r/2][c/2];
                char c2 = base[r/2][c/2 + 1];
                char c3 = base[r/2 + 1][c/2];
                char c4 = base[r/2 + 1][c/2 + 1];
                if (c1 == c2 && c2 == c3 && c3 == c4) nova[r][c] = c1;
                else nova[r][c] = '5';
            }
        }
    }

    // Saída C/C++ da matriz atualizada
    cout << "char mapa[" << nR << "][" << nC + 1 << "] = {\n";
    for (int r = 0; r < nR; r++) {
        cout << "  \"" << nova[r] << "\"";
        if (r < nR - 1) cout << ",";
        cout << "\n";
    }
    cout << "};\n";

    return 0;
}