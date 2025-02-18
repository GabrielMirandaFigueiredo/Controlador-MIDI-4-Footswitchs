#include <Wire.h>
#include <MIDI.h>

int powerPin = 11; // Alimentação MIDI
int gndPin = 10;   // GND MIDI

// Pinos dos botões
const int buttonPins[4] = {2, 3, 4, 5}; 

// CC numbers para cada botão
const int ccNumbers[4] = {69, 69, 69, 52};

// Valores
const int ccValues[4] = {0, 1, 2, 127};  

// Estado anterior dos botões
bool lastButtonState[4] = {LOW, LOW, LOW, LOW};

// Estado de controle para os botões
bool buttonState[4] = {false, false, false, false};

// Cria a instância MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  // Inicializa o MIDI (canal 1)
  MIDI.begin(1);

  delay(50);

  // Configura os pinos de alimentação MIDI como saída
  pinMode(powerPin, OUTPUT);
  pinMode(gndPin, OUTPUT);

  // Define os estados dos pinos de alimentação MIDI
  digitalWrite(powerPin, HIGH);  // 5V
  digitalWrite(gndPin, LOW);     // 0V
  
  // Configura os botões como entrada com pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Processa os 4 primeiros botões (valores fixos de CC)
  for (int i = 0; i < 4; i++) {
    bool currentState = digitalRead(buttonPins[i]) == LOW; // Botão pressionado

    // Verifica se o botão foi pressionado (transição de LOW para HIGH)
    if (currentState && !lastButtonState[i]) {
      // Envia o valor MIDI do botão, e define o estado de controle
      MIDI.sendControlChange(ccNumbers[i], ccValues[i], 1);
      buttonState[i] = true;
    }

    // Verifica se o botão foi liberado (transição de HIGH para LOW)
    if (!currentState && lastButtonState[i] && buttonState[i]) {
      buttonState[i] = false;
    }

    lastButtonState[i] = currentState; // Atualiza o estado do botão
  }

  delay(5); // Pequeno delay para debouncing
}
