# Cliente de Tempo (SNTP)

## Introdução

Este projeto implementa um cliente para o protocolo SNTP (Simple Network Time Protocol), com base nos requisitos do **Trabalho 01** da disciplina **Fundamentos de Redes de Computadores**. O cliente utiliza **sockets UDP** para comunicação com um servidor NTP, recupera a data e hora fornecidas pelo servidor e as apresenta no formato humano legível. Além disso, o projeto possui tratamento de erros e implementa um mecanismo de timeout para evitar bloqueios indefinidos.

---

## Objetivos

- Implementar um cliente SNTP funcional e compatível com os servidores NTP.
- Utilizar conceitos de redes, como sockets e comunicação cliente-servidor.
- Demonstrar habilidades em programação com C, lidando com estruturas de dados e manipulação de pacotes.
- Garantir que o cliente funcione em diferentes ambientes de desenvolvimento (Linux, macOS e Windows).

---

## Ambiente de Desenvolvimento

### Linux
- **Sistema Operacional**: Ubuntu 22.04 LTS ou similar.
- **Compilador**: GCC 12.3.0.
- **Editor de Texto**: Visual Studio Code (ou outro de sua preferência).
- **Outras Ferramentas**: `make`, `gdb` para depuração.

### macOS
- **Sistema Operacional**: macOS Ventura ou posterior.
- **Compilador**: GCC (Instalado com o Xcode Command Line Tools)
- **Editor de Texto**: Visual Studio Code (ou outro de sua preferência).
- **Outras Ferramentas**: `make`, `lldb` para depuração.

### Windows
- **Sistema Operacional**: Windows 10 ou posterior.
- **Compilador**: MinGW (GCC) ou WSL (Windows Subsystem for Linux).
- **Editor de Texto**: Visual Studio Code.
- **Outras Ferramentas**: PowerShell ou CMD para execução de comandos.

---

## Como Construir a Aplicação

### No Linux
1. Certifique-se de que o GCC está instalado:
   ```bash
   sudo apt update
   sudo apt install gcc make
   ```
2. Compile o código usando o comando:
   ```bash
   gcc -o cliente_sntp cliente_sntp.c
   ```
3. Execute o programa:
   ```bash
   ./cliente_sntp <IP_DO_SERVIDOR_NTP>
   ```

### No macOS
1. Certifique-se de que o GCC está instalado: </br>
No terminal, digite:
   ```bash
   gcc --version
2. Compile o código: </br>
   2.1 Navegue até o diretório onde o arquivo client.c está localizado:
      ```bash
      cd /path/to/your/file
      ```
   2.2 Após estar no diretorio do seu programa
      ```bash
      gcc cliente_sntp.c -o client
      ```
3. Execute o programa:
   ```bash
   ./cliente_sntp.c <IP_DO_SERVIDOR_NTP>
   ```

### No Windows (com MinGW)
1. Instale o MinGW e adicione-o ao `PATH`.
2. Compile o código:
   ```bash
   gcc -o cliente_sntp cliente_sntp.c -lws2_32
   ```
3. Execute o programa:
   ```cmd
   cliente_sntp.exe <IP_DO_SERVIDOR_NTP>
   ```

---

## Estrutura do Código

O programa implementa um cliente NTP que:
1. Cria um socket UDP.
2. Preenche os campos necessários de um pacote SNTP e o envia ao servidor.
3. Aguarda a resposta do servidor, aplicando um timeout configurado.
4. Processa o pacote recebido para calcular e exibir a data/hora do servidor NTP.

### Principais Componentes
- **Estrutura `ntp_packet`**: Representa o pacote SNTP, com campos para envio e recepção.
- **Função `handle_error`**: Trata erros e encerra o programa em caso de falhas críticas.
- **Mecanismo de Timeout**: Configurado para evitar que o cliente fique bloqueado indefinidamente ao esperar uma resposta do servidor.

### Código Fonte
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

// Definição da estrutura do pacote SNTP
typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t rootDelay;
    uint32_t rootDispersion;
    uint32_t refId;
    uint32_t refTm_s;
    uint32_t refTm_f;
    uint32_t origTm_s;
    uint32_t origTm_f;
    uint32_t rxTm_s;
    uint32_t rxTm_f;
    uint32_t txTm_s;
    uint32_t txTm_f;
} ntp_packet;

#define NTP_TIMESTAMP_DELTA 2208988800ull
#define TIMEOUT_SECONDS 20

// Função para tratar erros
void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <IP do servidor NTP>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int sockfd;
    struct sockaddr_in server_addr;
    ntp_packet packet;
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = 0x1B; // Leap Indicator = 0, Version = 3, Mode = 3 (Client)

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        handle_error("Erro ao criar socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(123); // Porta NTP
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        handle_error("Endereço inválido");
    }

    // Configuração e envio de pacote
    for (int attempt = 0; attempt < 2; ++attempt) {
        if (sendto(sockfd, &packet, sizeof(ntp_packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            handle_error("Erro ao enviar pacote");
        }

        struct timeval timeout = {TIMEOUT_SECONDS, 0};
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        socklen_t addr_len = sizeof(server_addr);
        if (recvfrom(sockfd, &packet, sizeof(ntp_packet), 0, (struct sockaddr *)&server_addr, &addr_len) < 0) {
            if (attempt == 1) {
                printf("Data/hora: não foi possível contactar servidor\n");
                close(sockfd);
                return EXIT_FAILURE;
            }
            continue;
        }

        packet.txTm_s = ntohl(packet.txTm_s);
        time_t tx_time = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);
        printf("Data/hora: %s", ctime(&tx_time));
        close(sockfd);
        return EXIT_SUCCESS;
    }

    close(sockfd);
    return EXIT_FAILURE;
}
```

---

## Considerações Finais

Este cliente SNTP é um exemplo prático da aplicação de conceitos de redes e programação em C, ilustrando como comunicar-se com servidores remotos para obter informações úteis como data e hora. O código pode ser estendido para incluir suporte a IPv6 ou integração com sistemas maiores.
