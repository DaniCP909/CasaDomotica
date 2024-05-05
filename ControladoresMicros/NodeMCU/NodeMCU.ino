#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <string.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);//TX,RX

#define GREEN D4

const char* ssid = "NODE_MCU_AP";
const char* password = "nodemcuadmin";
WiFiServer server(80);
String all_command = "";


bool luzInterna = false;
bool persiana = false;
bool ventilacion = false;
bool seguridad = false;
bool calefaccion = false;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password); 
  server.begin();
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  //  

  pinMode(GREEN, OUTPUT);

  digitalWrite(GREEN, LOW);
  s.begin(9600);

}

void loop() {

  

  WiFiClient client = server.available();
  respuesta();
  if (client) {
    Serial.println(client);
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\r') {
          // End of line reached, check if next character is newline

          Serial.println(request);  // full HTTP command line including GET  and HTTP 1

          // Extract command from request string
          int start = request.indexOf("GET /") + 5;
          int end = request.indexOf("HTTP/");
          String command = request.substring(start, end);

          //Purify the command
          command.replace("\n", "");
          command.replace("\r", "");
          command.replace(" ", ""); // removes all space characters
          command.replace("\t", ""); // removes all tab characters
          command.trim();

          Serial.println(command);


          if (command.equals("LI")) {
            comunicacion("LI");
          }
          if (command.equals("LIAF")) {
            comunicacion("LIAF");
          }
          if (command.equals("LIAN")) {
            comunicacion("LIAN");
          }
          
          if (command.equals("LE")) {
            comunicacion("LE");
          }
          if (command.equals("LEAF")) {
            comunicacion("LEAF");
          }
          if (command.equals("LEAN")) {
            comunicacion("LEAN");
          }

          if (command.equals("PE")) {
            comunicacion("PE");
          }

          if (command.equals("VE")) {
            comunicacion("VE");
          }

          //SEguridad oN
          if (command.equals("SEN")) {
            comunicacion("SEN");

          }

          //SEguridad oFf
          if (command.equals("SEF")) {
            comunicacion("SEF");
          }

          if (command.equals("CA")) {
            comunicacion("CA");

          }

          if (command.equals("RE")) {
            comunicacion("RE");
          }




          if (client.peek() == '\n') {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            String commandWithTags = "<html><body>" + all_command + "</body></html>";
            client.println(commandWithTags);
            break;

          }
        }
      }
    }
  }
}

void comunicacion(String data){
  for (int i = 0; i < data.length(); i++)
    {
      s.write(data[i]); 
      Serial.println(data[i]);
    }
}
void respuesta(){
  String cadenaEntrada="";
  if(s.available()){
    cadenaEntrada = s.readStringUntil('\n');

    if(cadenaEntrada=="OK"){
      digitalWrite(D4, HIGH);
      delay(500);
      digitalWrite(D4, LOW);
      delay(500);
      digitalWrite(D4, HIGH);
      delay(500);
      digitalWrite(D4, LOW);
      delay(500);
    }
    if(cadenaEntrada=="LOF"){
      all_command =  "Luz interna apagada";
      luzInterna=false;
    }
    if(cadenaEntrada=="LON"){
      all_command =  "Luz interna encendida";
      luzInterna=true;
    }
    if(cadenaEntrada=="LEOF"){
      all_command =  "Luz externa apagada";
      luzInterna=false;
    }
    if(cadenaEntrada=="LEON"){
      all_command =  "Luz externa encendida";
      luzInterna=true;
    }
    if(cadenaEntrada=="ALE"){
      all_command= "Automatico luz externa activado";
    }
    if(cadenaEntrada=="ALI"){
      all_command= "Automatico luz interna activado";
    }


    //String commandWithTags = "<html><body>" + all_command + "</body></html>";
    //client.println(commandWithTags);

    Serial.println(all_command);
    

    
  }
  delay(500);
}


