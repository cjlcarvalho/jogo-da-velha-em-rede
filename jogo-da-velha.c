#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int offline();
int jogada_off(char tabuleiro[][3], int jogador);
int checa_jogada_off(char tabuleiro[][3], char escolha[]);
void print_tabuleiro_off(char tabuleiro[][3]);
int checa_vitoria_off(char tabuleiro[][3], int jogador);
int checa_empate_off(char tabuleiro[][3]);

int offline(){
	int cont = 1;
	char tabuleiro[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

	while(cont){
		jogada_off(tabuleiro, (cont%2) == 0 ? 2 : 1);
		if(checa_vitoria_off(tabuleiro, (cont%2) == 0 ? 2 : 1)){
			printf("Vitória do jogador %d. Fim de jogo.\n", (cont%2) == 0 ? 2 : 1);
			return 0;
		}
		if(checa_empate_off(tabuleiro)){
			printf("Empate.\n");
			return 0;
		}
		cont++;
	}
	
	return 0;
}

int jogada_off(char tabuleiro[][3], int jogador){
	char escolha[2];
	char letra = (jogador == 1) ? 'X' : 'O';
	int permitido = 0;
	
	while(!permitido){
		printf("Escolha do jogador %d: ", jogador);
		scanf(" %s", escolha);
		if((escolha[0] >= 65 && escolha[0] <= 67) && (escolha[1] >= 49 && escolha[1] <= 51)){
			if(strlen(escolha) == 2){
				if(checa_jogada_off(tabuleiro, escolha)){
					permitido = 1;
					tabuleiro[escolha[0] - 65][escolha[1] - 49] = letra;
					print_tabuleiro_off(tabuleiro);
				}
				else{
					printf("Local preenchido.\n");
				}
			}
			else
				printf("Escolha incorreta, tente novamente.\n");
		}
		else
			printf("Escolha incorreta, tente novamente.\n");
	}
	return permitido;
}

int checa_jogada_off(char tabuleiro[][3], char escolha[]){
	return (tabuleiro[escolha[0] - 65][escolha[1] - 49] == ' ') ? 1 : 0;
}

void print_tabuleiro_off(char tabuleiro[][3]){
	int i = 0, j = 0;
	while(i<3){
		while(j<3){
			printf("%c ", tabuleiro[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

int checa_vitoria_off(char tabuleiro[][3], int jogador){
	char letra = (jogador == 1) ? 'X' : 'O';
	if((tabuleiro[0][0] == letra && tabuleiro[1][0] == letra && tabuleiro[2][0] == letra) || \
	   (tabuleiro[0][1] == letra && tabuleiro[1][1] == letra && tabuleiro[2][1] == letra) || \
	   (tabuleiro[0][2] == letra && tabuleiro[1][2] == letra && tabuleiro[2][2] == letra) || \
	   (tabuleiro[0][0] == letra && tabuleiro[0][1] == letra && tabuleiro[0][2] == letra) || \
	   (tabuleiro[1][0] == letra && tabuleiro[1][1] == letra && tabuleiro[1][2] == letra) || \
	   (tabuleiro[2][0] == letra && tabuleiro[2][1] == letra && tabuleiro[2][2] == letra) || \
	   (tabuleiro[0][0] == letra && tabuleiro[1][1] == letra && tabuleiro[2][2] == letra) || \
	   (tabuleiro[2][0] == letra && tabuleiro[1][1] == letra && tabuleiro[0][2] == letra))
		return 1;
	return 0;
}

int checa_empate_off(char tabuleiro[][3]){
	int i = 0, j = 0;
	while(i < 3){
		while(j < 3){
			if(tabuleiro[i][j] == ' ')
				return 0;
			j++;
		}
		i++;
	}
	return 1;
}