// made by Kristian Lauszus - see http://arduino.cc/forum/index.php/topic,58048.0.html for information
#define gX A1
#define gY A2
#define gZ 

#define aX 
#define aY 
#define aZ 

double zeroValue[2] = { 0 }; // gyroX, gyroY, gyroZ, accX, accY, accZ

/* All the angles start at 180 degrees */
double gyroXangle = 180;
double gyroYangle = 180;
double gyroZangle = 180;

int steps =0;

// Complimentary filter
double compAngleX = 180;
double compAngleY = 180;

// Used for timing
unsigned long timer;

int led1 = 9; 
int led2 = 8;
void setup() {
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  //low battery
  if (readVcc() < 3200){
  digitalWrite(led1,HIGH);
  delay(1000);
  }
  
  delay(100);//wait for the sensor to get ready
  
  // Calibrate sensor in horizontal position
  for (uint8_t i = 0; i < 100; i++) { // Take the average of 100 readings
    zeroValue[0] += analogRead(gX);
    zeroValue[1] += analogRead(gY);
    delay(10);
  }  
  zeroValue[0] /= 100;
  zeroValue[1] /= 100;
  timer = micros(); // start timing
}

void loop() {
  double gyroXrate = -((analogRead(gX)-zeroValue[0])/1.0323); // (gyroXadc-gryoZeroX)/Sensitivity - in quids - Sensitivity = 0.00333/3.3*1023=1.0323
  gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // Without any filter
  
  double gyroYrate = -((analogRead(gY)-zeroValue[1])/1.0323);
  gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);

  //filter to activate lights.
  if (gyroYrate >  200){
    steps += 1;
  }else{
    steps = 0;
  }
  Serial.println(steps);
  if (steps > 30){
    for(int i=0; i<3;i++){
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      delay(500);
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      delay(500);
    }
  }
 
  timer = micros(); // reset timing
  
  delay(10);
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

