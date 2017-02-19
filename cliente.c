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

int start_client();
int jogada_cli(char tabuleiro[], int jogador);
int checa_jogada_cli(char tabuleiro[], char escolha[], int* n);
void print_tabuleiro_cli(char tabuleiro[]);
int checa_vitoria_cli(char tabuleiro[], int jogador);
int checa_empate_cli(char tabuleiro[]);

int start_client(){

    #ifdef __WIN32__
        WSADATA wsaData;
	    WSAStartup(MAKEWORD(1, 1), &wsaData);
    #endif
	
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;

    char buff[11];
    char tabuleiro[11];
    char resposta[4];
    char ip[15];

    printf("Endereço IP do seu servidor: ");
    scanf(" %[^\n]s", ip);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(30000);

    printf("Tentando conectar ao servidor...\n");
    int conn = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if(conn == -1){
    	printf("IP inválido.\n");
    	return 0;
    }
    
    int i = 0;

    while(++i < 5){
        
        bzero(buff,sizeof(buff));
        printf("Esperando o jogador 1\n");
        
        int x;
        #ifdef __WIN32__
            x = recv(sockfd, buff, sizeof(buff), 0);
        #else
            x = read(sockfd, buff, sizeof(buff));
        #endif

        if(x == -1){
            printf("Conexão perdida\n");
            close(sockfd);
            return 0;
        }

        if(!strcmp(buff, "VIT")){
            printf("Vitória do jogador 1\n");
            close(sockfd);
            return 0;
        }

        strcpy(tabuleiro, buff);
        print_tabuleiro_cli(tabuleiro);
        jogada_cli(tabuleiro, 2);

        if(checa_vitoria_cli(tabuleiro, 2)){
            #ifdef __WIN32__
                send(sockfd, "VIT", sizeof("VIT"), 0);
            #else   
                write(sockfd, "VIT", sizeof("VIT"));
            #endif
            printf("Vitória do jogador 2\n");
            close(sockfd);
            return 0;
        }

        #ifdef __WIN32__
            send(sockfd, tabuleiro, sizeof(tabuleiro), 0);
        #else	
            write(sockfd, tabuleiro, sizeof(tabuleiro));
        #endif
    }

    bzero(buff, sizeof(buff));

    printf("Esperando o jogador 1\n");
    int x;
    #ifdef __WIN32__
        x = recv(sockfd, buff, sizeof(buff), 0);
    #else
        x = read(sockfd, buff, sizeof(buff));
    #endif

    if(x == -1){
        printf("Conexão perdida\n");
        close(sockfd);
        return 0;
    }

    if(!strcmp(buff, "VIT")){
        printf("Vitória do jogador 1\n");
        close(sockfd);
        return 0;
    }

    strcpy(tabuleiro, buff);
    print_tabuleiro_cli(tabuleiro);
    
    printf("Fim de jogo. Empate.\n");
    close(sockfd);
    return 0;
}

int jogada_cli(char tabuleiro[], int jogador){
    char escolha[2];
    char letra = (jogador == 1) ? 'X' : 'O';
    int permitido = 0;
    int n;
    
    while(!permitido){
        printf("Escolha do jogador %d: ", jogador);
        scanf(" %s", escolha);
        if((escolha[0] >= 65 && escolha[0] <= 67) && (escolha[1] >= 49 && escolha[1] <= 51)){
            if(strlen(escolha) == 2){
                if(checa_jogada_cli(tabuleiro, escolha, &n)){
                    permitido = 1;
                    tabuleiro[n] = letra;
                    print_tabuleiro_cli(tabuleiro);
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

int checa_jogada_cli(char tabuleiro[], char escolha[], int* n){
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

void print_tabuleiro_cli(char tabuleiro[]){
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

int checa_vitoria_cli(char tabuleiro[], int jogador){
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

int checa_empate_cli(char tabuleiro[]){
    int i = 1;
    while(i < 10){
        if(tabuleiro[i] == ' ')
            return 0;
        i++;
    }
    return 1;
}
