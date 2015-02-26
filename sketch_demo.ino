#include <NewPing.h>
#include <Ethernet.h>
#include <SPI.h>

const int SonarTrig = 7; 
const int SonarEcho = 6; 
const int SonarThresholdCM = 100;
const int LedPin = 9;
const int SoundPinAnalog = 0;
const int SoundThresholdCM = 503;
const int TouchPin = 8; 
int touchState = 0;

NewPing sonar(SonarTrig, SonarEcho);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 3);

EthernetServer server(80);
EthernetClient client;
EthernetClient requestSender;

void setup() { 
  
  pinMode(LedPin, OUTPUT); 
  digitalWrite(LedPin, 0);
  pinMode(TouchPin, INPUT); 
  Serial.begin(9600);

  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() { 
  processClientRequest();
  checkTouch();
  if (touchState == 1) {
    checkSonar();
    checkSound();
  }
}


void checkTouch() {
  int value = digitalRead(TouchPin); 
  //Serial.println(value); 
  if (value == 1) {
    if (touchState == 1) {
      touchState = 0;
      digitalWrite(LedPin, 0);
    }
    else {
      touchState = 1; 
    }
    delay(1000);
  }
}

void checkSound() {
  int value = analogRead(SoundPinAnalog); 
  //Serial.println(value);
  if (value < SoundThresholdCM) { 
    digitalWrite(LedPin, 1); // Зажигаем светодиод 
    sendRequest();
    delay(5000); 
  } 
  else { 
    digitalWrite(LedPin, 0); // иначе тушим 
  } 
}

void checkSonar() {
  int distance_cm = sonar.ping_cm(); // get distance
  //Serial.println(distance_cm); // print the distance
  if (distance_cm < SonarThresholdCM) { 
    digitalWrite(LedPin, 1); // Зажигаем светодиод 
    sendRequest();
    delay(5000); 
  } 
  else { 
    digitalWrite(LedPin, 0); // иначе тушим 
  } 
  delay(100); 
}

void sendRequest() {
  Serial.println("Trying to send a request");
  //EthernetClient requestSender;
  // if you get a connection, report back via serial:
  if (requestSender.connect("clients.adciserver.com", 1113)) {
    Serial.println("connected");
    requestSender.print("GET http://clients.adciserver.com:1113/test?say=OMGitWORKS!!");
    requestSender.println(" HTTP/1.1");
    requestSender.println("Host: clients.adciserver.com");
    requestSender.println();    
  } 
  else {
    Serial.println("No connection");
  } 
  if (requestSender.connected()) {
    requestSender.stop();
    Serial.println("connection stopped");
  }
}

void processClientRequest() {
  // listen for incoming clients
  client = server.available();
  if (client) {
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          client.println("It works fine. The room is under control.<br/>");
          if (touchState) {
            client.println("Sensors are turned on.");
          }
          else {
            client.println("Sensors are turned off.");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("client disconnected");
  } 
}
