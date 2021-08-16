// Mandar datos Seriales a LabView

const int analogInPin0 = A0;
const int analogInPin1 = A1;
const int analogInPin2 = A2;
const int analogInPin3 = A3;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Leer los valores analogicos
  sensorValue0 = analogRead(analogInPin0);
  // Mapear los valores recibidos
  sensorValue0 = map(sensorValue0, 0, 1023, 0, 255);
  
  sensorValue1 = analogRead(analogInPin1);
  // Mapear los valores recibidos
  sensorValue1 = map(sensorValue1, 0, 1023, 0, 255);
  
  sensorValue2 = analogRead(analogInPin2);
  // Mapear los valores recibidos
  sensorValue2 = map(sensorValue2, 0, 1023, 0, 255);
  
  sensorValue3 = analogRead(analogInPin3);
  // Mapear los valores recibidos
  sensorValue3 = map(sensorValue3, 0, 1023, 0, 255);

  Serial.println();
  Serial.print(sensorValue0);
  Serial.print(' ');
  Serial.print(sensorValue1);
  Serial.print(' ');
  Serial.print(sensorValue2);
  Serial.print(' ');
  Serial.println(sensorValue3);

  delay(500);
}
