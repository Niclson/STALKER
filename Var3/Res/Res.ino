//RECEIVER


#include <EEPROM.h>

#include "GyverTimers.h"
int cycle_step_motor = 1;

  
#define TIME_SECOND (400)      // примерное необходимое время нахождения в аномалии в секундах. время реальное = необходимое время / 1,5.  600/1,5 = 400 - примерно 10 минуты игрока
#define PWM         (5)        // скважность мотора. подберается в процессе 
#define RES_AN_1    (192)      //  числовой результат аномалии 1. Число от 0 до 255 
#define RES_AN_2    (134)      //  числовой результат аномалии 2. Число от 0 до 255
#define RES_AN_3    (251)      //  числовой результат аномалии 3. Число от 0 до 255



#include <VirtualWire.h>              //Download it here: http://electronoobs.com/eng_arduino_virtualWire.php
byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
int received_number = 0;





#include <Wire.h> // библиотека для дисплея I2C
#include <LiquidCrystal_I2C.h> // Подключение библиотеки для дисплея

LiquidCrystal_I2C lcd(0x27,16,2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой




int flag_transmit, flag_enable_step_motor;
uint32_t timeout;

int counter_time_anamaly_1, flag_counter_time_anamaly_1 = 0;
int counter_time_anamaly_2, flag_counter_time_anamaly_2 = 0;
int counter_time_anamaly_3, flag_counter_time_anamaly_3 = 0;

int flag_counter_time_anamaly_1_ee = 1;
int flag_counter_time_anamaly_2_ee = 4;
int flag_counter_time_anamaly_3_ee = 7;

int galetnik_status;
long int adc;











 




void setup()
{
  //Настройка прерывания////////////////////////////////
  Timer2.setFrequency(500);         // Задать частоту прерываний таймера в Гц, вернет реальную частоту в герцах
  Timer2.enableISR(CHANNEL_A);
  /////////////////////////////////////////////////////
  
  //Выводы драйвера шагового мотора////////////////////
  DDRC |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
  /////////////////////////////////////////////////////


  // выводы галетника
  DDRB |= (1<<5); 
  DDRD |= (1<<6);
  
  PORTB |= (1<<1) | (1<<3) | (1<<4); 
  PORTD |= (1<<6);
  
  DDRD |= (1<<5);
  PORTD |= (1<<5);

  pinMode(3, OUTPUT); 






  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("ANOMALY SCANNER");   // Набор текста на первой строке
  lcd.setCursor(0,1);              // Установка курсора в начало второй строки
  lcd.print("Chernobyl - 4");      // Набор текста на второй строке
  delay(1000);


  


      
  Serial.begin(1000000);
  Serial.println("Ready...");
  
  vw_set_rx_pin(4); // pin
  vw_setup(200); // bps
  vw_rx_start();
  delay(1000);


  
  lcd.clear();


  // чтение флагов аномалий из eeprom
  flag_counter_time_anamaly_1 = EEPROM.read(flag_counter_time_anamaly_1_ee);
  flag_counter_time_anamaly_2 = EEPROM.read(flag_counter_time_anamaly_2_ee);
  flag_counter_time_anamaly_3 = EEPROM.read(flag_counter_time_anamaly_3_ee);






  



  



  // обработчик простомтра выставленных влагов
  if(!(PIND&(1<<6)))  { 

      lcd.clear();
      
      lcd.setCursor(0,0);   lcd.print("A1:");
      lcd.setCursor(3,0);   lcd.print(flag_counter_time_anamaly_1);
      
      lcd.setCursor(7,0);   lcd.print("A2:");
      lcd.setCursor(10,0);   lcd.print(flag_counter_time_anamaly_2);
      
      lcd.setCursor(0,1);   lcd.print("A3:");
      lcd.setCursor(3,1);   lcd.print(flag_counter_time_anamaly_3);

      while((PINB&(1<<4)));
          
      EEPROM.write(flag_counter_time_anamaly_1_ee ,0);
      EEPROM.write(flag_counter_time_anamaly_2_ee ,0);
      EEPROM.write(flag_counter_time_anamaly_3_ee ,0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EEPROM CLEAR!");         
     
      while(1);
      
  }

  
  
  
}
































void loop()
{
    
    

    // Обработчик галетника
    if(!(PINB&(1<<1)))  { galetnik_status = 2; analogWrite(3, PWM);}
    if(!(PINB&(1<<3)))  { galetnik_status = 1; analogWrite(3, PWM);}
    if(!(PINB&(1<<4)))  { galetnik_status = 3; analogWrite(3, PWM);}
    


    

    // вывод надписи отсутствия сигнала 
    if(flag_transmit == 0){

      lcd.setCursor(0,0);
      lcd.print("ANOMALIY #");
      lcd.setCursor(10,0);
      lcd.print(galetnik_status);
       
      lcd.setCursor(0,1);// Установка курсора в начало первой строки 
      lcd.print("PLACE SCANNER");
    
    }







    // обработчик принятых дакетов от передатчиков
    if (vw_get_message(message, &messageLength)) // non-blocking
    {
      
        flag_transmit = 1;
        timeout = 0;
        
        Serial.print("Transmiter: ");
        for (int i = 0; i < messageLength; i++)
        {
            //Serial.print(message[i]);
            received_number = message[i];
        }
        Serial.println(received_number);
       // analogWrite(LED, received_number);


        lcd.clear();


        // обработчик аномалии 1
        if( (received_number == 192) && (galetnik_status == 1) ){

           if( flag_counter_time_anamaly_1 == 0)   {flag_enable_step_motor = 1;}
            
            analogWrite(3, PWM);
            //delay(500);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY #");
            lcd.setCursor(10,0);
            lcd.print(galetnik_status);
      
            lcd.setCursor(0,1);
            lcd.print("SCANNING:");
            lcd.setCursor(10,1);
            //lcd.print(received_number);
            adc = adc+analogRead(A7);
            lcd.print(adc);

            
            counter_time_anamaly_1++;
            counter_time_anamaly_2 = 0;
            counter_time_anamaly_3 = 0;


            

            if( flag_counter_time_anamaly_1 ){
                analogWrite(3, 0);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("ANOMALIY ");
                lcd.setCursor(9,0);
                lcd.print("OK");
                
                flag_enable_step_motor = 0;

                lcd.setCursor(0,1);
                lcd.print("ACTIVE LEVEL");
                lcd.setCursor(13,1);
                lcd.print(flag_counter_time_anamaly_1);
            }

        }
        if( (received_number == 192) && (galetnik_status != 1) ){

            
            flag_enable_step_motor = 0;
            analogWrite(3, 0);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY ERROR");
      
            lcd.setCursor(2,1);
            lcd.print("!WRONG TYPE!");

            counter_time_anamaly_2 = 0;
            counter_time_anamaly_3 = 0;
        
        }








        // обработчик аномалии 2
        if( (received_number == 134) && (galetnik_status == 2) ){

            if( flag_counter_time_anamaly_2 == 0)   {flag_enable_step_motor = 1;}
            
            analogWrite(3, PWM);
            //delay(500);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY #");
            lcd.setCursor(10,0);
            lcd.print(galetnik_status);
      
            lcd.setCursor(0,1);
            lcd.print("SCANNING:");
            lcd.setCursor(10,1);
            adc = adc+analogRead(A7);
            lcd.print(adc);
            
            counter_time_anamaly_1 = 0;
            counter_time_anamaly_2++;
            counter_time_anamaly_3 = 0;

            if( flag_counter_time_anamaly_2 ){
                analogWrite(3, 0);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("ANOMALIY ");
                lcd.setCursor(9,0);
                lcd.print("OK");

                flag_enable_step_motor = 0;

                lcd.setCursor(0,1);
                lcd.print("ACTIVE LEVEL");
                lcd.setCursor(13,1);
                lcd.print(flag_counter_time_anamaly_2);

            }

        }
        
        if( (received_number == 134) && (galetnik_status != 2) ){

            flag_enable_step_motor = 0;
            analogWrite(3, 0);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY ERROR");
      
            lcd.setCursor(2,1);
            lcd.print("!WRONG TYPE!");

            counter_time_anamaly_1 = 0;
            counter_time_anamaly_3 = 0;
        
        }
      







        // обработчик аномалии 3
        if( (received_number == 251) && (galetnik_status == 3) ){

            if( flag_counter_time_anamaly_3 == 0)   {flag_enable_step_motor = 1;}
            
            analogWrite(3, PWM);
            //delay(500);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY #");
            lcd.setCursor(10,0);
            lcd.print(galetnik_status);
      
            lcd.setCursor(0,1);
            lcd.print("SCANNING:");
            lcd.setCursor(10,1);
            adc = adc+analogRead(A7);
            lcd.print(adc);
            
            counter_time_anamaly_1 = 0;
            counter_time_anamaly_2 = 0;
            counter_time_anamaly_3++;
            

            if( flag_counter_time_anamaly_3 ){
                analogWrite(3, 0);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("ANOMALIY ");
                lcd.setCursor(9,0);
                lcd.print("OK");

                flag_enable_step_motor = 0;

                lcd.setCursor(0,1);
                lcd.print("ACTIVE LEVEL");
                lcd.setCursor(13,1);
                lcd.print(flag_counter_time_anamaly_3);
            }

        }
        if( (received_number == 251) && (galetnik_status != 3) ){
 
            flag_enable_step_motor = 0;
            
            analogWrite(3, 0);
            lcd.clear();

            lcd.setCursor(0,0);
            lcd.print("ANOMALIY ERROR");
      
            lcd.setCursor(2,1);
            lcd.print("!WRONG TYPE!");

            counter_time_anamaly_2 = 0;
            counter_time_anamaly_1 = 0;
        
        }




        //проверка времени нахождения в аномалии 1
        if(counter_time_anamaly_1 >= TIME_SECOND){

            analogWrite(3, 0);
            
            adc = 0;
            counter_time_anamaly_1 = 0;
            flag_counter_time_anamaly_1 = RES_AN_1;
            EEPROM.write(flag_counter_time_anamaly_1_ee ,flag_counter_time_anamaly_1);  
        }

        //проверка времени нахождения в аномалии 2
        if(counter_time_anamaly_2 >= TIME_SECOND){

            analogWrite(3, 0);
            
            adc = 0;
            counter_time_anamaly_2 = 0;
            flag_counter_time_anamaly_2 = RES_AN_2;  
            EEPROM.write(flag_counter_time_anamaly_2_ee ,flag_counter_time_anamaly_2);
        }
        
        //проверка времени нахождения в аномалии 3
        if(counter_time_anamaly_3 >= TIME_SECOND){

            analogWrite(3, 0);
            
            adc = 0;
            counter_time_anamaly_3 = 0;
            flag_counter_time_anamaly_3 = RES_AN_3;  
            EEPROM.write(flag_counter_time_anamaly_3_ee ,flag_counter_time_anamaly_3);
        }











    
    }




    // пауза ожидания очередного пакета от передатчиков. 
    // если в течении времени пакета нет, то вывести надпись
    // НЕТ СИГНАЛА
    if(flag_transmit == 1){
        timeout++;
  
        Serial.println(timeout);
        if ( (timeout == 50000) ){ 
            
            counter_time_anamaly_1 = 0;
            counter_time_anamaly_2 = 0;
            counter_time_anamaly_3 = 0;
            flag_enable_step_motor = 0;
            
            flag_transmit = 0;
            timeout = 0;
            delay(50);
            lcd.clear();

            
            timeout = 0;
        }
    }




  analogWrite(3, 0);


























  
}


ISR(TIMER2_A) {  
  if(flag_enable_step_motor){
    if(cycle_step_motor == 1){PORTC |= (1<<0); PORTC &= ~((1<<1)|(1<<2)|(1<<3));} 
    if(cycle_step_motor == 2){PORTC |= (1<<1); PORTC &= ~((1<<0)|(1<<2)|(1<<3));}
    if(cycle_step_motor == 3){PORTC |= (1<<2); PORTC &= ~((1<<1)|(1<<0)|(1<<3));}
    if(cycle_step_motor == 4){PORTC |= (1<<3); PORTC &= ~((1<<1)|(1<<2)|(1<<0));}
    
    cycle_step_motor++;
    if(cycle_step_motor == 5){cycle_step_motor = 1;}
  }
  else{PORTC &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3));}
}
