#include <Arduino.h>

// Enumeração para os estados do semáforo
enum Estado {
  VERMELHO = 0,
  AMARELO = 1,
  VERDE = 2
};

// Classe que representa uma cor do semáforo
class Luz {
private:
  int pino;
  int duracao; // tempo em segundos
  bool ativa;

public:
  Luz(int p, int d) : pino(p), duracao(d), ativa(false) {
    pinMode(pino, OUTPUT);
    desativar();
  }

  void ativar() {
    digitalWrite(pino, HIGH);
    ativa = true;
  }

  void desativar() {
    digitalWrite(pino, LOW);
    ativa = false;
  }

  bool estaAtiva() const {
    return ativa;
  }

  int getDuracao() const {
    return duracao;
  }

  int getPino() const {
    return pino;
  }
};

// Classe principal do Semáforo
class Semaforo {
private:
  Luz* vermelho;      // Ponteiro para LED Vermelho
  Luz* amarelo;       // Ponteiro para LED Amarelo
  Luz* verde;         // Ponteiro para LED Verde
  Estado estadoAtual;
  unsigned long tempoUltimaTransicao;

public:
  // Construtor com definição dos pinos - aloca memória dinamicamente
  Semaforo(int pinoVerm, int pinoAmarl, int pinoVerd)
    : estadoAtual(VERMELHO),
      tempoUltimaTransicao(millis())
  {
    // Alocação dinâmica de memória para os LEDs
    vermelho = new Luz(pinoVerm, 6);    // 6 segundos
    amarelo = new Luz(pinoAmarl, 2);    // 2 segundos
    verde = new Luz(pinoVerd, 4);       // 4 segundos
    
    iniciar();
  }

  // Destrutor - libera a memória alocada
  ~Semaforo() {
    delete vermelho;
    delete amarelo;
    delete verde;
    Serial.println("Semáforo destruído - Memória liberada");
  }

  // Inicializa o semáforo (começa com vermelho)
  void iniciar() {
    desativarTodas();
    estadoAtual = VERMELHO;
    vermelho->ativar();  // Acesso via ponteiro
    tempoUltimaTransicao = millis();
    
    Serial.println("=== SEMÁFORO INICIADO ===");
    Serial.println("Estado: VERMELHO (6s)");
  }

  // Desativa todas as luzes via ponteiros
  void desativarTodas() {
    vermelho->desativar();
    amarelo->desativar();
    verde->desativar();
  }

  // Obtém a duração do estado atual em milissegundos
  unsigned long getDuracaoAtual() const {
    switch (estadoAtual) {
      case VERMELHO:
        return (unsigned long)vermelho->getDuracao() * 1000;
      case AMARELO:
        return (unsigned long)amarelo->getDuracao() * 1000;
      case VERDE:
        return (unsigned long)verde->getDuracao() * 1000;
      default:
        return 0;
    }
  }

  // Transiciona para o próximo estado
  void proximoEstado() {
    desativarTodas();

    switch (estadoAtual) {
      case VERMELHO:
        estadoAtual = VERDE;
        verde->ativar();
        Serial.println("Estado: VERDE (4s) - Pedestres podem atravessar!");
        break;

      case VERDE:
        estadoAtual = AMARELO;
        amarelo->ativar();
        Serial.println("Estado: AMARELO (2s) - Prepare-se para parar!");
        break;

      case AMARELO:
        estadoAtual = VERMELHO;
        vermelho->ativar();
        Serial.println("Estado: VERMELHO (6s) - PARADO!");
        break;
    }

    tempoUltimaTransicao = millis();
  }

  // Atualiza o semáforo (deve ser chamado continuamente no loop)
  void atualizar() {
    unsigned long tempoDecorrido = millis() - tempoUltimaTransicao;

    if (tempoDecorrido >= getDuracaoAtual()) {
      proximoEstado();
    }
  }

  // Retorna informações do estado atual
  const char* getEstadoNome() const {
    switch (estadoAtual) {
      case VERMELHO:
        return "VERMELHO";
      case AMARELO:
        return "AMARELO";
      case VERDE:
        return "VERDE";
      default:
        return "DESCONHECIDO";
    }
  }

  // Exibe o tempo restante do estado atual
  unsigned long getTempoRestante() const {
    unsigned long tempoDecorrido = millis() - tempoUltimaTransicao;
    unsigned long duracao = getDuracaoAtual();
    
    if (tempoDecorrido < duracao) {
      return (duracao - tempoDecorrido) / 1000;
    }
    return 0;
  }

  // Retorna o estado atual
  Estado getEstado() const {
    return estadoAtual;
  }

  // Retorna ponteiro para LED Vermelho
  Luz* getPontVermelho() const {
    return vermelho;
  }

  // Retorna ponteiro para LED Amarelo
  Luz* getPontAmarelo() const {
    return amarelo;
  }

  // Retorna ponteiro para LED Verde
  Luz* getPontVerde() const {
    return verde;
  }
};

// ============ CONFIGURAÇÃO DO HARDWARE ============
// Pinos do Arduino onde os LEDs estão conectados
const int PINO_VERMELHO = 13;  // LED Vermelho no pino 13
const int PINO_AMARELO = 12;   // LED Amarelo no pino 12
const int PINO_VERDE = 11;     // LED Verde no pino 11

// Ponteiro global para o semáforo (alocação dinâmica)
Semaforo* pSemaforo = nullptr;

// ============ SETUP E LOOP ============
void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Alocação dinâmica do semáforo
  pSemaforo = new Semaforo(PINO_VERMELHO, PINO_AMARELO, PINO_VERDE);
}

void loop() {
  // Verifica se o ponteiro é válido
  if (pSemaforo != nullptr) {
    // Atualiza o semáforo continuamente
    pSemaforo->atualizar();

    // Exibe informações a cada segundo
    static unsigned long ultimoTempo = 0;
    unsigned long tempoAtual = millis();

    if (tempoAtual - ultimoTempo >= 1000) {
      ultimoTempo = tempoAtual;
      
      Serial.print("Estado: ");
      Serial.print(pSemaforo->getEstadoNome());
      Serial.print(" | Tempo restante: ");
      Serial.print(pSemaforo->getTempoRestante());
      Serial.println("s");

      // Exibição de qual LED está ativo
      if (pSemaforo->getPontVermelho()->estaAtiva()) {
        Serial.println("  → LED Vermelho ATIVO");
      }
      if (pSemaforo->getPontAmarelo()->estaAtiva()) {
        Serial.println("  → LED Amarelo ATIVO");
      }
      if (pSemaforo->getPontVerde()->estaAtiva()) {
        Serial.println("  → LED Verde ATIVO");
      }
    }
  } else {
    Serial.println("ERRO: Ponteiro do semáforo é nulo!");
  }

  // Pequeno delay para não sobrecarregar o processador
  delay(10);
}