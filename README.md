# Semáforo com Arduino - Atividade Ponderada

---

# AVISO IMPORTANTE

## ATENÇÃO: SUBSTITUIÇÃO DO LED AMARELO POR LED AZUL

**O LED amarelo foi substituído por um LED azul no circuito devido à indisponibilidade de LEDs amarelos no momento da montagem.**

Esta substituição não afeta o funcionamento lógico do código, apenas a cor visual exibida durante o estado AMARELO do semáforo. O código e a lógica permanecem inalterados.

---

## Descrição

Este projeto implementa um semáforo controlado por Arduino utilizando programação orientada a objetos em C++. O sistema simula o funcionamento de um semáforo com três estados (vermelho, amarelo e verde), cada um com sua duração específica, e utiliza alocação dinâmica de memória para gerenciar os componentes.

## Objetivos

- Demonstrar o uso de programação orientada a objetos em Arduino
- Implementar gerenciamento de estados de um semáforo
- Utilizar ponteiros e alocação dinâmica de memória
- Controlar LEDs através de pinos digitais do Arduino

## Componentes Necessários

- 1 Arduino Uno (ou compatível)
- 3 LEDs (vermelho, amarelo e verde)
- 3 resistores de 220Ω (ou similar)
- Fios jumper
- Protoboard

## Circuito

![Circuito do Semáforo](https://plum-atomic-lemur-391.mypinata.cloud/ipfs/bafybeigpwhnbayjdafiwhmrrovm4m2vpf6gf5z3zgmoibmbdjrh74sedii)

## Estrutura do Código

### Enumeração de Estados

O código utiliza uma enumeração para representar os três estados possíveis do semáforo:
- `VERMELHO = 0`
- `AMARELO = 1`
- `VERDE = 2`

### Classe Luz

A classe `Luz` representa uma cor individual do semáforo e encapsula:
- **Pino do Arduino**: onde o LED está conectado
- **Duração**: tempo que a luz permanece ativa (em segundos)
- **Estado ativo**: se a luz está ligada ou desligada

**Métodos principais**:
- `ativar()`: liga o LED no pino correspondente
- `desativar()`: desliga o LED
- `estaAtiva()`: retorna se a luz está ativa
- `getDuracao()`: retorna a duração configurada
- `getPino()`: retorna o pino do Arduino

### Classe Semaforo

A classe `Semaforo` gerencia o funcionamento completo do semáforo:

**Atributos principais**:
- Três ponteiros para objetos `Luz` (vermelho, amarelo, verde)
- Estado atual do semáforo
- Tempo da última transição de estado

**Métodos principais**:
- `iniciar()`: inicializa o semáforo no estado vermelho
- `atualizar()`: verifica se deve fazer a transição de estado baseado no tempo decorrido
- `proximoEstado()`: realiza a transição para o próximo estado na sequência
- `getEstadoNome()`: retorna o nome do estado atual como string
- `getTempoRestante()`: calcula o tempo restante do estado atual

**Sequência de funcionamento**:
1. Vermelho: 6 segundos
2. Verde: 4 segundos
3. Amarelo: 2 segundos
4. Retorna ao vermelho (ciclo contínuo)

## Configuração do Hardware

O código está configurado para utilizar os seguintes pinos:
- **Pino 13**: LED Vermelho
- **Pino 12**: LED Amarelo
- **Pino 11**: LED Verde

Os LEDs devem ser conectados com resistores de 220Ω em série para limitar a corrente.

## Funcionamento

### Setup

No `setup()`, o código:
1. Inicializa a comunicação serial a 9600 baud
2. Aloca dinamicamente um objeto `Semaforo` na memória
3. Inicializa o semáforo no estado vermelho

### Loop

No `loop()`, o código:
1. Atualiza o semáforo continuamente (verifica transições de estado)
2. A cada segundo, imprime no Serial Monitor:
   - Estado atual do semáforo
   - Tempo restante do estado atual
   - Qual LED está ativo no momento

## Alocação Dinâmica de Memória

O projeto utiliza alocação dinâmica de memória para demonstrar o uso de ponteiros:
- O objeto `Semaforo` é criado com `new` no `setup()`
- Cada `Luz` também é alocada dinamicamente dentro do construtor de `Semaforo`
- O destrutor de `Semaforo` libera toda a memória alocada com `delete`

**Nota**: Em projetos Arduino reais, geralmente não é necessário usar alocação dinâmica devido ao ambiente embarcado, mas este projeto demonstra os conceitos de gerenciamento de memória.

## Monitor Serial

O código envia mensagens para o Serial Monitor incluindo:
- Mensagem de inicialização
- Transições de estado com duração de cada estado
- Informações periódicas sobre o estado atual e tempo restante
- Indicadores de quais LEDs estão ativos

Para visualizar as mensagens, abra o Serial Monitor no Arduino IDE e configure para 9600 baud.

## Vídeo Demonstrativo

<a href="https://plum-atomic-lemur-391.mypinata.cloud/ipfs/bafybeidtpseiycgsmecmzpclsxuzocsvlcrkztsmpr3xvta76hd5amquf4">
Link do video </a>

## Como Usar

1. Monte o circuito conforme a imagem fornecida
2. Carregue o código `atividade.ino` no Arduino
3. Abra o Serial Monitor (Ctrl+Shift+M) e configure para 9600 baud
4. Observe o funcionamento do semáforo e as mensagens no monitor serial

## Autor

Victor Garcia Dos Santos, desenvolvido como parte da atividade ponderada do Inteli.

## Observações

- As durações dos estados podem ser ajustadas modificando os valores no construtor de `Semaforo`
- O delay de 10ms no final do `loop()` evita sobrecarga do processador
- Os pinos podem ser alterados modificando as constantes `PINO_VERMELHO`, `PINO_AMARELO` e `PINO_VERDE`

