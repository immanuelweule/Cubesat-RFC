//===============================================================
// @file:     GroundControl_V0.1
// @brief:    Communication CubeSat - Ground Control
//
// @authors:  Adrian Setka, Immanuel Weule
//
// @hardware: ESP32-DevKitC V4 (ESP32-WROOM-32U)
// @comments: Can only connect to 2,4 GHz, not to 5 GHz
//
// @date:     2021-07-12
//
// @links:    https://randomnerdtutorials.com/esp32-esp8266-plot-chart-web-server/
//            https://randomnerdtutorials.com/esp32-mpu-6050-web-server/
//            https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
//            https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
//
//            https://diyprojects.io/esp8266-web-server-part-1-store-web-interface-spiffs-area-html-css-js/#.YOxFhxOA5eg
//            
//            https://techtutorialsx.com/2019/06/13/esp8266-spiffs-appending-content-to-file/
//===============================================================



//===============================================================
// Header files, variable declarations and WiFi setup
//===============================================================

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h> //File system

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>  //For URL/name instead of IP address
#include <analogWrite.h>
#include <TimeLib.h>

//Set WiFi SSID and password
// const char* ssid = "Kewwin_02"; //WiFi SSID
// const char* password = "2214934027604276"; //WiFi password
const char* ssid = "Apartment 322"; //WiFi SSID
const char* password = "06456469822825645048"; //WiFi password

const char* http_username = "admin";  // username for login
const char* http_password = "admin";  // password for login

const char* PARAM_COMMAND = "inCommand"; //Variable for commandline SPIFFS file
const char* PARAM_COMMAND2 = "inCommand2"; // Variables for last commands 
const char* PARAM_COMMAND3 = "inCommand3";
const char* PARAM_COMMAND4 = "inCommand4";
const char* PARAM_COMMAND5 = "inCommand5";

//Variables for hardware configuration SPIFFS files
const char* PS_EPM = "PSepm";
const char* PS_ODC = "PSodc";
const char* PS_TMS = "PStms";
const char* PS_PAY = "PSpay";
const char* CE_EPM = "CEepm";
const char* CE_ODC = "CEodc";
const char* CE_TMS = "CEtms";
const char* CE_PAY = "CEpay";
const char* API_EPM = "APIepm";
const char* API_ODC = "APIodc";
const char* API_TMS = "APItms";
const char* API_PAY = "APIpay";


int numLog = 0; // numerator for log file
int counter=0;  //Counter for checking connection status in loop

AsyncWebServer server(80);  //Setup a HTTP server



//Just for test purposes of sensormonitoring
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; //I2C sensor connection of BMP280 modul



//===============================================================
// Function declarations
//===============================================================

