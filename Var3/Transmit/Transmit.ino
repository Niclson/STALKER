//TRANSMITTER

#include <VirtualWire.h>
#define size 1
int pot = A0;
byte TX_buffer[size]={0};
byte i;

int val = 192; // Значение, которое передаёт устройство приёмнику и это чило является значение второй аномалии


void setup() 
{
  Serial.begin(1000000);
   pinMode(2, OUTPUT);
  // virtual wire
  vw_set_tx_pin(3); // pin
  vw_setup(200); // bps
  for(i=0;i<size;i++)
  {
     TX_buffer[i]=i;
  }
}

void loop()
{ 
  
  TX_buffer[0] = val;  
  vw_send(TX_buffer, size); 
  vw_wait_tx(); 
  Serial.println(val);
  delay(1000);   
}
