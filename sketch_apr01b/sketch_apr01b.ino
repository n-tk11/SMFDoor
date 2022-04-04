#include <usbdrv.h>
#include <avr/io.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;

#define RQ_SW 1
#define RQ_SET_LED_SERVO 2 
#define RQ_LOCK 7


#define NOTHING 3
#define CORRECT 4
#define WAIT 5
#define WRONG 6



int ccount = 0;
int pass[4];

usbMsgLen_t usbFunctionSetup(uint8_t data[8]){
    usbRequest_t *rq = (usbRequest_t*)data;
    static uint8_t sw_state;
    if( rq->bRequest == RQ_SW){
      sw_state = 0;
      if(!digitalRead(PIN_PC0)) {
        delay(100);
        sw_state = 1;
        //usbMsgPtr = (uchar*)&sw_state;
        //return sizeof(sw_state);
        while(!digitalRead(PIN_PC0));  
      }else if(!digitalRead(PIN_PC1)){
        delay(100);
        sw_state = 2;
        //usbMsgPtr = (uchar*)&sw_state;
        //return sizeof(sw_state);
        while(!digitalRead(PIN_PC1));
      }else if(!digitalRead(PIN_PC2)){
        delay(100);
        sw_state = 3;
        //usbMsgPtr = (uchar*)&sw_state;
        //return sizeof(sw_state);
        while(!digitalRead(PIN_PC2));  
      }else if(!digitalRead(PIN_PC3)){
        delay(100);
        sw_state = 4;
        //usbMsgPtr = (uchar*)&sw_state;
        //return sizeof(sw_state);
        while(!digitalRead(PIN_PC3));  
      }
      if(ccount==4){
        lcd.clear();
        ccount=0;
      }
      if(sw_state!= 0){
        lcd.print(sw_state);
        ccount++;
      }
      usbMsgPtr = (uchar*)&sw_state;
      return sizeof(sw_state);
    }

    else if(rq->bRequest == RQ_SET_LED_SERVO){
      uint8_t mode = rq->wValue.bytes[0];
      if(mode == NOTHING){
          digitalWrite(PIN_PB2, LOW);
          digitalWrite(PIN_PB3, LOW);
          digitalWrite(PIN_PB4, LOW);
          
      }else if(mode == CORRECT){
          digitalWrite(PIN_PB2, HIGH);
          digitalWrite(PIN_PB3, LOW);
          digitalWrite(PIN_PB4, LOW);
          lcd.clear();
          lcd.print("CORRECT!!");
          delay(5000);
          lcd.clear();
          myservo.write(0);
      }else if(mode == WAIT){
          digitalWrite(PIN_PB2, LOW);
          digitalWrite(PIN_PB3, HIGH);
          digitalWrite(PIN_PB4, LOW); 
      }else if(mode == WRONG){
          digitalWrite(PIN_PB2, LOW);
          digitalWrite(PIN_PB3, LOW);
          digitalWrite(PIN_PB4, HIGH); 
          lcd.clear();
          lcd.print("WRONG!!");
          delay(5000);
          lcd.clear();
      }

      return 0;
    }

    else if(rq->bRequest == RQ_LOCK){
      myservo.write(90);
      return 0;
    }
    return 0;
}


void setup() {
  pinMode(PIN_PC0, INPUT_PULLUP);
  pinMode(PIN_PC1, INPUT_PULLUP);
  pinMode(PIN_PC2, INPUT_PULLUP);
  pinMode(PIN_PC3, INPUT_PULLUP);
  pinMode(PIN_PB2, OUTPUT);
  pinMode(PIN_PB3, OUTPUT);
  pinMode(PIN_PB4, OUTPUT);
  
  myservo.attach(PIN_PB1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  usbInit();
  usbDeviceDisconnect();
  delay(300);
  usbDeviceConnect();
}

void loop() {
  usbPoll();
  //int i=0;

  //lcd.setCursor(i,0);
  //lcd.print(pass[i]); 

}
