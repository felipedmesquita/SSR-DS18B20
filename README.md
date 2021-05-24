# SSR-DS18B20
### Programa base para sistemas simples de controle de temperatura.
#### SSR
Inclui o código para controle de um SSR (rele de estado sólido) com precisão de semiperiodos da onda AC de 60Hz, utilizando apenas os timers internos do ESP32.
#### DS18B20
Faz a leitura de um sensor DS18B20 sem parar a execução do programa enqaunto aguarda resposta do sensor, facilitando a leitura de mais de um sensor.
#### Blynk
Envia os dados de temperatura e razão cíclica do SSR para registro e pode receber comandos
de variação na razão do PWM do rele.
#### Labview [desenvolvimento]
Comunicação por TCP para controle do rele e leitura da temperatura por uma vi.



## Referencias
- [Timers do ESP-32](https://diyprojects.io/esp32-timers-alarms-interrupts-arduino-code/)
- [Labview TCP/IP com ESP-01](https://youtu.be/s-s0ttA4c_4)


## Como usar:
 1. Confira se tem todas as bibliotecas utilizadas
 1. Crie um arquivo auth.h com informações da sua wfi e token do blynk:
```c++
 char auth[] = "sRLvg0_6tajytgTsB0oU-WCkTER6BNhH";
 char ssid[] = "Vivo Fibra 208";
 char pass[] = "senhadawifi";
```

## Próximas tarefas
- Implementar TCP/IP com labview
- Salvar os dados permanentemente na memória flash do ESP32
