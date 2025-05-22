#include <ESP8266WiFi.h>
#include <ArduinoJson.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Ultrasonic Pin variables
const int trigPin = 12; //D6 (ESP8266 Pin)
const int echoPin = 14; //D5 (ESP8266 Pin)

//velocity of sound & centemiter to inch conversion variables
#define CM_TO_INCH 0.393701
int SOUND_VELOCITY = 340;
long duration;
float distanceCm;
float distanceInch;
int percentage = 0;
int tankHeight = 20;

//Network parameter
String request;
char* ssid = "vector";
char* password = "oxygen2020";

//IPAddress ip(192, 168, 0, 107); //set static ip
//IPAddress gateway(192, 168, 0, 1); //set gateway
//IPAddress subnet(255, 255, 255, 0);//set subnet

///////////////////////////////////

//WiFiServer object
WiFiServer server(80);

void setup(){

//Init Serial USB
Serial.begin(115200);

//Ultrasonic Pin configuration
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
//Init ESPBrowser
//WiFi.config(ip, gateway, subnet);
WiFi.begin(ssid, password);
  
// Connect to Wifi network.
while (WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(F("."));
}
  server.begin();
  Serial.println();
  Serial.println(WiFi.localIP());
  server.begin();

  //Wire begin
  //Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight  
  lcd.setCursor(0, 0);   
//  lcd.print("IP:");
  lcd.print(WiFi.localIP());
}

void loop(){
  
WiFiClient client = server.available();

  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  //pir status json funstion call
  pirWebView(client);//Return webpage
}

void pirWebView(WiFiClient client){

  ///ultrasonic manipulation
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Reads the returns of the sound wave from Echo Pin in miliseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Distance calculation
  distanceCm = duration * SOUND_VELOCITY/20000;
  
  // Conversion of Centemeter to Inche
  distanceInch = distanceCm * CM_TO_INCH;

  percentage = 100*(1-(distanceCm/tankHeight));

  //Android Application connection status display
  lcd.setCursor(0, 1);  
  lcd.print("App:Connected");  

  if(percentage < 0){
    percentage = 0;
    distanceInch = 0.0;
    distanceCm = 0.0;
  }
  ////Send wbepage to client
  client.println("HTTP/1.1 200 OK");           // This tells the browser that the request to provide data was accepted
  client.println("Access-Control-Allow-Origin: *");  //Tells the browser it has accepted its request for data from a different domain (origin).
  client.println("Content-Type: application/json;charset=utf-8");  //Lets the browser know that the data will be in a JSON format
  client.println("Server: Arduino");           // The data is coming from an Arduino Web Server (this line can be omitted)
  client.println("Connection: close");         // Will close the connection at the end of data transmission.
  client.println();                            // You need to include this blank line - it tells the browser that it has reached the end of the Server reponse header.
  
  client.print("{\"water_distance_inch\": \"");
  client.print(distanceInch,1);  
  client.print("\", \"water_distance_centemeter\": \"");
  client.print(distanceCm,1); 
  client.print("\", \"water_percentage\": \"");
  client.print(percentage,0);               
  client.print("\"}");              
  
  delay(1000);
}
