#ifndef JOGO_DA_VELHA_H
#define JOGO_DA_VELHA_H

int offline();
int jogada_off(char tabuleiro[][3], int jogador);
int checa_jogada_off(char tabuleiro[][3], char escolha[]);
void print_tabuleiro_off(char tabuleiro[][3]);
int checa_vitoria_off(char tabuleiro[][3], int jogador);
int checa_empate_off(char tabuleiro[][3]);

#endif