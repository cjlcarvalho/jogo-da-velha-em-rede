#ifdef __WIN32__
#define _WIN32_WINNT 0x0501
#endif

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#ifdef __WIN32__
#include <io.h>
#include <winsock.h>
#include <windows.h>
#define bzero(b, len) (memset((b), '\0', (len)), (void) 0)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

int start_server();
int open_listener_socket();
void bind_to_port(int socket, int port);
int jogada_srv(char tabuleiro[], int jogador);
int checa_jogada_srv(char tabuleiro[], char escolha[], int* n);
void print_tabuleiro_srv(char tabuleiro[]);
int checa_vitoria_srv(char tabuleiro[], int jogador);
int checa_empate_srv(char tabuleiro[]);

int start_server(){
	
	#ifdef __WIN32__
		WSADATA wsaData;
		WSAStartup(MAKEWORD(1, 1), &wsaData);
	#endif
	
	char tabuleiro[11] = "          ";
	// Vincular a uma porta e abrir conexão
	int listener_d = open_listener_socket();
	bind_to_port(listener_d, 30000);

	#ifdef __unix__
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);
	#endif
	
	char buff[11];
	char resposta[4];
	int len, finaliza, connect_d;
	printf("Esperando o jogador 2\n");
	listen(listener_d, 5);

	// Criando conexão
	#ifdef __WIN32__
	connect_d = accept(listener_d, NULL, NULL);
	#else
	connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
	#endif
	// Rodada 1
	// Imprime tabuleiro
	print_tabuleiro_srv(tabuleiro);
	
	// Joga
	jogada_srv(tabuleiro, 1);

	bzero(buff, sizeof(buff));
	strcpy(buff, tabuleiro);
	// Envia jogada	
	#ifdef __WIN32__
	send(connect_d, buff, sizeof(buff), 0);
	#else	
	write(connect_d, buff, sizeof(buff));
	#endif
	
	bzero(buff, sizeof(buff));

	// Espera jogada do jogador 2
	printf("Esperando o jogador 2\n");
	#ifdef __WIN32__
	recv(connect_d, buff, sizeof(buff), 0);
	#else
	read(connect_d, buff, sizeof(buff));
	#endif
	strcpy(tabuleiro, buff);

	// Rodada 2
	// Imprime tabuleiro
	print_tabuleiro_srv(tabuleiro);
	
	// Joga
	jogada_srv(tabuleiro, 1);

	// Envia jogada	
	#ifdef __WIN32__
	send(connect_d, tabuleiro, sizeof(tabuleiro), 0);
	#else
	write(connect_d, tabuleiro, sizeof(tabuleiro));
	#endif	

	bzero(buff, sizeof(buff));

	// Espera jogada do jogador 2
	printf("Esperando o jogador 2\n");
	#ifdef __WIN32__
	recv(connect_d, buff, sizeof(buff), 0);
	#else
	read(connect_d, buff, sizeof(buff));
	#endif
	strcpy(tabuleiro, buff);

	// Rodada 3
	// Imprime tabuleiro
	print_tabuleiro_srv(tabuleiro);
	
	// Joga
	jogada_srv(tabuleiro, 1);

	if(checa_vitoria_srv(tabuleiro, 1)){
		#ifdef __WIN32__
		send(connect_d, "VIT", sizeof("VIT"), 0);
		#else	
		write(connect_d, "VIT", sizeof("VIT"));
		#endif
		printf("Vitória do jogador 1");
		return 0;
	}

	// Envia jogada		
	#ifdef __WIN32__
	send(connect_d, tabuleiro, sizeof(tabuleiro), 0);
	#else
	write(connect_d, tabuleiro, sizeof(tabuleiro));
	#endif	

	bzero(buff, sizeof(buff));

	// Espera jogada do jogador 2
	printf("Esperando o jogador 2\n");
	#ifdef __WIN32__
	recv(connect_d, buff, sizeof(buff), 0);
	#else
	read(connect_d, buff, sizeof(buff));
	#endif
	strcpy(tabuleiro, buff);

	// Rodada 4
	// Imprime tabuleiro
	if(!strcmp(buff, "VIT")){
		printf("Vitória do jogador 2\n");
		return 0;
	}
	print_tabuleiro_srv(tabuleiro);
	
	// Joga
	jogada_srv(tabuleiro, 1);

	if(checa_vitoria_srv(tabuleiro, 1)){
		#ifdef __WIN32__
		send(connect_d, "VIT", sizeof("VIT"), 0);
		#else	
		write(connect_d, "VIT", sizeof("VIT"));
		#endif
		printf("Vitória do jogador 1");
		return 0;
	}

	// Envia jogada		
	#ifdef __WIN32__
	send(connect_d, tabuleiro, sizeof(tabuleiro), 0);
	#else
	write(connect_d, tabuleiro, sizeof(tabuleiro));
	#endif	

	bzero(buff, sizeof(buff));

	// Espera jogada do jogador 2
	printf("Esperando o jogador 2\n");
	#ifdef __WIN32__
	recv(connect_d, buff, sizeof(buff), 0);
	#else
	read(connect_d, buff, sizeof(buff));
	#endif
	strcpy(tabuleiro, buff);

	// Rodada 5
	// Imprime tabuleiro
	if(!strcmp(buff, "VIT")){
		printf("Vitória do jogador 2\n");
		return 0;
	}
	print_tabuleiro_srv(tabuleiro);
	
	// Joga
	jogada_srv(tabuleiro, 1);

	if(checa_vitoria_srv(tabuleiro, 1)){
		#ifdef __WIN32__
		send(connect_d, "VIT", sizeof("VIT"), 0);
		#else	
		write(connect_d, "VIT", sizeof("VIT"));
		#endif
		printf("Vitória do jogador 1");
		return 0;
	}

	// Envia jogada	
	#ifdef __WIN32__
	send(connect_d, tabuleiro, sizeof(tabuleiro), 0);
	#else	
	write(connect_d, tabuleiro, sizeof(tabuleiro));
	#endif	

	printf("Fim de jogo. Empate");
	#ifdef __WIN32__
	send(connect_d, "FIM", sizeof("FIM"), 0);
	#else	
	write(connect_d, "FIM", sizeof("FIM"));
	#endif

	close(connect_d);
	close(listener_d);
	
	return 0;
}

