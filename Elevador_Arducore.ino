/*
 	AUTOR:			Thiago Vilella
 	YOUTUBE:		https://www.youtube.com/channel/UCpg7_v1Wtvp-XPj-vjh8igQ
 	MAIS VÍDEOS:	http://bit.ly/2XE2M1g
 	CURSO ARDUINO:	http://bit.ly/2ZhypC9
 	CURSO ROBOTICA:	http://bit.ly/2zjK9Eo
 	COMPRE:			https://www.arducore.com.br/
 	SKETCH:			Elevador Arducore
 	VERSÃO:			1.0.0
 	DATA:			20/11/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <Servo.h>
#include <myPushButton.h>

// DEFINIÇÕES DE PINOS
#define pinT 9
#define pin1 10
#define pin2 11
#define pin3 12

#define pinBT 3
#define pinB1 4
#define pinB2 5
#define pinB3 6

#define pinSensorFogo 2

#define pinServo 7

// DEFINIÇÕES
#define VelUP 65
#define VelDW 115
#define VelFogo 100
#define Parado 90

#define DEBUG
// INSTANCIANDO OBJETOS
Servo servo;
myPushButton BotaoT(pinBT);
myPushButton Botao1(pinB1);
myPushButton Botao2(pinB2);
myPushButton Botao3(pinB3);

// DECLARAÇÃO DE FUNÇÕES
void moverCabine(byte andar, byte velMaxUP = VelUP, byte velMaxDown = VelDW);
byte verificaAndar();
void verificaIncendio();

void comandoSerial();
void lerSensores();

// DECLARAÇÃO DE VARIÁVEIS
byte sensorAndar[] = {pinT, pin1, pin2, pin3};
int  delayAndar[] = {250, 300, 350, 400};
byte andarAtual;
byte botaoApertado;
bool fogo = false;

// INCLUSÃO DO LCD
#include "displayLCD.h"


// ************* INÍCIO DO SETUP *************
void setup() {
  Serial.begin(9600);

  servo.attach(pinServo);

  for (byte i = 0; i < 4; i++) {
    pinMode(sensorAndar[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(pinSensorFogo), incendioDetectado, FALLING);

  configuraDisplay();
  andarAtual = verificaAndar(); // IDENTIFICA SE ESTÁ PARADO EM ALGUM ANDAR
  exibeDisplay(PARADO, andarAtual);

#ifdef DEBUG
  Serial.println("Fim Setup");
#endif
}
// ************** FIM DO SETUP ***************

// ************** INÍCIO DO LOOP *************
void loop() {
  BotaoT.lerBotao(); Botao1.lerBotao(); Botao2.lerBotao(); Botao3.lerBotao(); // LEITURA DOS BOTÕES

  // FUNÇÃO PARA ATUAR QUANDO FOGO FOR TRUE
  verificaIncendio(fogo);

  // VERIFICA QUAL BOTÃO FOI PRESSIONADO E MOVE A CABINE ATÉ O ANDAR CHAMADO
  if (BotaoT.pressed()) {
    moverCabine(0);
  } else if (Botao1.pressed()) {
    moverCabine(1);
  } else if (Botao2.pressed()) {
    moverCabine(2);
  } else if (Botao3.pressed()) {
    moverCabine(3);
  }

}
// *************** FIM DO LOOP ***************

// IMPLEMENTO DE FUNÇÕES

void moverCabine(byte andar, byte velMaxUP = VelUP, byte velMaxDown = VelDW) {

  if (andarAtual < andar) { // VERIFICA SE O ANDAR ATUAL É MENOR QUE O ANDAR QUE CHAMOU
    servo.write(velMaxUP);
    exibeDisplay(SUBINDO, andar);

    while (digitalRead(sensorAndar[andar]) && !fogo) { // LOOP ENQUANTO NÃO CHEGA NO ANDAR E NÃO TEM SINAL DE FOGO

    }

    // SE HOUVER FOGO EXECUTA O BLOCO ABAIXO
    if (fogo) {
      servo.write(Parado); // PARA O SERVO
      return; // SAI DA FUNÇÃO SEM EXECUTAR MAIS NADA
    }

    // SE NÃO HOUVER FOGO CONTINUA EXECUTANDO O BLOCO ABAIXO
    delay(delayAndar[andar]);
    servo.write(Parado);
    andarAtual = andar;
    exibeDisplay(PARADO, andarAtual);

  } else if (andarAtual > andar) { // VERIFICA SE O ANDAR ATUAL É MAIOR QUE O ANDAR QUE CHAMOU
    servo.write(velMaxDown);
    exibeDisplay(DESCENDO, andar);

    while (digitalRead(sensorAndar[andar]) && !fogo) {// LOOP ENQUANTO NÃO CHEGA NO ANDAR E NÃO TEM SINAL DE FOGO

    }

    // SE HOUVER FOGO EXECUTA O BLOCO ABAIXO
    if (fogo) {
      servo.write(Parado);
      return;// SAI DA FUNÇÃO SEM EXECUTAR MAIS NADA
    }

    // SE NÃO HOUVER FOGO CONTINUA EXECUTANDO O BLOCO ABAIXO
    delay(delayAndar[andar]);

    servo.write(Parado);
    andarAtual = andar;
    exibeDisplay(PARADO, andarAtual);

  }

}

byte verificaAndar() {
  if (!digitalRead(sensorAndar[0])) {
    delay(delayAndar[0]);
    servo.write(Parado);
    return 0;
  }
  else if (!digitalRead(sensorAndar[1])) {
    delay(delayAndar[1]);
    servo.write(Parado);
    return 1;
  }
  else if (!digitalRead(sensorAndar[2])) {
    delay(delayAndar[2]);
    servo.write(Parado);
    return 2;
  }
  else if (!digitalRead(sensorAndar[3])) {
    delay(delayAndar[3]);
    servo.write(Parado);
    return 3;
  }
  else {
    servo.write(VelDW);
    delay(10);
    verificaAndar();
  }
}

void verificaIncendio(bool incendio) {
  // SE FOR IDENTIFICADO FOGO EXECUTA O BLOCO ABAIXO
  if (incendio) {
    exibeDisplay(FOGO, 0); // AVISA QUE FOI IDENTIFICADO FOGO

    if (digitalRead(sensorAndar[0])) {
      servo.write(VelFogo); // DESCE LENTAMENTE

      while (digitalRead(sensorAndar[0])) { // LOOP PARA VERIFICAR SE CHEGOU NO TÉRREO

      }
      delay(400); // PEQUENO DELAY PARA ACERTAR A POSIÇÃO
      servo.write(Parado); //PARA O SERVO
    }

    lcd.setCursor(0, 1);
    lcd.print("TRAVADO "); // AVISA NO DISPLAY QUE ESTÁ TRAVADO

    while (1) {} // APÓS CHEGAR AO TERREO ENTRA NO LOOP INFINITO

  }
}

// FUNÇÃO DA INTERRUPÇÃO
void incendioDetectado() {
  fogo = true;
  //  Serial.println("fogo");
}

// FUNÇÕES PARA TESTE VIA MONITOR SERIAL
void lerSensores() {
  for (byte i = 0; i < 4; i++) {
    Serial.print("Sensor 0"); Serial.print(i); Serial.print(": "); Serial.println(digitalRead(sensorAndar[i]));
  }
  Serial.println("--------------------------------------");
  delay(250);
}

void comandoSerial() {
  if (Serial.available()) {
    char c = Serial.read();
    moverCabine(c - '0');
    lerSensores();
  }
}
