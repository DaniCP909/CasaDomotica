#include <DHT.h>
#include <SoftwareSerial.h>
#include <string.h>

#define DHTPIN 2     // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE DHT11   // Tipo de sensor DHT utilizado

//Analogicos
const int ldrPin1 = A0;  // Pin analógico donde está conectado el LDR
const int ldrPin2 = A1;  // Pin analógico donde está conectado el LDR
const int co2 = A15;

int iman;


//Digitales
#define DHTPIN 31     // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE DHT11   // Tipo de sensor DHT utilizado
const int calefactor = 3; // Pin donde esta conectado el sistema de calefacción

#define pirPin 40 //Sensor de movimiento

const int ledPin = 11;   // Pin PWM donde está conectado el LED de la luz externa

SoftwareSerial s(50,51);  //RX,TX Comunicación entre placas

const int ventilador = 47 ; // motor aire acondicionado

//MOTOR PERSIANA 
#define E1 10  // Enable Pin for motor 1
#define I1 8     // Control pin 1 for motor 1
#define I2 9     // Control pin 2 for motor 1

const int ledInterior = 4;


int val; // variable aux



bool bajar =true;
bool motionState = false ;
DHT dht(DHTPIN, DHTTYPE);

bool luzInterna = false; //luz apagada;
bool AutoLI = false;
bool persiana = false;  //persiana bajada
bool autventilacion= false;
bool ventilacion = false; //ventilacion apagada
bool seguridad = false; //seguridad apagada
bool calefaccion = false; //calefaccion apagada
bool AutoCa = false;


void setup() {
  pinMode(ledPin, OUTPUT);  // Configurar el pin del LED como salida
  pinMode(calefactor, OUTPUT);  // Configurar el pin del LED como salida
  for (int i = 8 ; i<11 ; i++)                     // Inicializamos los pines del motor
  pinMode( i, OUTPUT);
  pinMode(ledInterior, OUTPUT);

  pinMode(ventilador,  OUTPUT) ;
  pinMode(movimiento, INPUT);

   pinMode(co2, INPUT);

  Serial.begin(9600);
  s.begin(9600);
  dht.begin(); 

}

void loop() {
  comunicacion();//analogWrite (ledInterior,  255);
  //movimiento();

  delay(100);


}

void leeriman(){
  iman = analogRead(A8);
  Serial.println(iman);
  delay(500);
}

void comunicacion(){
  String cadenaEntrada="";
  String cadenaSalida="";

  if(s.available()>0){
  cadenaEntrada = s.readStringUntil('\n');
  Serial.println(cadenaEntrada);

  if(cadenaEntrada=="LI"){ //Luz interior
    Serial.println("Luz Interior");
    if(luzInterna){
      apagaIlumniacionInterior();
      Serial.println("Apago");
    }else{
      enciendeIlumniacionInterior();
      Serial.println("Enciendo");

    }
    
  }

  if(cadenaEntrada=="PE"){//Persiana
    if(persiana){
      bajarPersiana();
      persiana=false;
      comBack("PEF");
    }else{
      subirPersiana();
      persiana=true;
      comBack("PEN");
    }
  }
  if(cadenaEntrada=="VE"){//Ventilación
  if(!autventilacion)
    if(ventilacion){
      analogWrite (ventilador,  0);
      ventilacion= false;
    }else{
      analogWrite (ventilador,  255);
      ventilacion=true;
    }
  }
  if(cadenaEntrada=="SEN"){//Seguridad ON
    seguridadON();
    seguridad= true;
  }
  if(cadenaEntrada=="SEF"){//Seguridad OFF
    seguridadOFF();
    seguridad = false;
  }
  if(cadenaEntrada=="CA"){//Calefacción
    if(calefaccion){
      analogWrite (calefactor,  LOW);
      calefaccion=false;
    }else{
      analogWrite (calefactor,  HIGH);
      calefaccion=true;
    }
  }
 }
}

void comBack(String data){
    for (int i = 0; i < data.length(); i++)
    {
      s.write(data[i]); 
    }
}


void enciendeIlumniacionInterior(){
      digitalWrite(ledInterior, HIGH);
      luzInterna=true;
      comBack("LON");
}

void apagaIlumniacionInterior(){
      digitalWrite(ledInterior, LOW);
      luzInterna=false;
      comBack("LOF");
}
void seguridadON(){}
void seguridadOFF(){}

void automaticoExterno(){
  static int lastBrightness = 0;
  int ldrValue = analogRead(ldrPin1);  // Leer el valor del LDR
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

void calefactorAutomatico(){
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


void bajarPersiana() {

    digitalWrite(E1, HIGH);     // Activamos Motor1
    digitalWrite(I1, HIGH);     // Arrancamos
    digitalWrite(I2, LOW);
    delay(3000);

    digitalWrite(E1, LOW);      // Paramos Motor 1
    delay(1000);
    Serial.print("Bajando");
    bajar= false;
  }

  void subirPersiana() {
    digitalWrite(E1, HIGH);     // Activamos Motor1
    digitalWrite(I1, LOW);      // Arrancamos con cambio de direccion
    digitalWrite(I2, HIGH);
    delay(3000);

    digitalWrite(E1, LOW);      // Paramos Motor 1
    delay(1000);
    Serial.print("Subiendo");
    bajar= true;

  }

  void aireAcondicionadoAutomatico(){
            for ( int n = 0 ; n < 255 ; n++)
          {
              analogWrite (ventilador,  n) ;
              delay(15) ; //Tarda 3,825
          }
          analogWrite (ventilador,  0) ;
          Serial.println("Airee");
  }

  void movimiento(){
    val = digitalRead(pirPin);
    if (val == HIGH) {
      digitalWrite(ledInterior, HIGH); 
      if (motionState == false) {
        Serial.println("¡Movimiento detectado!");  
        motionState = true;
      }
    }
    else {
      digitalWrite(ledInterior, LOW); 
      if (motionState == true) {
        Serial.println("¡Movimiento finalizado!");
        motionState = false;
      }
    }
  }

void automaticoCO2(){
    int sensorData = analogRead(co2);
  Serial.print("Calidad de Aire:");
  Serial.print(sensorData, DEC);
  Serial.println(" PPM");
  delay(100);

	if (sensorData > 120.0){

    //Activamos motor velocidad 200
		analogWrite(ventilador, 200);

    delay(3000);
	}
  else {
    //Paramos motor
    analogWrite(ventilador, 0);
  }
}


