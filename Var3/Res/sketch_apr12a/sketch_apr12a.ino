#include "GyverTimers.h"

int cycle_step_motor = 1;

void setup() {

  //Настройка прерывания////////////////////////////////
  Timer2.setFrequency(500);         // Задать частоту прерываний таймера в Гц, вернет реальную частоту в герцах
  Timer2.enableISR(CHANNEL_A);
  /////////////////////////////////////////////////////

  //Выводы драйвера шагового мотора////////////////////
  DDRC |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
  /////////////////////////////////////////////////////
}

void loop() {



}

ISR(TIMER2_A) {  

  if(cycle_step_motor == 1){PORTC |= (1<<0); PORTC &= ~((1<<1)|(1<<2)|(1<<3));} 
  if(cycle_step_motor == 2){PORTC |= (1<<1); PORTC &= ~((1<<0)|(1<<2)|(1<<3));}
  if(cycle_step_motor == 3){PORTC |= (1<<2); PORTC &= ~((1<<1)|(1<<0)|(1<<3));}
  if(cycle_step_motor == 4){PORTC |= (1<<3); PORTC &= ~((1<<1)|(1<<2)|(1<<0));}
    
  cycle_step_motor++;
  if(cycle_step_motor == 5){cycle_step_motor = 1;}
}
