#include <DHT.h>
#include <SoftwareSerial.h>
#include <string.h>

SoftwareSerial s(50,51);  //RX,TX Comunicación entre placas

//Analogicos
const int ldrPin1 = A0;  // Pin analógico donde está conectado el LDR
const int ldrPin2 = A1;  // Pin analógico donde está conectado el LDR
const int co2 = A15;  //Pin analógivo donde está el sensor de CO2



//Digitales


#define DHTPIN1 31     // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE1 DHT11   // Tipo de sensor DHT utilizado
#define DHTPIN2 32     // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE2 DHT11   // Tipo de sensor DHT utilizado

DHT dht1(DHTPIN1, DHTTYPE1);
DHT dht2(DHTPIN2, DHTTYPE2);

const int calefactor = 40; // Pin donde esta conectado el sistema de calefacción

#define pirPin 40 //Sensor de movimiento
const int ventilador = 47 ; // motor aire acondicionado






//MOTOR PERSIANA 
#define E1 10  // Enable Pin for motor 1
#define I1 8     // Control pin 1 for motor 1
#define I2 9     // Control pin 2 for motor 1

//leds
const int ledInterior1 = 3;
const int ledInterior2 = 4;
const int ledInterior3 = 7;
const int ledInterior4 = 6;
const int ledExterior1 = 11;   // Pin PWM donde está conectado el LED de la luz externa
const int ledExterior2 = 12;   // Pin PWM donde está conectado el LED de la luz externa




bool bajar =true;
bool motionState = false ;



bool luzInterna = false; //luz apagada;
bool AutoLI = false;
bool luzExterna = false; //luz apagada;
bool AutoLE = false;
bool persiana = false;  //persiana bajada
bool AutoVE= false;
bool ventilacion = false; //ventilacion apagada
bool seguridad = false; //seguridad apagada
bool calefaccion = false; //calefaccion apagada
bool AutoCA = false;


void setup() {
  pinMode(calefactor, OUTPUT);  // Configurar el pin del LED como salida
  for (int i = 8 ; i<11 ; i++)                     // Inicializamos los pines del motor
  pinMode( i, OUTPUT);
  pinMode(ledInterior1, OUTPUT);
  pinMode(ledInterior2, OUTPUT);
  pinMode(ledInterior3, OUTPUT);
  pinMode(ledInterior4, OUTPUT);
  
  pinMode(ledExterior1, OUTPUT);
  pinMode(ledExterior2, OUTPUT);


  pinMode(ventilador,  OUTPUT) ;

  pinMode(co2, INPUT);

  Serial.begin(9600);
  s.begin(9600);
  dht1.begin(); 
  dht2.begin(); 


}

void loop() {
  if(AutoLI){automaticoInterno();}
  if(AutoLE){automaticoExterno();}
  if(AutoVE){automaticoCO2();}
  if(AutoCA){calefactorAutomatico();}
  comunicacion();



}

