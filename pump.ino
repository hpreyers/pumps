//
// Author: Hannes Coopman - with a little help from his friends, ;-)
// 

/*
  HTTP Advanced Authentication example
  Created Mar 16, 2017 by Ahmed El-Sharnoby.
  This example code is in the public domain.
*/

//INCLUDES
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

//VARIABLES
#ifndef STASSID
#define STASSID "" //statische variabele
#define STAPSK  ""
#endif

// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";

//CONST VARIABLES 
//WEBSERVER
const char* ssid = STASSID;
const char* password = STAPSK;
const char* www_username = "admin";
const char* www_password = "admin";
const char* www_realm = "Gelieve in te loggen"; // allows you to set the realm of authentication Default:"Login Required"
//FUNCTION VARIABLES
const int relayPin = D13;
const int led = LED_BUILTIN;

//DYNAMIC VARIABLES
int statusUitgang = digitalRead (led);

ESP8266WebServer server(80);

//const String& Message = String("De ") + value;

const char * htmlMessage = " <!DOCTYPE html> "
"<html> "
"<body> "
"<p>Tempcontrol by HACO2401.  </p>"
"</br>"
"<form action=\"/DOORVERWIJZEN\" method=\"POST\"><input type=\"submit\" value=\"KLIK HIER OM DOOR TE GAAN\"></form>"
"</body> "
"</html> ";

const char * htmlMessage1 = " <!DOCTYPE html> "
"<html> "
"<body> "
"<p>Druk op de knop om de circulatiepomp </p>"
"<p>van de buffer naar de radiators aan of uit te zetten.</p>"
"<p>Status Circulatiepomp: AAN</p>"
"</br>"
"<form action=\"/POMPAAN\" method=\"POST\"><input type=\"submit\" value=\"POMP UIT\"></form>"
"</body> "
"</html> ";

const char * htmlMessage2 = " <!DOCTYPE html> "
"<html> "
"<body> "
"<p>Druk op de knop om de circulatiepomp </p>"
"<p>van de buffer naar de radiators aan of uit te zetten.</p>"
"<p>Status Circulatiepomp: UIT</p>"
"</br>"
"<form action=\"/POMPUIT\" method=\"POST\"><input type=\"submit\" value=\"POMP AAN\"></form>"
"</body> "
"</html> ";

const char * htmlMessagePRE = "<!DOCTYPE HTML>\r\n<html><head><title>Tempcontrol by HACO2401</title> <style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
"body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; } .gridContainer {width: 75.5%;padding: 3.82%; }</style></head><body><br><br>";

const char * htmlMessageTITLE = "<H1>Tempcontrol by HACO2401</H1>";

const char * htmlMessageFORM = "<form action=\"/DOORVERWIJZEN\" method=\"POST\"><input type=\"submit\" value=\"KLIK HIER OM DOOR TE GAAN\"></form>";

const char * htmlMessagePOST = "</body></html>\r\n\r\n";

void createhtmlMessageNEW() {
createhtmlBody();
htmlMessageNEW = htmlMessagePRE;
htmlMessageNEW += htmlMessageTITLE;
htmlMessageNEW += htmlMessageBODY;
htmlMessageNEW += htmlMessagePOST;
return htmlMessageNEW;
}

void createhtmlBody() {
htmlMessageBODY = htmlMessageFORM;
return htmlMessageBODY;
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
	// server.send(200, "text/html", "<form action=\"/DOORVERWIJZEN\" method=\"POST\"><input type=\"submit\" value=\"DOORVERWIJZEN\"></form>");
	// set the built-in led to ON
	digitalWrite (led, 1);
	// create the webpage
	createhtmlMessageNEW();
	// show the NEW html page
	server.send(200, "text/html", htmlMessageNEW);
}

void handleDOORVERWIJZEN() { // If a POST request is made to URI /LED
	// digitalWrite(led,!digitalRead(led));      // Change the state of the LED

	//if (statusUitgang = HIGH){
	digitalWrite (LED_BUILTIN, LOW); 
	server.send(200,"text/html", htmlMessage2);
	//server.send(303); 
	//if (statusUitgang = LOW){
	//	server.send(200,"text/html", htmlMessage2);
	//	server.send(303); 
	//	server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
	//	server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
	//}}
}

void handlePOMPAAN() {
 digitalWrite(LED_BUILTIN, HIGH);      // Change the state of the LED
  
 // if (statusUitgang = HIGH){
    server.send(200,"text/html", htmlMessage2);
    server.send(303); 
 // if (statusUitgang = LOW){
  //  server.send(200,"text/html", htmlMessage2);
     // server.send(303); 
     // server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
 //server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
 // }} 
}

void handlePOMPUIT() {
 digitalWrite(LED_BUILTIN,LOW);      // Change the state of the LED
  
 // if (statusUitgang = HIGH){
    server.send(200,"text/html", htmlMessage1);
    server.send(303); 
 // if (statusUitgang = LOW){
  //  server.send(200,"text/html", htmlMessage2);
    //  server.send(303); 
     // server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
 //server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
  //}} 
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}


void setup() {
  Serial.begin(115200);
  
  pinMode(relayPin, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite (led, LOW);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  // Verbonden.
  Serial.println("OK!");
  
  // Access Point (SSID).
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // IP adres.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
 
  // Gateway
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
 
  // Signaalsterkte.
  long rssi = WiFi.RSSI();
  Serial.print("Signaal sterkte (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
 
  // MAC adres
  Serial.print("ESP MAC: ");
  Serial.println(WiFi.macAddress()); 
 
  // Start server
  Serial.print("Start server...");
  server.begin();
  Serial.println("OK!");

  //ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password))
      //Basic Auth Method with Custom realm and Failure Response
      //return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
      //Digest Auth Method with realm="Login Required" and empty Failure Response
      //return server.requestAuthentication(DIGEST_AUTH);
      //Digest Auth Method with Custom realm and empty Failure Response
      //return server.requestAuthentication(DIGEST_AUTH, www_realm);
      //Digest Auth Method with Custom realm and Failure Response
    {
      return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
    }
  });

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/DOORVERWIJZEN", HTTP_POST, handleDOORVERWIJZEN);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.on("/POMPAAN", HTTP_POST, handlePOMPAAN);
  server.on("/POMPUIT", HTTP_POST, handlePOMPUIT);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
  delay(1000);

}

void loop() {
  //ArduinoOTA.handle();
  server.handleClient();
  
}
