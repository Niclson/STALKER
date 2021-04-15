void setup() {
  DDRC |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
  PORTD |= (1<<2);
}

void loop() {
    if(!(PIND&(1<<2))){
        PORTC |= (1<<0); delay(5); PORTC &= ~(1<<0);
        PORTC |= (1<<1); delay(5); PORTC &= ~(1<<1);
        PORTC |= (1<<2); delay(5); PORTC &= ~(1<<2);
        PORTC |= (1<<3); delay(5); PORTC &= ~(1<<3);
    }

}
