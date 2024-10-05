/*
 * This is a basic example on how to use Espalexa and its device declaration methods.
 */ 
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif


#define ESPALEXA_MAXDEVICES 5
// #define ESPALEXA_DEBUG s
#include <Espalexa.h>

// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);

// Change this!!
//const char* ssid = "TELWEB_0F40";
const char* ssid = "Tenda_38ECE0";
const char* password = "arzechelimagh";

boolean wifiConnected = false;

Espalexa espalexa;

EspalexaDevice* device1; //this is optional

void ImpostaLuce(uint8_t brightness);
int StatoAttuale = 0;
int ledPin = 12;

void setup()
{
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    
    // Define your devices here. 
    espalexa.addDevice("Luce", firstLightChanged); //simplest definition, default state off
    //device3 = new EspalexaDevice("LuceProva", thirdLightChanged); //you can also create the Device objects yourself like here
    //espalexa.addDevice(device3); //and then add them
    //device3->setValue(128); //this allows you to e.g. update their state value at any time!

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//our callback functions
void firstLightChanged(uint8_t brightness) {
    Serial.print("Device 1 changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
      ImpostaLuce(brightness);
    }
    else  {
      Serial.println("OFF");
    }
}

void secondLightChanged(uint8_t brightness) {
    Serial.print("Device 2 changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
    }
    else  {
      Serial.println("OFF");
    }
}

void thirdLightChanged(uint8_t brightness) {
    Serial.print("Device 3 changed to ");
    
    //do what you need to do here

    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
    }
    else  {
      Serial.println("OFF");
    }
}

// connect to wifi â€“ returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void ImpostaLuce(uint8_t brightness)
{
    
    int NewStato = brightness * 4;
    int FadeValue  = 0;
    
    Serial.print("Luce da impostare al ");
    Serial.print((100 * brightness) / 255);    
    Serial.println("%");        

    Serial.print("Quindi al valore: ");
    Serial.print(NewStato);    
    Serial.println(" (0/1024)");        
    
    if (NewStato < 0) {
      NewStato = 0;
    } 
    if (NewStato >= 1024) {
     NewStato = 1024;
    }

    int passo = (NewStato - StatoAttuale) / 30;
    Serial.print("Stato: ");
    Serial.println(StatoAttuale);
    Serial.print("NewStato: ");
    Serial.println(NewStato);
    Serial.print("Passo: ");
    Serial.println(passo);

    if (passo > 0) {
      Serial.println("Incremento");

      for (FadeValue = StatoAttuale ; FadeValue <= NewStato; FadeValue += passo) {
        delay(20);
        Serial.println(FadeValue);    
        analogWrite(ledPin, FadeValue);
      }
    }  
    if (passo < 0) {
      Serial.println("Decremento");
      
      for (FadeValue = StatoAttuale ; FadeValue >= NewStato; FadeValue += passo) {
        delay(20);
        Serial.println(FadeValue);    
        analogWrite(ledPin, FadeValue);
      }
    }  

    analogWrite(ledPin, NewStato);
    StatoAttuale = NewStato;
        
    Serial.print("Regolata la luce a: ");
    Serial.print(NewStato);    
    Serial.println(" (0/1024)");        
    
    
    Serial.print("Regolata la luce al ");
    Serial.print(NewStato*100/1024);    
    Serial.println("%");        
    Serial.println(" ");        

    String message = "Regolo la luce al ";
    message += NewStato*100/1024;
    message += "%";
     
}