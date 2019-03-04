#include <SoftwareSerial.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

#define DEBUG 1                                     // change value to 1 to enable debuging using serial monitor  
#define dht_pin A0                                  // defining pin A0 for DHT sensor
#define lightSensor A1                              // defining pin A1 as input pin for LDR voltage divider

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(dht_pin, DHTTYPE);
Adafruit_BMP085 bmp;
SoftwareSerial esp8266Module(10, 11);               // RX, TX

String network = "PePull_2_plus";                       // your access point SSID
String password = "nothing123";                         // your wifi Access Point password
#define IP "184.106.153.149"                            // IP address of thingspeak.com
String GET = "GET /update?api_key=UXXXCI7CSATD2L88";    // replace with your channel key
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;

void setup()
{
  if(DEBUG){
    Serial.begin(9600);                             // Setting hardware serial baud rate to 9600
  }
  
  dht.begin();  
  esp8266Module.begin(115200);                      // Setting softserial baud rate to 115200
  setupEsp8266();                                   

  if (!bmp.begin()) {
    if(DEBUG){
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
    while (1) {}
  }

  delay(2000);
}

void loop() 
{
    // reconnect wifi just in case
    sendCommand("AT+CWJAP=\""+ network +"\",\""+ password +"\"",20,"OK");
  
    double humi = dht.readHumidity();
    int lightIntensity = analogRead(lightSensor);
    double bmp_temp = bmp.readTemperature();
    double bmp_pressure = bmp.readPressure();
    updateTemp(String(bmp_temp) ,String(lightIntensity),String(bmp_pressure),String(humi));
    delay(30000);
}

//-------------------------------------------------------------------
// Following function setup the esp8266, put it in station made and 
// connect to wifi access point.
//------------------------------------------------------------------
void setupEsp8266()                                   
{
  Serial.println("Setting esp8266");
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
}

//-------------------------------------------------------------------
// Following function sends sensor data to thingspeak.com
//-------------------------------------------------------------------
void updateTemp(String voltage1,String voltage2,String voltage3,String voltage4)
{
  String getData = GET + "&field1=" + voltage1 + "&field2=" + voltage2 + "&field3=" + voltage3 + "&field4=" + voltage4;
  
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  Serial.println(getData);
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  
  esp8266Module.println(getData);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}

//-------------------------------------------------------------------
// send command to ESP8266
//-------------------------------------------------------------------
void sendCommand(String command, int maxTime, char readReplay[])
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  
  while(countTimeCommand < (maxTime*1))
  {
    esp8266Module.println(command);//at+cipsend
    if(esp8266Module.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
 
