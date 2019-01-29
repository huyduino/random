#include <LCD5110_Graph.h>

LCD5110 myGLCD(7,6,5,3,4);

extern uint8_t SmallFont[];
extern unsigned char TinyFont[];
int sc=10000;
int to=10000;
int rc=0;
double xa=0;
int a, b;


void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(TinyFont);
  randomSeed(analogRead(7));
  rc = sc;  //record = 10000
}



void loop()
{

if(sc>rc)
rc=sc;  // increase th record 

if(sc<=10000) //reseting record to reset speed
rc=sc;


  myGLCD.clrScr();
  myGLCD.drawRect(0, 0, 83, 47);

if(xa == 0)
{
  a=random(0,83);
  b=random(0,47);
if(rc>0&&rc<11000) //seting the speed
xa=150;
if(rc>11000&&rc<12000)
xa=130;
if(rc>12000&&rc<13000)
xa=100;
if(rc>13000&&rc<14000)
xa=65;
if(rc>14000&&rc<15000)
xa=45;
if(rc>15000&&rc<17000)
xa=35;
if(rc>17000&&rc<19000)
xa=25;
if(rc>20000)
xa=20;
}
  xa--; 
  

  

  myGLCD.print("Score ", LEFT, 0);
  myGLCD.print(String(sc), 22, 0);

  if(sc>=15000&&sc<16000)
  {
  myGLCD.print("not bad.", RIGHT , 0);
  }

  if(sc>=16000&&sc<17000)
  {
  myGLCD.print("Good!", RIGHT , 0);
  }

  
if(sc>=17000&&sc<19000)  {
  myGLCD.print("MY BOY!", RIGHT , 0);
  }

  
  if(sc>=19000)
  {
  myGLCD.print("WOW!!", RIGHT , 0);
  }
  myGLCD.drawCircle(a,b,3);
  myGLCD.drawCircle(a,b,4);
  myGLCD.drawCircle(a,b,5);
  
  myGLCD.drawCircle(map(analogRead(A1),0,1024,0,83),map(analogRead(A0),0,1024,0,47),1);
  myGLCD.drawCircle(map(analogRead(A1),0,1024,0,83),map(analogRead(A0),0,1024,0,47),3);
  myGLCD.drawCircle(map(analogRead(A1),0,1024,0,83),map(analogRead(A0),0,1024,0,47),4);


  if(xa == 0)   //loosing one round
{
sc -= 150;
}

if(map(analogRead(A1),0,1024,0,83)<a+5&&map(analogRead(A1),0,1024,0,83)>a-5 &&  map(analogRead(A0),0,1024,0,47)<b+5&& map(analogRead(A0),0,1024,0,47)>b-5) // winning one round
{
    xa=0;
    sc+=100;
}


if(sc<=0)  // Game over
while(1)
{
    myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("game over!", CENTER, 32);
    myGLCD.update();

}

if(sc>=20500)  // WIN!! (IMPOSSIBLE : )) ) 
while(1)
{
    myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("WINNER!!", CENTER, 24);
  myGLCD.print("WHO THE HELL", CENTER, 32);
  myGLCD.print("ARE YOU?!", CENTER, 40);
    myGLCD.update();

}

if(to==0)  // (time out)
while(1)
{
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("T.O! record:", CENTER, 32);
  myGLCD.print(String(sc), CENTER, 40);

  myGLCD.update();

}


  myGLCD.update();
  to --;   // timer decrease
  delay(10);  

}
