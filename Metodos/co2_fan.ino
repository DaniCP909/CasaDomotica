int sensorPin = A4;
int sensorData;
int motor = 3;

int speed;
int flag;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(motor,OUTPUT);
  analogWrite(motor,0);
}

void loop() {


  sensorData = analogRead(sensorPin);
  Serial.print("Calidad de Aire:");
  Serial.print(sensorData, DEC);
  Serial.println(" PPM");
  delay(100);

	if (sensorData > 120.0){

    //Activamos motor velocidad 200
		analogWrite(motor, 200);

    delay(3000);
	}
  else {
    //Paramos motor
    analogWrite(motor, 0);
  }
}