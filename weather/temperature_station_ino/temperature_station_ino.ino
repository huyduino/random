//TEMPERATURE STATION
//MADE BY RUMMAN ISLAM BD
//DATE:24.12.16

#include <LCD5110_Graph.h>                                      //LIBRARY
#include<DHT.h>
#define TS 7                                                //TEMPERATURE SENSOR PIN NUMBER
#define DHTTYPE DHT11

DHT dht(TS, DHTTYPE);
LCD5110 myGLCD(8,9,10,12,11);
//      LCD PIN NUMBER       
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 12
//      CS   - Pin 11

extern unsigned char SmallFont[];                               //FRONT SIZE
extern unsigned char TinyFont[];

char lightString [4];     
char tempString [5];
char humiString [4];

String Ttr;
String str;                                                //INITILIZE STRING 
String htr;

int lightIntensity ;                                       //INITILIZE VALUE
int Tempsensor;

void setup()                        
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
   
  digitalWrite(LED_BUILTIN, HIGH);                          //FOR BLINKING LED
  delay(100);                       
  digitalWrite(LED_BUILTIN, LOW);    
  delay(100); 
  
//  Tempsensor=DHT.read11(TS);                                    // 1ST SENSOR
  int t=dht.readTemperature();                                        // 2ND SENSOR   
  int h=dht.readHumidity();                                           // 3RD SENSOR 
  lightIntensity = analogRead(A0);   
  lightIntensity = map(lightIntensity, 0, 1000, 0, 100);       //CONVERT THE VALUE INTO %
  delay(1000);



  Ttr =String(t)+"*c";                                //FOR CONVERT THE VALUE   
  htr =String(h)+"%"; 
  Ttr.toCharArray(tempString,5);
  htr.toCharArray(humiString,4);
  str = String(lightIntensity)+"%";
  str.toCharArray(lightString,4);                     //FOR CONVERT THE VALUE  
  delay(1000);
  
  myGLCD.clrScr();
  myGLCD.update();
  myGLCD.setFont(TinyFont);
  myGLCD.print("**WEATHER STATION**",CENTER,0);
  myGLCD.print("MADE BY RUMMAN",CENTER,7);
  myGLCD.print("-------------------------",CENTER,12);
  myGLCD.setFont(SmallFont);
  myGLCD.print("CELCIOUS:",LEFT,18);                    // FOR PRINT IN SCREEN  
  myGLCD.print(tempString,RIGHT,18);
  myGLCD.setFont(TinyFont);
  myGLCD.print("------------------------",LEFT,25);
  myGLCD.setFont(SmallFont);
  myGLCD.print("HUMIDITY:",LEFT,29);
  myGLCD.print(humiString,RIGHT,29);
  myGLCD.setFont(TinyFont);
  myGLCD.print("------------------------",LEFT,36);
  myGLCD.setFont(SmallFont); 
  myGLCD.print("SUN:",LEFT,41);
  myGLCD.print(lightString,RIGHT,41);
  
 
  myGLCD.update();  
  delay(1000);
}