void ConnectToWiFi() {
  Serial.println("Connecting to ");
  Serial.print(ssid);
  
  WiFi.disconnect();
  WiFi.begin(ssid, password); //For ESP as a station; for ESP as AP use "WiFi.softAP(ssid, password)"

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
      delay(100);
  }
    
  //If connection is successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  return;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File f = fs.open(path, "r");
  if(!f || f.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(f.available()){
    fileContent+=String((char)f.read());
  }
  f.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File f = fs.open(path, "w");
  if(!f){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(f.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  f.close();
}
      
void writeConfig(String ident) {
  String ce = (readFile(SPIFFS, ("/ce" + ident + ".txt").c_str()));
  String ps = (readFile(SPIFFS, ("/ps" + ident + ".txt").c_str()));
  Serial.println(ce);
  Serial.println(ps);
  File f = SPIFFS.open("/configEPM.txt", "a");
  f.printf("%s%s", ce, ps);
  f.close();
  
}

void sendCommand() {
  String command = readFile(SPIFFS, "/inCommand.txt");
  //SPI function to send string
  return;
}

int determineConfig() {
  int config = 0;
  
  return config;
}
//===============================================================
// Test Functions
//===============================================================


String readBMP280Temperature() {
  // Read temperature as Celsius (the default)
  float temp = bmp.readTemperature();
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(temp)) {    
    Serial.println("Failed to read temp from BMP280 sensor!");
    return "";
  }
  else {
    File f = SPIFFS.open("/logA.txt", "a");
    time_t t = now();
    f.printf("Value %d : %.2f C   (%d:%d:%d)\n", numLog, temp, hour(t), minute(t), second(t));
    f.close();
    return String(temp);
  }
}

String readBMP280Altitude() {
  float alt = bmp.readAltitude(1013.25);
  if (isnan(alt)) {
    Serial.println("Failed to read alt from BMP280 sensor!");
    return "";
  }
  else {
    File f = SPIFFS.open("/logA.txt", "a");
    time_t t = now();
    f.printf("Value %d : %.2f m   (%d:%d:%d)\n", numLog, alt, hour(t), minute(t), second(t));
    f.close();
    return String(alt);
  }
}

String readBMP280Pressure() {
  float pres = bmp.readPressure() / 100.0F;
  if (isnan(pres)) {
    Serial.println("Failed to read pres from BMP280 sensor!");
    return "";
  }
  else {
    File f = SPIFFS.open("/logP.txt", "a");
    time_t t = now();
    f.printf("Value %d : %.2f hPa   (%d:%d:%d)\n", numLog, pres, hour(t), minute(t), second(t));
    f.close();
    numLog++;
    return String(pres);
  }
}


//===============================================================
// Setup
//===============================================================

void setup(void){
  
  Serial.begin(115200); //Open a serial connection
  Serial.println("Booting...");

  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //while (1) delay(10);
  }
  
  //Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Header for SPIFFS Files
  const char* header1 = "Log File Temperature\n";
  const char* header2 = "Log File Altitude\n";
  const char* header3 = "Log File Pressure\n";
  writeFile(SPIFFS, "/logT.txt", header1);
  writeFile(SPIFFS, "/logA.txt", header2);
  writeFile(SPIFFS, "/logP.txt", header3);
  writeFile(SPIFFS, "/configEPM", "");
  writeFile(SPIFFS, "/configODC", "");
  writeFile(SPIFFS, "/configTMS", "");
  writeFile(SPIFFS, "/configPAY", "");
  
  
  //Initialize how ESP should act - AP or STA (comment out one initialization)
  //WiFi.mode(WIFI_AP); //Access point mode: stations can connect to the ESP
  WiFi.mode(WIFI_STA); //Station mode: the ESP connects to an access point

  ConnectToWiFi();

  if(!MDNS.begin("cubesat")) {  //Argument of MDNS.begin holds website name (".local" has to be added)
     Serial.println("Error starting mDNS");
     return;
  }
  
  //Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", readBMP280Temperature().c_str());
  });
  server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", readBMP280Altitude().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", readBMP280Pressure().c_str());
  });
  server.on("/command", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/inCommand.txt", "text/text");
  });
  server.on("/ceepm", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/ceEPM.txt", "text/text");
  });
  server.on("/psepm", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/psEPM.txt", "text/text");
  });
  server.on("/ceodc", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/ceODC.txt", "text/text");
  });
  server.on("/psodc", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/psODC.txt", "text/text");
  });
  server.on("/cetms", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/ceTMS.txt", "text/text");
  });
  server.on("/pstms", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/psTMS.txt", "text/text");
  });
  server.on("/cepay", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/cePAY.txt", "text/text");
  });
  server.on("/pspay", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/psPAY.txt", "text/text");
  });
  server.on("/apiepm", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/apiEPM.txt", "text/text");
  });
  server.on("/apiodc", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/apiODC.txt", "text/text");
  });
  server.on("/apitms", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/apiTMS.txt", "text/text");
  });
  server.on("/apipay", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/apiPAY.txt", "text/text");
  });

  //Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    String inputMessage;
    // GET inCommand value on <ESP_IP>/get?inCommand=<inputMessage> and safe SPIFFS file
    if (request->hasParam(PARAM_COMMAND)) {
      inputMessage = request->getParam(PARAM_COMMAND)->value();
      writeFile(SPIFFS, "/inCommand.txt", inputMessage.c_str());
    }
    //GET inConfig value on <ESP_IP>/get?configForm=<inputMessage> and safe SPIFFS files
    else if (request->hasParam(CE_EPM)) {
      inputMessage = request->getParam(CE_EPM)->value();
      writeFile(SPIFFS, "/ceEPM.txt", inputMessage.c_str());

      inputMessage = request->getParam(PS_EPM)->value();
      writeFile(SPIFFS, "/psEPM.txt", inputMessage.c_str());

      inputMessage = request->getParam(CE_ODC)->value();
      writeFile(SPIFFS, "/ceODC.txt", inputMessage.c_str());

      inputMessage = request->getParam(PS_ODC)->value();
      writeFile(SPIFFS, "/psODC.txt", inputMessage.c_str());

      inputMessage = request->getParam(CE_TMS)->value();
      writeFile(SPIFFS, "/ceTMS.txt", inputMessage.c_str());

      inputMessage = request->getParam(PS_TMS)->value();
      writeFile(SPIFFS, "/psTMS.txt", inputMessage.c_str());
      
      inputMessage = request->getParam(CE_PAY)->value();
      writeFile(SPIFFS, "/cePAY.txt", inputMessage.c_str());

      inputMessage = request->getParam(PS_PAY)->value();
      writeFile(SPIFFS, "/psPAY.txt", inputMessage.c_str());

      inputMessage = request->getParam(API_EPM)->value();
      writeFile(SPIFFS, "/apiEPM.txt", inputMessage.c_str());

      inputMessage = request->getParam(API_ODC)->value();
      writeFile(SPIFFS, "/apiODC.txt", inputMessage.c_str());

      inputMessage = request->getParam(API_TMS)->value();
      writeFile(SPIFFS, "/apiTMS.txt", inputMessage.c_str());

      inputMessage = request->getParam(API_PAY)->value();
      writeFile(SPIFFS, "/apiPAY.txt", inputMessage.c_str());
      delay(500);
      writeConfig("EPM");
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/text", inputMessage);
  });

  /*To get update of ADC Value only
  server.on("/readADC", [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    int a = analogRead(Analog_Pin);
    String adcValue = String(a);
    request->send(200, "text/plane", adcValue);
  }); */
  
  
  server.onNotFound(notFound);
  server.begin();
}



//===============================================================ç
// Loop
//===============================================================

void loop(void){

  //Check if ESP is still connected to WiFi and reconnect if connection was lost
  if(counter>6) { //Dont check connection status in every loop (for better runtime)
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi not connected. Try to reconnect...");
      ConnectToWiFi();
    }
    counter=0;
  } else {
    counter++;
  }
  
  //To access your stored values 
  readFile(SPIFFS, "/psEPM.txt");
  readFile(SPIFFS, "/configEPM.txt");

  delay(10000);
}