void comunicacion(){
  String cadenaEntrada="";
  String cadenaSalida="";

  if(s.available()>0){
  cadenaEntrada = s.readStringUntil('\n');
  Serial.println(cadenaEntrada);
  
  if(cadenaEntrada=="RE"){
    luzInterna = false;
    AutoLI = false;
    luzExterna = false;
    AutoLE = false;
    persiana = false;
    AutoVE= false;
    ventilacion = false;
    seguridad = false;
    calefaccion = false;
    AutoCA = false;
  }


  if(cadenaEntrada=="LI"){ //Luz interior
    Serial.println("Luz Interior");
    if(!AutoLI){
      if(luzInterna){
        apagaIlumniacionInterior();
      }else{
        enciendeIlumniacionInterior();
      }
    }else{
      comBack("ALI");
    }
  }
  if(cadenaEntrada=="LIAN"){
    AutoLI=true; 
    automaticoInterno();
  }  
  if(cadenaEntrada=="LIAF"){
    AutoLI=false; 
  }

  if(cadenaEntrada=="LE"){ //Luz exterior
    Serial.println("Luz Exterior");
    if(!AutoLE){
      if(luzExterna){
        apagaIlumniacionExterior();
      }else{
        enciendeIlumniacionExterior();
      }
    }else{ 
      comBack("ALE");
    }
    
  }
   if(cadenaEntrada=="LEAN"){
    AutoLE=true; 
    automaticoExterno();
  }  
  if(cadenaEntrada=="LEAF"){
    AutoLE=false; 
  }

  if(cadenaEntrada=="PE"){//Persiana
    Serial.println("Persiana");

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
    Serial.println("Ventilacion");
    if(!AutoVE){
      if(ventilacion){
        analogWrite (ventilador,  0);
        ventilacion= false;
      }else{
        analogWrite (ventilador,  255);
        ventilacion=true;
      }
    }
    
  }
  if(cadenaEntrada=="SEN"){//Seguridad ON
    Serial.println("Seguridad ON");

    seguridadON();
    seguridad= true;
  }
  if(cadenaEntrada=="SEF"){//Seguridad OFF
    Serial.println("Seguridad OFF");

    seguridadOFF();
    seguridad = false;
  }
  if(cadenaEntrada=="CA"){//Calefacción
    Serial.println("Calefaccion");
    if(!AutoCA){
      if(calefaccion){
        analogWrite (calefactor,  0);
        calefaccion=false;
      }else{
        analogWrite (calefactor,  200);
        calefaccion=true;
      }
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
      digitalWrite(ledInterior1, HIGH);
      digitalWrite(ledInterior2, HIGH);
      digitalWrite(ledInterior3, HIGH);
      digitalWrite(ledInterior4, HIGH);

      luzInterna=true;
      comBack("LON");
}

void apagaIlumniacionInterior(){
      digitalWrite(ledInterior1, LOW);
      digitalWrite(ledInterior2, LOW);
      digitalWrite(ledInterior3, LOW);
      digitalWrite(ledInterior4, LOW);

      luzInterna=false;
      comBack("LOF");
}

void automaticoInterno(){
  static int lastBrightness = 0;

  int ldrValue1 = analogRead(ldrPin1);  // Leer el valor del LDR 1
  int ldrValue2 = analogRead(ldrPin2);  // Leer el valor del LDR 1
  
  int ldrValue = max(ldrValue1,ldrValue2);

  int brightness = map(ldrValue, 1000, 300, 255, 0);  // Mapear el valor del LDR al rango de PWM (0-255)

  if(ldrValue < 300) {
    analogWrite(ledInterior1, LOW);
    analogWrite(ledInterior2, LOW); 
    analogWrite(ledInterior3, LOW); 
    analogWrite(ledInterior4, LOW); 

    lastBrightness = 0;
  }else{
      analogWrite(ledInterior1, brightness);  // Escribir el valor de brillo en el LED
      analogWrite(ledInterior2, brightness);  // Escribir el valor de brillo en el LED
      analogWrite(ledInterior3, brightness);  // Escribir el valor de brillo en el LED
      analogWrite(ledInterior4, brightness);  // Escribir el valor de brillo en el LED
      
      lastBrightness = brightness;
  }
  
  delay(100);  // Pequeña pausa para evitar lecturas demasiado rápidas
}

void enciendeIlumniacionExterior(){
      digitalWrite(ledExterior1, HIGH);
      digitalWrite(ledExterior2, HIGH);
      
      luzExterna=true;
      comBack("LEON");
}

void apagaIlumniacionExterior(){
      digitalWrite(ledExterior1, LOW);
      digitalWrite(ledExterior2, LOW);

      luzExterna=false;
      comBack("LEOF");
}
void seguridadON(){}
void seguridadOFF(){}



void automaticoExterno(){
  static int lastBrightness1 = 0;
  static int lastBrightness2 = 0;

  int ldrValue1 = analogRead(ldrPin1);  // Leer el valor del LDR 1
  int ldrValue2 = analogRead(ldrPin2);  // Leer el valor del LDR 1


  int brightness1 = map(ldrValue1, 1000, 300, 255, 0);  // Mapear el valor del LDR al rango de PWM (0-255)
  int brightness2 = map(ldrValue2, 1000, 300, 255, 0);  // Mapear el valor del LDR al rango de PWM (0-255)

  if(ldrValue1 < 300) {
    analogWrite(ledExterior1, LOW); 
    lastBrightness1 = 0;
  }else{
      analogWrite(ledExterior1, brightness1);  // Escribir el valor de brillo en el LED
      lastBrightness1 = brightness1;
  }

  if(ldrValue2 < 300) {
    analogWrite(ledExterior2, LOW); 
    lastBrightness2 = 0;
  }else{
      analogWrite(ledExterior2, brightness2);  // Escribir el valor de brillo en el LED
      lastBrightness2 = brightness2;
  }
  
  delay(100);  // Pequeña pausa para evitar lecturas demasiado rápidas
}

void calefactorAutomatico(){
    float temperature1 = dht1.readTemperature();  // Leer la temperatura
    float humidity1 = dht1.readHumidity();        // Leer la humedad
    float temperature2 = dht2.readTemperature();  // Leer la temperatura
    float humidity2 = dht2.readHumidity();        // Leer la humedad

    int temperature= max(temperature1, temperature2);

  // Comprobar si la lectura del sensor fue exitosa
  if (isnan(temperature1) || isnan(humidity1)) {
    Serial.println("Error al leer el sensor DHT11 1");
    return;
  }

  if (isnan(temperature2) || isnan(humidity2)) {
    Serial.println("Error al leer el sensor DHT11 2");
    return;
  }

  // Mapear la temperatura y la humedad a valores de brillo del LED
  int brightness = map(temperature, 20, 50, 255, 0);  // Suponiendo que la temperatura oscila entre 0°C y 40°C

  // Tomar el brillo máximo entre la temperatura y la humedad
  //int brightness = max(tempBrightness, humBrightness);

  analogWrite(calefactor, brightness);  // Escribir el valor de brillo en el LED
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
