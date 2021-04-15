// Передатчик
#include <VirtualWire.h>

const int led_pin = 13; // Пин светодиода
const int transmit_pin = 4; // Пин подключения передатчика

void setup()
{
vw_set_tx_pin(transmit_pin);
vw_setup(100);       // Скорость передачи (Бит в секунду)
pinMode(led_pin, OUTPUT);
Serial.begin (9600);

Serial.println("TRANSMIT");
}

void loop()
{
      const char *msg = "Anamaliy_1"; // Передаваемое сообщение
      digitalWrite(led_pin, HIGH); // Зажигаем светодиод в начале передачи
      vw_send((uint8_t *)msg, strlen(msg)); // Отправка сообщения
      vw_wait_tx(); // Ожидаем окончания отправки сообщения
      digitalWrite(led_pin, LOW); // Гасим светодиод в конце передачи
      delay(100);
}