int jogada_srv(char tabuleiro[], int jogador){
	char escolha[2];
	char letra = (jogador == 1) ? 'X' : 'O';
	int permitido = 0;
	int n;
	
	while(!permitido){
		printf("Escolha do jogador %d: ", jogador);
		scanf(" %s", escolha);
		if((escolha[0] >= 65 && escolha[0] <= 67) && (escolha[1] >= 49 && escolha[1] <= 51)){
			if(strlen(escolha) == 2){
				if(checa_jogada_srv(tabuleiro, escolha, &n)){
					permitido = 1;
					tabuleiro[n] = letra;
					print_tabuleiro_srv(tabuleiro);
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

int checa_jogada_srv(char tabuleiro[], char escolha[], int* n){
    *n = 0;
    if(escolha[0] - 65 == 0)
    {
        if(escolha[1] - 48 == 1){
            if(tabuleiro[0] == ' '){
                *n = 0; 
                return 1;
            }
        }
        else if(escolha[1] - 48 == 2){
            if(tabuleiro[1] == ' '){
                *n = 1;
                return 1;
            }
        }
        else if(escolha[1] - 48 == 3){
            if(tabuleiro[2] == ' '){
                *n = 2;
                return 1;
            }
        }
    }
    else if(escolha[0] - 65 == 1){
        if(escolha[1] - 48 == 1){
            if(tabuleiro[3] == ' '){
                *n = 3; 
                return 1;
            }
        }
        else if(escolha[1] - 48 == 2){
            if(tabuleiro[4] == ' '){
                *n = 4;
                return 1;
            }
        }
        else if(escolha[1] - 48 == 3){
            if(tabuleiro[5] == ' '){
                *n = 5;
                return 1;
            }
        }
    }
    else if(escolha[0] - 65 == 2){
        if(escolha[1] - 48 == 1){
            if(tabuleiro[6] == ' '){
                *n = 6; 
                return 1;
            }
        }
        else if(escolha[1] - 48 == 2){
            if(tabuleiro[7] == ' '){
                *n = 7;
                return 1;
            }
        }
        else if(escolha[1] - 48 == 3){
            if(tabuleiro[8] == ' '){
                *n = 8;
                return 1;
            }
        }
    }
    return 0;
}

void print_tabuleiro_srv(char tabuleiro[]){
    int i = 0;

    while(i < strlen(tabuleiro))
    {
        if(tabuleiro[i] == '\0') 
            break;
        if(i != strlen(tabuleiro) - 1)
            printf("%c ", tabuleiro[i]);
        if(i == 2 || i == 5 || i == strlen(tabuleiro) - 1)
            printf("\n");
        i++;
    }
}

int checa_vitoria_srv(char tabuleiro[], int jogador){
	char letra = (jogador == 1) ? 'X' : 'O';
	if((tabuleiro[0] == letra && tabuleiro[1] == letra && tabuleiro[2] == letra) || \
	   (tabuleiro[3] == letra && tabuleiro[4] == letra && tabuleiro[5] == letra) || \
	   (tabuleiro[6] == letra && tabuleiro[7] == letra && tabuleiro[8] == letra) || \
	   (tabuleiro[0] == letra && tabuleiro[3] == letra && tabuleiro[6] == letra) || \
	   (tabuleiro[1] == letra && tabuleiro[4] == letra && tabuleiro[7] == letra) || \
	   (tabuleiro[2] == letra && tabuleiro[5] == letra && tabuleiro[8] == letra) || \
	   (tabuleiro[0] == letra && tabuleiro[4] == letra && tabuleiro[8] == letra) || \
	   (tabuleiro[2] == letra && tabuleiro[4] == letra && tabuleiro[6] == letra))
		return 1;
	return 0;
}

int checa_empate_srv(char tabuleiro[]){
	int i = 1;
	while(i < 10){
		if(tabuleiro[i] == ' ')
			return 0;
		i++;
	}
	return 1;
}

int open_listener_socket(){
	int s = socket(PF_INET, SOCK_STREAM, 0);
	return s;
}

void bind_to_port(int socket, int port){
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	//name.sin_port = (in_port_t)htons(30000);
	name.sin_port = htons(30000);
	name.sin_addr.s_addr = INADDR_ANY;
	int c = bind(socket, (struct sockaddr *)&name, sizeof(name));
}
