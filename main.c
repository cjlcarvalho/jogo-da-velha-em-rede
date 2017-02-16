#include <stdio.h>
#include <stdlib.h>
#include "servidor.h"
#include "cliente.h"
#include "jogo-da-velha.h"

int menu();

int main(){
	int escolha;
	escolha = menu();
	switch(escolha){
		case 1: offline(); break;
		case 2: start_server(); break;
		case 3: start_client(); break;
		default: break;
	}
	return 0;
}
int menu(){
	int escolha;
	puts("Bem vindo ao jogo da velha.");
	puts("REGRAS DO JOGO:");
	puts("1 - Cada jogador joga uma vez durante a rodada");
	puts("2 - O jogador um irá marcar o tabuleiro com X e o jogador dois irá marcar com O");
	puts("3 - A célula informada deve seguir um padrão linha-coluna, \
com a linha sendo uma letra (A, B, C) e a coluna sendo um número (1, 2, 3)");
	puts("OPÇÕES DE JOGO: ");
	puts("1 - JOGAR CONTRA OUTRO JOGADOR NESSE COMPUTADOR");
	puts("2 - INICIAR PARTIDA CONTRA OUTRO JOGADOR EM OUTRO COMPUTADOR");
	puts("3 - ENTRAR NUMA PARTIDA DE OUTRO JOGADOR EM OUTRO COMPUTADOR");
	printf("Selecione: ");
	scanf(" %d", &escolha);
	printf("\n");
	return escolha;
}