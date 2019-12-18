#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.begin(16,2);
  pinMode(switchPin, INPUT);
  pinMode(8, OUTPUT);

  lcd.print("Ask me.");
  Serial.print("Ask me.");

  lcd.setCursor(0, 1);

  lcd.print("Crystal Ball.");
  Serial.print("Crystal Ball.");
}

void loop() {
  // put your main code here, to run repeatedly:\

  switchState = digitalRead(switchPin);
  /* if(switchState)
      Serial.print("s HIGH");
   else  
     Serial.print("s LOW");
  Serial.print("\n");*/
  if( switchState != prevSwitchState){
    if(switchState == LOW){
      reply = random(8);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tha Ball Says:.");
      Serial.print("The ball says:");
      lcd.setCursor(0, 1);
      Serial.print(reply);
      if(reply != 0){
        digitalWrite(8,LOW);
      }else{
         digitalWrite(8,HIGH);
       }
      switch (reply){
        case 0:
          lcd.print("Yes.");
          break;
        case 1:
          lcd.print("Most Likely.");
          break;
        case 2: 
          lcd.print("Certainly.");
          break;
        case 3:
          lcd.print("Outlook good.");
          break;
        case 4:
          lcd.print("Unsure.");
          break;
        case 5:
          lcd.print("Ask again.");
          break;
        case 6:
          lcd.print("Doubtful.");
          break;
        case 7:
          lcd.print("No.");
          break;
        default: 
          lcd.print("Zzz...");
          break;
      }
    }
  }
  prevSwitchState = switchState;
}
