#include <DHT.h>

#define DHTPIN 2     // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE DHT11   // Tipo de sensor DHT utilizado

const int ldrPin = A0;  // Pin analógico donde está conectado el LDR
const int ledPin = 3;    // Pin PWM donde está conectado el LED
const int calefactor = 6; // Pin PWM donde está conectado el LED
const int aire = 11 ; // motor aire acondicionado

const int led = 13 ;
const int sensor = 3; //sensor PIN
int val; // variable aux

#define E1 10  // Enable Pin for motor 1
#define I1 8     // Control pin 1 for motor 1
#define I2 9     // Control pin 2 for motor 1

bool bajar =true;
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  pinMode(ledPin, OUTPUT);  // Configurar el pin del LED como salida
  pinMode(calefactor, OUTPUT);  // Configurar el pin del LED como salida
  for (int i = 8 ; i<11 ; i++)                     // Inicializamos los pines del motor
  pinMode( i, OUTPUT);

  pinMode(aire,  OUTPUT) ;

  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);

  Serial.begin(9600);
  dht.begin(); 

}

void loop() {
  enciendeLedLDR();
  enciendeCalefactor();

  if(bajar){
    bajarMotor();
  }else{
    subirMotor();
  }

  delay(3000);
  aireAcondicionado();
  delay(5000);
  segIman();

}

void enciendeLedLDR(){
   static int lastBrightness = 0;
  int ldrValue = analogRead(ldrPin);  // Leer el valor del LDR
  int brightness = map(ldrValue, 100, 600, 255, 0);  // Mapear el valor del LDR al rango de PWM (0-255)
  if(ldrValue >600) {
    analogWrite(ledPin, LOW); 
    lastBrightness = 0;
  }else{
      analogWrite(ledPin, brightness);  // Escribir el valor de brillo en el LED
      lastBrightness = brightness;
  }

  
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print("  Brightness: ");
  Serial.println(brightness);
  
  delay(100);  // Pequeña pausa para evitar lecturas demasiado rápidas
}

void enciendeCalefactor(){
   float temperature = dht.readTemperature();  // Leer la temperatura
  float humidity = dht.readHumidity();        // Leer la humedad

  // Comprobar si la lectura del sensor fue exitosa
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error al leer el sensor DHT11");
    return;
  }

  // Mapear la temperatura y la humedad a valores de brillo del LED
  int brightness = map(temperature, 20, 50, 255, 0);  // Suponiendo que la temperatura oscila entre 0°C y 40°C
  int humBrightness = map(humidity, 20, 80, 0, 255);     // Suponiendo que la humedad oscila entre 20% y 80%

  // Tomar el brillo máximo entre la temperatura y la humedad
  //int brightness = max(tempBrightness, humBrightness);

  analogWrite(calefactor, brightness);  // Escribir el valor de brillo en el LED

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print("°C  Humedad: ");
  Serial.print(humidity);
  Serial.print("%  Brillo: ");
  Serial.println(brightness);

  delay(1000);  // Pequeña pausa para evitar lecturas demasiado rápidas
}


void bajarMotor() {

    digitalWrite(E1, HIGH);     // Activamos Motor1
    digitalWrite(I1, HIGH);     // Arrancamos
    digitalWrite(I2, LOW);
    delay(3000);

    digitalWrite(E1, LOW);      // Paramos Motor 1
    delay(1000);
    Serial.print("Bajando");
    bajar= false;
  }

  void subirMotor() {
    digitalWrite(E1, HIGH);     // Activamos Motor1
    digitalWrite(I1, LOW);      // Arrancamos con cambio de direccion
    digitalWrite(I2, HIGH);
    delay(3000);

    digitalWrite(E1, LOW);      // Paramos Motor 1
    delay(1000);
    Serial.print("Subiendo");
    bajar= true;

  }

  void aireAcondicionado(){
            for ( int n = 0 ; n < 255 ; n++)
          {
              analogWrite (aire,  n) ;
              delay(15) ; //Tarda 3,825
          }
          analogWrite (aire,  0) ;
          Serial.println("Airee");
  }

void segIman(){
  val = digitalRead(sensor);
  if(val=LOW){
    digitalWrite(led,HIGH);
          Serial.println("Detectado");

  }else{
    digitalWrite(led,LOW);
          Serial.println("Din Detectar");


  }
}

