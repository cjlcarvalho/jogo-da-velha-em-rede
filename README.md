# jogo-da-velha-em-rede
Implementação do jogo da velha conectado em rede utilizando sockets em C

#### Rodando no Windows

Configure o compilador da seguinte maneira:
* Chame o compilador com `-std=c99`
* Adicione as seguintes opções ao chamar o linker `-static-libgcc -l ws2_32`
