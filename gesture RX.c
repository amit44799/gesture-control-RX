 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSN_GPIO    8
#define CE_GPIO     7

#define RIGHT_FORWARD     5  
#define RIGHT_BACKWARD    4  
#define LEFT_FORWARD      3  
#define LEFT_BACKWARD     2  

RF24 radio(CE_GPIO, CSN_GPIO);

const byte Address[6] = "00001";
unsigned char Received_Command = 0,Speed_index = 0,Run_Stop_Mode = 0;  
unsigned char Rx_Array[2];
unsigned int Run_Stop_Counter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RIGHT_FORWARD,OUTPUT);   
  pinMode(RIGHT_BACKWARD,OUTPUT); 
  pinMode(LEFT_FORWARD,OUTPUT);
  pinMode(LEFT_BACKWARD,OUTPUT); 
  radio.begin();
  radio.openReadingPipe(0, Address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("START");
}

void loop()
{
  if (radio.available())
  {  
    delay(1);
    radio.read(&Rx_Array, 2);
    Received_Command = Rx_Array[0];
    Speed_index = Rx_Array[1];
    Serial.print(Received_Command);
    Serial.print(" , ");
    Serial.println(Speed_index);
  }
  if(Run_Stop_Mode==0) 
  {
    digitalWrite(RIGHT_FORWARD,LOW);
    digitalWrite(RIGHT_BACKWARD,LOW);
    digitalWrite(LEFT_FORWARD,LOW);
    digitalWrite(LEFT_BACKWARD,LOW);
    Run_Stop_Counter++;
    if(Run_Stop_Counter>=((5-Speed_index)*100))
    {
      Run_Stop_Counter = 0;
      Run_Stop_Mode = 1;
    }
  }
  else if(Run_Stop_Mode==1)
  {
    Run_Stop_Counter++;
    if(Run_Stop_Counter>=((Speed_index)*100))   
    {
      Run_Stop_Counter = 0;
      Run_Stop_Mode = 0;
    }
    
    if(Received_Command == 1)          
    {
      digitalWrite(LEFT_FORWARD,HIGH);
      digitalWrite(RIGHT_FORWARD,HIGH);
    }
    else if(Received_Command == 2)     
    {
      digitalWrite(LEFT_BACKWARD,HIGH);
      digitalWrite(RIGHT_BACKWARD,HIGH);
    }
    else if(Received_Command == 3)
    {
      digitalWrite(LEFT_FORWARD,HIGH);
      digitalWrite(RIGHT_BACKWARD,HIGH);
    }
    else if(Received_Command == 4)   
    {
      digitalWrite(RIGHT_FORWARD,HIGH);
      digitalWrite(LEFT_BACKWARD,HIGH);
    }
    else if(Received_Command == 0)   
    {      
      digitalWrite(RIGHT_FORWARD,LOW);
      digitalWrite(RIGHT_BACKWARD,LOW);
      digitalWrite(LEFT_FORWARD,LOW);
      digitalWrite(LEFT_BACKWARD,LOW);
    }
    else                                
    {
      digitalWrite(RIGHT_FORWARD,LOW);
      digitalWrite(RIGHT_BACKWARD,LOW);
      digitalWrite(LEFT_FORWARD,LOW);
      digitalWrite(LEFT_BACKWARD,LOW);
    }
  }
}
