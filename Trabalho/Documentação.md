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
- **Sistema Operacional**: Windows 11 ou posterior.
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
   2.1. Navegue até o diretório onde o arquivo client.c está localizado:
      ```bash
      cd /path/to/your/file
      ```
   2.2. Após estar no diretorio do seu programa
      ```bash
      gcc cliente_sntp.c -o client
      ```
3. Execute o programa:
   ```bash
   ./cliente_sntp <IP_DO_SERVIDOR_NTP>
   ```

Aqui está o conteúdo formatado corretamente em Markdown:

### No Windows (com WSL)

Configure o WSL no Windows e instale uma distribuição Linux, como Ubuntu.

1. No terminal do WSL, instale o GCC:
   ```bash
   sudo apt update
   sudo apt install gcc
   ```

2. Compile o código:
   ```bash
   gcc -o cliente_sntp cliente_sntp.c
   ```

3. Execute o programa:
   ```bash
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


## Considerações Finais

Este cliente SNTP é um exemplo prático da aplicação de conceitos de redes e programação em C, ilustrando como comunicar-se com servidores remotos para obter informações úteis como data e hora. O código pode ser estendido para incluir suporte a IPv6 ou integração com sistemas maiores.

