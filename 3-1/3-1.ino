const int sensorPin=A0;
const float baselineTemp=19.0;

const float stageValue = 0.25;

int brightDelay = 5;
int overtimeFlag = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int pinNumber=2; pinNumber<6;pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  int sensorVal=analogRead(sensorPin);
  Serial.print("\nSensor Value:");
  Serial.print(sensorVal);
  float voltage=(sensorVal/1024.0)*5.0;
  Serial.print(" / Volts:");
  Serial.print(voltage);

  float temperature = (voltage) *100;
  Serial.print(" / temperature:");
  Serial.print(temperature);

  if( temperature < baselineTemp ){
  Serial.print(" / LED:0");
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }else if(temperature >= baselineTemp&&temperature < baselineTemp+(stageValue*1)){
  Serial.print(" / LED:1");
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }else if(temperature >= baselineTemp+(stageValue*1)&&temperature < baselineTemp+(stageValue*2)){
  Serial.print(" / LED:2");
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }else if(temperature >= baselineTemp+(stageValue*2)&&temperature < baselineTemp+(stageValue*3)){
  Serial.print(" / LED:3");
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
  }else if(temperature >= baselineTemp+(stageValue*3)&&temperature < baselineTemp+(stageValue*4)){
  Serial.print(" / LED:4");
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }else if (temperature >= baselineTemp+(stageValue*4)){
  Serial.print(" / LED:5 / LED_bright:" + overtimeFlag);
    if((overtimeFlag%brightDelay*2)<brightDelay){
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
    }else{
      digitalWrite(2,HIGH);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,HIGH);
    }

    if(overtimeFlag > 10){
      overtimeFlag = 0;
    }else{
      overtimeFlag = overtimeFlag + 1;
    }
  }
  delay(1);
}
