// Programa : RFID - Controle de Acesso com leitor RFID// Autor : David de Moura Marques// GitHub: www.github.com/DavidMoura07#include "RingTone.h"#include "RFID.h"#include <EEPROM.h>#define SS_PIN 10#define RST_PIN 9#define BUZZER 7#define LED_GREEN 5#define LED_RED 3#define JUMPER 4#define BUTTON 2RFID rfid(SS_PIN, RST_PIN); // instancia o leitorRingTone ringTone(BUZZER);int timeButton = 0;struct Card {  byte* id;  int size;};int findCard(Card card){  int memPosition;  int cardPosition = 0;  byte idPart;  int flag = 1;  for ( memPosition = 1; memPosition < EEPROM.length() ; memPosition++ ){        if(cardPosition > card.size){      cardPosition = 0;    }        if(EEPROM.read(memPosition) == card.id[cardPosition]){      cardPosition += 1;    }    if(cardPosition == card.size){      return 1;    }  }  return 0;}void eeClear(){  for (int i = 0 ; i < EEPROM.length() ; i++) {    EEPROM.write(i, 0);  }}int eeWrite(Card card){  int qtdItems = EEPROM.read(0);  int eeAddress = qtdItems * card.size + 1; //Move address to last position.  if(eeAddress <=  (EEPROM.length() - card.size))  {    int lastPosition = eeAddress+card.size;    int cardPosition = 0;    while( eeAddress < lastPosition){      EEPROM.write(eeAddress, card.id[cardPosition]);      eeAddress++;      cardPosition++;    }        qtdItems += 1;    EEPROM.write(0, qtdItems);        return 1;  }  return 0;}void eeRead(){  int address;  byte value;    for(address = 0; address < 10 /* EEPROM.length() */; address++){    value = EEPROM.read(address);      Serial.print(address);    Serial.print("\t");    Serial.print(value);    Serial.println();    delay(25);  }}void printID(Card card){  int i;  for(i=0; i < card.size; i++){    Serial.print(card.id[i]);    Serial.print(" ");  }  Serial.println();}void setup(){  rfid.startRead();  Serial.begin(9600);  Serial.println("Aproxime o seu cartao do leitor...");  Serial.println();  pinMode(BUZZER, OUTPUT);  pinMode(LED_GREEN, OUTPUT);  pinMode(LED_RED, OUTPUT);  pinMode(JUMPER, INPUT);  pinMode(BUTTON, INPUT);  eeRead();}void loop(){  Card card;  if(digitalRead(BUTTON) == HIGH){    if(timeButton >= 5){      digitalWrite(LED_RED, HIGH);      digitalWrite(LED_GREEN, HIGH);      eeClear();      delay(250);      digitalWrite(LED_RED, LOW);      digitalWrite(LED_GREEN, LOW);      delay(250);            digitalWrite(LED_RED, HIGH);      digitalWrite(LED_GREEN, HIGH);      delay(250);      digitalWrite(LED_RED, LOW);      digitalWrite(LED_GREEN, LOW);      delay(250);            digitalWrite(LED_RED, HIGH);      digitalWrite(LED_GREEN, HIGH);      delay(250);      digitalWrite(LED_RED, LOW);      digitalWrite(LED_GREEN, LOW);      delay(250);            digitalWrite(LED_RED, HIGH);      digitalWrite(LED_GREEN, HIGH);      delay(1000);      digitalWrite(LED_RED, LOW);      digitalWrite(LED_GREEN, LOW);      ringTone.success();    }else{      digitalWrite(LED_RED, HIGH);      delay(1000);      digitalWrite(LED_RED, LOW);      timeButton = timeButton + 1;    }  }else{    timeButton = 0;  }    if(digitalRead(JUMPER)){    Serial.println("Aguardando novo cartao");    digitalWrite(LED_GREEN, HIGH);        if (rfid.hasCard()) {      //Mostra UID na serial      card.id = rfid.getCardID();      card.size = rfid.getSizeID();                  if(!findCard(card)){        if(eeWrite(card)){          Serial.println("Novo cartao adicionado: ");          printID(card);          Serial.println();          digitalWrite(LED_GREEN, HIGH);          delay(1000);          digitalWrite(LED_GREEN, LOW);          ringTone.success();        }else{          Serial.println("Memoria cheia, cartao nao inserido");          ringTone.error();        }      }else{        Serial.println("Cartao já adicionado!");        ringTone.success();      }    }      }    // caso tenha lido algum cartão  if (rfid.hasCard()) {    //Mostra UID na serial    card.id = rfid.getCardID();    card.size = rfid.getSizeID();    Serial.print("UID da tag :");    printID(card);    if (findCard(card))    {      Serial.println("Acesso liberado!");      digitalWrite(LED_GREEN, HIGH);      ringTone.success();          } else    {      Serial.println("Acesso negado!");      digitalWrite(LED_RED, HIGH);      ringTone.error();    }  }  delay(1000);  digitalWrite(LED_GREEN, LOW);  digitalWrite(LED_RED, LOW);}