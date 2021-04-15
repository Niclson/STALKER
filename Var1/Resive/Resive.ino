//Приёмник
#include <VirtualWire.h> // библиотека приёмника
#include <Wire.h> // библиотека для дисплея I2C
#include <LiquidCrystal_I2C.h> // Подключение библиотеки для дисплея

LiquidCrystal_I2C lcd(0x27,16,2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой

byte message[VW_MAX_MESSAGE_LEN]; // Буфер для хранения принимаемых данных
byte messageLength = VW_MAX_MESSAGE_LEN; // Размер сообщения

const int led_pin = 13; // Пин светодиода
const int receiver_pin = 2; // Пин подключения приемника

void setup()
{

      lcd.init();                      // Инициализация дисплея  
      lcd.backlight();                 // Подключение подсветки
      lcd.setCursor(0,0);              // Установка курсора в начало первой строки
      lcd.print("Hello");       // Набор текста на первой строке
      lcd.setCursor(0,1);              // Установка курсора в начало второй строки
      lcd.print("ArduinoMaster");       // Набор текста на второй строке
      
      Serial.begin(9600); // Скорость передачиданных
      Serial.println("MX-RM-5V is ready");
      vw_set_rx_pin(receiver_pin); // Пин подключения приемника
    
      vw_setup(100); // Скорость передачи данных (бит в секунду)
      vw_rx_start(); // Активация применика
}


void loop()
{
  
    if (vw_get_message(message, &messageLength)) // Если есть данные..
    {
        digitalWrite(led_pin, HIGH); // Зажигаем светодиод в начале приема пакета
        for (int i = 0; i < messageLength; i++)
        {
            Serial.write(message[i]); // выводим их в одной строке
            


            
        }
        Serial.println();
           lcd.setCursor(0,0);// Установка курсора в начало первой строки 
            lcd.print(millis()/1000);       // Набор текста на первой строке
        digitalWrite(led_pin, LOW); // Гасим светодиод в конце
    }



    
}
