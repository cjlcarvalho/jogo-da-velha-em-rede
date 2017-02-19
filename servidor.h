#ifndef SERVIDOR_H
#define SERVIDOR_H

int start_server();
int jogada_srv(char tabuleiro[], int jogador);
int checa_jogada_srv(char tabuleiro[], char escolha[], int* n);
void print_tabuleiro_srv(char tabuleiro[]);
int checa_vitoria_srv(char tabuleiro[], int jogador);
int checa_empate_srv(char tabuleiro[]);

#endif