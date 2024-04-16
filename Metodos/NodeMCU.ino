#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <string.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);//TX,RX

#define GREEN D4

const char* ssid = "NODE_MCU_AP";
const char* password = "nodemcuadmin";
WiFiServer server(80);

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

  String all_command = "";

  WiFiClient client = server.available();

  if (client) {
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

          all_command =  command + " is on";  // green is on 


          if (command.equals("green_on")) {
            //digitalWrite(GREEN, HIGH);
            comunicacion('N');
            all_command =  "green is on";
          }

          if (command.equals("green_off")) {
            //digitalWrite(GREEN, LOW);
            comunicacion('F');
            all_command =  "green is off";
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

void comunicacion(char data){
   s.write(data);
  


  if(s.available()){
    char c =s.read();

    if(c=='O'){
      digitalWrite(D4, HIGH);
      delay(500);
      digitalWrite(D4, LOW);
      delay(500);
      digitalWrite(D4, HIGH);
      delay(500);
      digitalWrite(D4, LOW);
      delay(500);
      


    }

    if(c=='N'){
      digitalWrite(D4, HIGH);

    }if(c=='F'){
      digitalWrite(D4, LOW);
    }
    
    //data=s.read();
    Serial.println(c);
  }
  delay(500);
}

