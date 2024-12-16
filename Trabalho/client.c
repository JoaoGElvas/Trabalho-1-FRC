#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

typedef struct
{
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

void handle_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <IP do servidor NTP>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int sockfd;
    struct sockaddr_in server_addr;
    ntp_packet packet;
    memset(&packet, 0, sizeof(ntp_packet));
    packet.li_vn_mode = 0x1B; // Leap Indicator = 0, Version = 3, Mode = 3 (Client)

    // Criar o socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        handle_error("Erro ao criar socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(123); // Porta NTP
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        handle_error("Endereço inválido");
    }

    for (int attempt = 0; attempt < 2; ++attempt)
    {
        // Enviar requisição ao servidor
        if (sendto(sockfd, &packet, sizeof(ntp_packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            handle_error("Erro ao enviar pacote");
        }

        // Configurar timeout
        struct timeval timeout = {TIMEOUT_SECONDS, 0};
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        // Receber resposta
        socklen_t addr_len = sizeof(server_addr);
        if (recvfrom(sockfd, &packet, sizeof(ntp_packet), 0, (struct sockaddr *)&server_addr, &addr_len) < 0)
        {
            if (attempt == 1)
            {
                printf("Data/hora: não foi possível contactar servidor\n");
                close(sockfd);
                return EXIT_FAILURE;
            }
            continue;
        }

        // Processar resposta
        packet.txTm_s = ntohl(packet.txTm_s); // Converter para ordem de bytes do host
        time_t tx_time = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);

        // Exibir data e hora
        printf("Data/hora: %s", ctime(&tx_time));
        close(sockfd);
        return EXIT_SUCCESS;
    }

    close(sockfd);
    return EXIT_FAILURE;
}
