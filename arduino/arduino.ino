// Controle de um SSR no nivel de semiperiodos para uma rede 60Hz
//https://github.com/felipedmesquita/SSR-DS18B20
#include "auth.h"
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define rele_pin 2
#define temp_pin 15

OneWire oneWire(temp_pin);
DallasTemperature sensors(&oneWire);
//Pode-se ler um sensor especifico por um identificador sensors.getTempC(sensor1)
//DeviceAddress sensor1 = { 0x28, 0x60, 0x1C, 0x79, 0xA2, 0x0, 0x3, 0xE0}; //ROM = 28 60 1C 79 A2 0 3 E0


//Para diferenciar do timer interno que eu chamei de "timer"
BlynkTimer blynktimer;

// Controle do SSR:
volatile int count = 0, semiperiodos = 1, ciclo = 120, total;
unsigned long startTime;
//habilitação dos timers, não precisa ser alterada
//periodo e outras definições são feitas na steup()
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
//Função para abrir ou fecahr o SSR se o numero de
//semiperiodos desejados para o ciclo foi atingido.
void ICACHE_RAM_ATTR isr() {
  total++;
  if (count < semiperiodos) {
    digitalWrite(rele_pin, LOW);
  } else {
    digitalWrite(rele_pin, HIGH);
  }
  if (++count >= ciclo) count = 0;
}

void cadaSegundo() {
  //sensors.getTempC apenas le o ultimo valor de temperatura, sem atualizar
  Blynk.virtualWrite(V1, sensors.getTempCByIndex(0));
  Blynk.virtualWrite(V3, semiperiodos);
  //pede que o sensores calculem novos valores atualizados de temperatura:
  sensors.requestTemperatures();
}

BLYNK_WRITE(V8) {
  semiperiodos = param.asInt();
}


void setup() {
  pinMode(temp_pin, INPUT_PULLUP);
  pinMode(rele_pin, OUTPUT);
  digitalWrite(rele_pin, HIGH);

  Blynk.begin(auth, ssid, pass);
  blynktimer.setInterval(1000L, cadaSegundo);

  //Queremos um timer que chame uma função 120x por segundo
  //para decidirmos se nesse semiperiodo desligamos ou ligamos o SSR
  // 80000000/6666 = 12001,200 tics/s
  //poderia ser divido por 666600 para
  //disparar 120 vezes por segundo, mas
  //o prescaler é de 16bits
  //timerbegin(id, prescaler, rising edge ou falling edge)
  timer = timerBegin(0, 6666, true);
  //timerAttachInterrupt(timer, function, rising ou change)
  timerAttachInterrupt(timer, &isr, true);
  //timerAlarmWrite(timer, frequency, autoreload)
  timerAlarmWrite(timer, 100, true);
  //inicia o timer
  timerAlarmEnable(timer);

  //Faz o pedido de temperatura para os sensores nao parar
  //até chegar a resposta (os sensores demoram bem)
  sensors.setWaitForConversion(false);
  startTime = millis();
}

void loop() {
  Blynk.run();
  blynktimer.run();
}
