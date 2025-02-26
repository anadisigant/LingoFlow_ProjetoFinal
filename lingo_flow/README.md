 # LEDs Animados

__SUBGRUPO 0__<br>
Repositório criado a fim de registrar a realização da atividade referente à aula síncrona do dia 20/01/2025.

__Componentes do grupo:__
Ana Karolina Disigant Reis, Andressa Sousa Fonseca, Lucas Carneiro de Araújo Lima, Luiz Rodrigo Oliveira da Silva, Matheus Santos Souza e Wesley R.

## ATIVIDADE 
__Microcontroladores - GPIO (PARTE 2)__<br>

A atividade pede que, com o auxílio do simulador de eletrônica Wokwi, utilize-se o Raspberry Pi Pico W e um teclado matricial 4x4 para gerar informações visuais em uma matriz de LEDs Coloridos (5x5 WS2812). 

__Para esta prática, os seguintes componentes e ferramentas se fazem necessários:__
1) Teclado matricial 4x4.
2) Microcontrolador Raspberry Pi Pico W.
3) Matriz de LEDs Coloridos (LED-RGB 5x5 WS2812).
4) Buzzer Passivo Eletromagnético MLT-8530 SMD 3.6V
5) Simulador de Sistemas Embarcados Wokwi
6) Ambiente de trabalho VSCode
7) Ferramenta educacional BitDogLab (Para aplicação prática)
8) PuTTY (Para aplicação prática)


__O código com a lógica adotada pelo grupo se encontra no presente reposítório e o resultado pode ser assistido no vídeo alocado neste link: [Aplicação GPIOs - animações em LED](https://www.youtube.com/watch?v=UjJnq5CxnIs).__

<div align="center">
  <img src="https://github.com/user-attachments/assets/0ae5fa23-4bc6-43d1-8f01-7f12f507d591" alt="GIF demonstrativo" width="300"/>
</div>


## Instruções de Uso

### 1. Clone o repositório
Abra o terminal e execute o comando abaixo para clonar o repositório em sua máquina:
```bash
git clone https://github.com/Dreh3/ledsAnimados.git
```

### 2. Configure o ambiente de desenvolvimento
Certifique-se de que o [SDK do Raspberry Pi Pico](https://github.com/raspberrypi/pico-sdk) esteja instalado e configurado corretamente no seu sistema.

### 3. Compile o projeto
Navegue até o diretório raiz do projeto e execute os comandos abaixo:
```bash
mkdir build
cd build
cmake ..
make
```
O arquivo `matrizAnimada.uf2` será gerado na pasta `build`.

### 4. Carregue o programa no Raspberry Pi Pico
1. Conecte o Raspberry Pi Pico ao seu computador enquanto mantém pressionado o botão **BOOTSEL**.
2. O Pico será montado como um dispositivo de armazenamento USB.
3. Copie o arquivo `matrizAnimada.uf2` para o dispositivo montado.
4. O Pico reiniciará automaticamente e executará o programa.

### 5. Conecte a matriz de LEDs
Siga o diagrama de conexão fornecido no arquivo `diagram.json` para conectar corretamente a matriz de LEDs **WS2812** ao Raspberry Pi Pico.

### 6. Execute o programa
Após completar as etapas anteriores, o programa será executado no Raspberry Pi Pico, exibindo as animações implementadas na matriz de LEDs.

## Observações

- Certifique-se de usar cabos adequados para conexão e alimentação da matriz de LEDs.
- Para personalizar as animações, modifique os parâmetros no código fonte antes de compilar.

## Recursos

- O programa funciona **tanto** na placa BitDogLab (extra) **quanto** no simulador Wokwi (obrigatório)
- O programa emite **sinal sonoro** a cada opção selecionada
- O programa executa uma animação do **número escolhido** na matriz de LEDs
- O programa funciona **tanto** com o teclado matricial (para o Wokwi) **quanto** com o teclado do computador (para aplicação na placa BitDogLab)
- Além das 6 animações correspondentes ao número de membros da equipe (6), o programa conta com **2 animações extras**

# Funções dos botões

| COMANDO                            | DESCRIÇÃO                                     | FPS   |
|:----------------------------------:|:---------------------------------------------:|:-----:|
| A                                  | Desliga todos os LEDs acesos                  | 10    |
| B                                  | Liga todos os LEDs da cor azul                | 10    |
| C                                  | Liga todos os LEDs da cor vermelha            | 10    |
| D                                  | Liga todos os LEDs da cor verde               | 10    |
| #                                  | Liga todos os LEDs da cor branca              | 10    |
| *                                  | Sai do modo de execução e habilita o de gravação | 10    |
| 1                                  | Animação de Andressa                          | 10    |
| 2                                  | Animação de Matheus                           | 10    |
| 3                                  | Animação de Ana Karolina                      | 10    |
| 4                                  | Animação de Lucas                             | 10    |
| 5                                  | Animação de Wesley                            | 10    |
| 6                                  | Animação de Luiz                              | 10    |
| 7                                  | Animação extra                                | 10    |
| 8                                  | Animação extra                                | 10    |


Vale ressaltar que há ativação do buzzer em todos os comandos.

