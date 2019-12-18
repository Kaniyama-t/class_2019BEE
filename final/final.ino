#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// old
const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
const int sensorPin=A0;

// Initialization
int lcdProceedTime = 0;
int ledProceedTime = 0;
int lcdStrCursor = 0;
String msgLine1 = "";
String msgLine2 = "";

// String Resources 1
      String flightNum = "ANA 412";
      String flightState = "";
      String flightDest = "TAKAMATSU";
      float  destDegree = 0.0;
      int    callFlag = 0;
      int    stateFlag = 11;
const String flightHeader = "{$FLIGHT_NUMBER}  {$FLIGHT_STATE}";
const String floghtInfoMsg = "Thank you for your boarding. {$DESTINATION}'s temp is {$DEGREE}.";//"Thank you for your boarding. This airplane go to {$DESTINATION}. The temperatures is {$DEGREE} degree. ";
const String flightCallMsg = "We are inviting ALL passengers. Please come to boarding gate.";//"We are inviting ALL passengers. Please come to boarding gate.';
const String flightFinalCallMsg = "This is FINAL CALL for {$FLIGHT_NUMBER}. Please come to boarding gate hurry."; //"This is FINAL CALL for {$FLIGHT_NUMBER} that go to {$DESTINATION}. Please come to boarding gate hurry.";

// String Resources 2
const String ontimeText = "ON TIME";
const String delayText =  "  DELAY";

// Const Int Resources 1
const int lcdSwitchTime = 1;
const int ledSwitchTime = 5;

// Const Int Resources 2
const int ONTIME = 10;
const int DELAY = 12;
const int NORMAL_CALL = 1;
const int FINAL_CALL = 2;

// Initialization functions.
void printDisplay(int line, String text);
void shiftLCD(bool init);
String convertMsg(String msg);
void refreshDegree();
void setFlightState(int stateNum);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  lcd.begin(16,2);
  
  // switch
  pinMode(switchPin, INPUT);
  // LED
  pinMode(8, OUTPUT);

  // state & KCD
  setFlightState(ONTIME + NORMAL_CALL);
  
}


void loop() {
  // put your main code here, to run repeatedly:\

  // Switch Listener
  switchState = digitalRead(switchPin);
  if( switchState != prevSwitchState){
    if(switchState == LOW){
      stateFlag++;
      if(stateFlag > 14){
        stateFlag = ONTIME + NORMAL_CALL;
      }
      setFlightState(stateFlag);
    }
  }
  prevSwitchState = switchState;

  // Degree
  // It will be set in setFlightState & shifting, for saving memory.  

  // LED
  if(stateFlag > DELAY){
    if((ledProceedTime % ledSwitchTime*2) < ledSwitchTime){
      digitalWrite(8, HIGH);
    }else{
      digitalWrite(8, LOW);
    }
    if(ledProceedTime>(ledSwitchTime*2)){
      ledProceedTime = 0;
    }else{
      ledProceedTime++;
    }
  }

  // LCD
  if(lcdProceedTime > lcdSwitchTime){
    lcdProceedTime = 0;
    shiftLCD(false);
  }else{
    lcdProceedTime++;
  }

  //delay(100);
}

void setFlightState(int stateNum){
  refreshDegree();
  if(stateNum < 13){
    // ON TIME
    flightState = ontimeText;
  }else{
    // DELAY
    flightState = delayText;
  }
  printDisplay(1, flightHeader);
  stateFlag = stateNum;
  if((stateNum % 2) == 1){
    callFlag = NORMAL_CALL;
    String callmsg = floghtInfoMsg + flightCallMsg;
    Serial.print(callmsg);
    printDisplay(2,callmsg);
  }else{
    callFlag = FINAL_CALL;
    String callmsg = floghtInfoMsg + flightFinalCallMsg;
    Serial.print(callmsg);
    printDisplay(2,callmsg);
  }
}

void refreshDegree(){
  int sensorVal=analogRead(A1);
  Serial.print("SENSOR / [GOT VALUE] Sensor Value:");
  Serial.print(sensorVal);
  float voltage=(sensorVal/1024.0)*5.0;
  Serial.print(" / Volts:");
  Serial.print(voltage);

  float temperature = (voltage) *100;
  Serial.print(" / temperature:");
  Serial.print(temperature);
  Serial.print("\n");

  destDegree = temperature;
}

String convertMsg(String msg){
  String degreestr = String(destDegree,1);
  for (int i = 0; i < 4; i++){
    String replaceFrom = "";
    String replaceTo = "";
    switch(i){
        case 0:
          replaceFrom = "{$FLIGHT_NUMBER}";
          replaceTo = flightNum;
          break;
        case 1:
          replaceFrom = "{$FLIGHT_STATE}";
          replaceTo = flightState;
          break;
        case 2:
          replaceFrom = "{$DESTINATION}";
          replaceTo = flightDest;
          break;
        case 3:
          replaceFrom = "{$DEGREE}";
          replaceTo = degreestr;
          break;
    }
    while(msg.indexOf(replaceFrom) != -1){
      msg.replace(replaceFrom,replaceTo);
    }
  }

  Serial.print("SYSTEM / converted msg : " + msg + "\n");

  return msg;
}

void shiftLCD(bool init){
  // Shift
  refreshDegree();

  if(init){
    lcdStrCursor = 0;
  }else{
    lcdStrCursor++;
  }

  String convertedMsg1 = convertMsg(msgLine1);

  // Line 2 Substring for LCD
  String msgline2part = "";
  if(lcdStrCursor > (msgLine2.length()+16)){
    lcdStrCursor = 0;
  }
  
  if(lcdStrCursor < 16){
    String convertedMsg = convertMsg(msgLine2);    
    msgline2part = convertedMsg.substring(0,lcdStrCursor);
    Serial.print("LCD / shifted msg,as:" + msgline2part + "\n");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(convertedMsg1);
    lcd.setCursor(16-lcdStrCursor, 1);
    lcd.print(msgline2part);  
  }else if((lcdStrCursor) < msgLine2.length() + 16){
    String convertedMsg = convertMsg(msgLine2);    
    msgline2part = convertedMsg.substring(lcdStrCursor-16,lcdStrCursor);
    Serial.print("LCD / shifted msg,as:" + msgline2part + "\n");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(convertedMsg1);
    lcd.setCursor(0, 1);
    lcd.print(msgline2part);  
  }else{
    String convertedMsg = convertMsg(msgLine2);    
    msgline2part = convertedMsg.substring(lcdStrCursor-16,msgLine2.length());
    Serial.print("LCD / shifted msg,as:" + msgline2part + "\n");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(convertedMsg1);
    lcd.setCursor(0, 1);
    lcd.print(msgline2part);  
  }


  // show
}

void printDisplay(int line, String text){
  // --- Convert Msg --------------------------------------
  String msg = text;//convertMsg(text);

  // --- Edit ---------------------------------------------
  if(line == 1){
    msgLine1 = msg;
    Serial.print("OUTPUT / [Refreshed] Line-1 :" + msgLine1 + "\n");
  }else if(line == 2){
    msgLine2 = msg;
    Serial.print("OUTPUT / [Refreshed] Line-2 :" + msgLine2 + "\n");
  }

  // --- Show ---------------------------------------------
  shiftLCD(true);
}
