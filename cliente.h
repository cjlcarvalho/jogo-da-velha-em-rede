#ifndef CLIENTE_H
#define CLIENTE_H

int start_client();
int jogada_cli(char tabuleiro[], int jogador);
int checa_jogada_cli(char tabuleiro[], char escolha[], int* n);
void print_tabuleiro_cli(char tabuleiro[]);
int checa_vitoria_cli(char tabuleiro[], int jogador);
int checa_empate_cli(char tabuleiro[]);
int read_in_cli(int socket, char buf[], int len);

#endif