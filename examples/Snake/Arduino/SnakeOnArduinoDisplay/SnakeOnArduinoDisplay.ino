#include <TFT_HX8357.h> // Hardware-specific library
#include <Timer.h>
#include "DHT.h"      

#define DHTPIN 2          // Hier die Pin Nummer eintragen wo der Sensor angeschlossen ist
#define DHTTYPE DHT11     // Hier wird definiert was für ein Sensor ausgelesen wird. In 
                          // unserem Beispiel möchten wir einen DHT11 auslesen, falls du 
                          // ein DHT22 hast einfach DHT22 eintragen

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library

DHT dht(DHTPIN, DHTTYPE);

struct coord{
  int x;
  int y;
};

const int maxX= 480;
const int maxY= 320;

unsigned int snakeIndex = 0;
struct coord snakeDrawPos;


const int graphLength = 220;
const int graphSpacing = 2;
//struct coord startList[graphLength];
//struct coord endList[graphLength];

struct coord *tempPath;
struct coord *humidPath;

struct coord graphDrawPos;
Timer graphUpdateTimer;
Timer sensorUpdateTimer;

float temperature = 0;
float humidity = 0;

//int matrix[mapSize][mapSize];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tempPath = new coord[graphLength];
  humidPath = new coord[graphLength];


  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  dht.begin();
  graphDrawPos.x = 10;
  graphDrawPos.y = 150;

  snakeDrawPos.x = 10;
  snakeDrawPos.y = maxY-100;

  tempPath[0].x=0;
  tempPath[0].y=0;
  
  humidPath[0].x=0;
  humidPath[0].y=0;
  //endList[0].x = startList[0].x + 10;
  for(int i=1; i<graphLength; i++)
  {
    tempPath[i].x=tempPath[i-1].x+graphSpacing;
    tempPath[i].y=0;

    humidPath[i].x=humidPath[i-1].x+graphSpacing;
    humidPath[i].y=0;
    //startList[i].x = endList[i-1].x;
    //endList[i].x = endList[i-1].x + 10;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("hallo]");
  //delay(1000);
  if(Serial.available() > 0)
  {
    String serialBuffer = Serial.readStringUntil(']');

    if(serialBuffer.indexOf("r") != -1)
    {
      //clear the display
      tft.fillRect(snakeDrawPos.x,snakeDrawPos.y,100,100,TFT_BLACK);
    }else if(serialBuffer.indexOf("snake") != -1)
    {
      snakeIndex = atoi(serialBuffer.substring(serialBuffer.indexOf("=")).c_str());
    }else if(serialBuffer.indexOf("x=") != -1)
    {
      struct coord point;
      point.x = snakeDrawPos.x+atoi(serialBuffer.substring(serialBuffer.indexOf("=")+1,serialBuffer.indexOf("y")-(serialBuffer.indexOf("=")-1)).c_str());
      point.y = snakeDrawPos.y+atoi(serialBuffer.substring(serialBuffer.indexOf("y=")+2).c_str());

      //tft.fillRect(3*point.x,3*point.y,3,3,/*rand()%snakeIndex*/TFT_BLUE);
      tft.drawPixel(point.x,point.y,/*rand()%snakeIndex*/TFT_BLUE);
    /*  Serial.print("X = ");
      Serial.print(point.x);
      Serial.print("\tY = ");
      Serial.println(point.y);
      Serial.println(serialBuffer.substring(serialBuffer.indexOf("=")+1,serialBuffer.indexOf("y")-(serialBuffer.indexOf("=")-1)));*/
    }
  }
  handleSensor();
}
void handleSensor()
{
  if(sensorUpdateTimer.start(100))
  {
    float filter = 0.8;
    humidity    = ((float)1-filter)*(float)dht.readHumidity()+filter*humidity;
    temperature = ((float)1-filter)*(float)dht.readTemperature()+filter*temperature;
    

    // Nun senden wir die gemessenen Werte an den PC dise werden wir im Seriellem Monitor sehen
    /*Serial.print("Luftfeuchtigkeit: ");
    Serial.print(humidity);                  // Ausgeben der Luftfeuchtigkeit
    Serial.print("%\t");              // Tabulator
    Serial.print("Temperatur: ");
    Serial.print(temperature);                  // Ausgeben der Temperatur
    Serial.write(char(176));                // Schreiben des ° Zeichen
    Serial.println("C");*/
  
    printText("Temperatur:   "+String(temperature,2)+"C" , 20,5,TFT_RED);
    printText("Feuchtigkeit: "+String(humidity,2)+"%" , 20,25,TFT_BLUE);
  }
/*********************( Überprüfen ob alles richtig Ausgelesen wurde )*********************/ 
  
  if(graphUpdateTimer.start(1000))
  {

    tft.fillRect(graphDrawPos.x,graphDrawPos.y-100,50+graphSpacing*graphLength,200,TFT_BLACK);
    //tft.clear();
  
    
    
    shiftBack(tempPath,-int(temperature*2));
    drawGraph(tempPath,TFT_RED);
  
    shiftBack(humidPath,-int(humidity*2));
    drawGraph(humidPath,TFT_BLUE);
  }
}

void printText(String text,int xPos, int yPos,uint16_t color)
{
  tft.setTextSize(2);
  tft.setCursor(xPos,yPos);
  tft.setTextColor(color,TFT_BLACK);
  tft.print(text); 
}
void shiftBack(struct coord *path,int newValue)
{
  for(int i=0; i<graphLength-1; i++)
  {
    path[i].y = path[i+1].y;
  }
  path[graphLength-1].y = newValue;
}
void drawGraph(struct coord *path,uint16_t color)
{  
  for(int i=0; i<graphLength-1; i++)
  {
    tft.drawLine(path[i].x+graphDrawPos.x,path[i].y+graphDrawPos.y,path[i+1].x+graphDrawPos.x,path[i+1].y+graphDrawPos.y,color);
  }
  
}
