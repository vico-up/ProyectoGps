
/*
  Tarea primria 
  *enviar datos gprs

  Tareas secundarias
  *arreglar fecha
  *Mensaje inicial
*/

/* Esta es la version2 de gpstracker usando el arduino mega
*/


#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>
#include <Wire.h>


LiquidCrystal_I2C  lcd(0x27, 16, 2);  //objeto lcd
// The TinyGPSPlus object
TinyGPSPlus gps;

//Declaramos variables y pines

//Por lguna razon visual studio muestr este error pero funciona normal
HardwareSerial &gpsNeo = Serial1; //Nombramos como gpsNeo a los pines tx1, rx1 del mega
static const uint32_t GPSBaud = 9600; //configuramos a 9600 ojo q 4800 tiene problemas


float latitud = 0;
float longitud = 0;
int dia = 0;
int mes = 0;
int year = 0;
int hora = 0;
int minutos = 0;
int segundos = 0;
int horaActual = 0;
int diaActual = 0;
int velocidad = 0;



void setup()
{
  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  gpsNeo.begin(GPSBaud);  //iniciamos comucacion serial
  Serial.println(F("GpsTracker"));
  Serial.println(F("Buscando Satelites"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F(".............."));
  Serial.println();
  


}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsNeo.available() > 0)
  {
    if (gps.encode(gpsNeo.read()))
    {
      //datosGps();
      coordenadas();
      fecha();
      horita();
      veloz();
    }
  }


  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while(true);
  }
}

void coordenadas()
{
  Serial.print(F("Coordenadas: ")); 
  if (gps.location.isValid())
  {
    latitud = gps.location.lat();
    longitud = gps.location.lng();
    Serial.print(latitud, 6);
    Serial.print(F(","));
    Serial.print(longitud, 6);
  }
  else
  {
    Serial.print(F("INVALIDO"));
  }
}

void fecha()
{
  Serial.print(F("  Fecha/Tiempo: "));
  if (gps.date.isValid())
  {
    dia = gps.date.day();
    mes = gps.date.month();
    year = gps.date.year();
    diaActual = fechaBolivia(hora);
    
    Serial.print(diaActual);
    Serial.print(F("/"));
    Serial.print(mes);
    Serial.print(F("/"));
    Serial.print(year);

  }
  else
  {
    Serial.print(F("INVALIDO"));
  }

}

void horita()
{
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    hora = gps.time.hour();
    minutos = gps.time.minute();
    segundos = gps.time.second();
    horaActual = horaBolivia(hora);

    if (horaActual < 10)
    {
      Serial.print(F("0")); lcd.print(F("0"));
    }
      Serial.print(horaActual);
      Serial.print(F(":"));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(horaActual); lcd.print(":");

    
    if (minutos < 10){Serial.print(F("0")); lcd.print(F("0"));}
      Serial.print(minutos);
      Serial.print(F(":"));
      lcd.print(minutos); lcd.print(":");
    
    if (segundos < 10) {Serial.print(F("0")); lcd.print(F("0"));}
    Serial.print(segundos);
    Serial.print(F(" "));
    lcd.print(segundos); 
  }
  else
  {
    Serial.print(F("INVALID"));
  }

}

void veloz()
{
  Serial.print(F("Velocidad: ")); 
  if (gps.speed.isValid())
  {
    velocidad = gps.speed.kmph();
    Serial.print(gps.speed.kmph());
    lcd.setCursor(0, 1);
    lcd.print("Vel:"); lcd.print(velocidad); lcd.print(" Km/h");
  }
  else
  {
    Serial.print(F("INVALIDO"));
  }

  Serial.println();
}

//Funcion que muestra todos los datos del gps
/*void datosGps()
{
  Serial.print(F("Coordenadas: ")); 
  if (gps.location.isValid())
  {
    latitud = gps.location.lat();
    longitud = gps.location.lng();
    Serial.print(latitud, 6);
    Serial.print(F(","));
    Serial.print(longitud, 6);
  }
  else
  {
    Serial.print(F("INVALIDO"));

  }

  Serial.print(F("  Fecha/Tiempo: "));
  if (gps.date.isValid())
  {
    dia = gps.date.day();
    mes = gps.date.month();
    year = gps.date.year();
    
    Serial.print(diaActual);
    Serial.print(F("/"));
    Serial.print(mes);
    Serial.print(F("/"));
    Serial.print(year);


  }
  else
  {
    Serial.print(F("INVALIDO"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    hora = gps.time.hour();
    minutos = gps.time.minute();
    segundos = gps.time.second();
    horaActual = horaBolivia(hora);

    if (horaActual < 10)
    {
      Serial.print(F("0")); lcd.print(F("0"));
    }
      Serial.print(horaActual);
      Serial.print(F(":"));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(horaActual); lcd.print(":");

    
    if (minutos < 10){Serial.print(F("0")); lcd.print(F("0"));}
      Serial.print(minutos);
      Serial.print(F(":"));
      lcd.print(minutos); lcd.print(":");
    
    if (segundos < 10) {Serial.print(F("0")); lcd.print(F("0"));}
    Serial.print(segundos);
    Serial.print(F(" "));
    lcd.print(segundos); 
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("Velocidad: ")); 
  if (gps.speed.isValid())
  {
    velocidad = gps.speed.kmph();
    Serial.print(gps.speed.kmph());
    lcd.setCursor(0, 1);
    lcd.print("Vel:"); lcd.print(velocidad); lcd.print(" Km/h");
  }
  else
  {
    Serial.print(F("INVALIDO"));
  }

  Serial.println();
}*/

int horaBolivia (int hora)
{
  horaActual = hora - 4;
  if(horaActual < 0)
  {
    horaActual = horaActual + 24;
    
  }

  return horaActual;
}

int fechaBolivia (int hora)
{
  if((hora>=0) && (hora<=4))
  {
    diaActual = dia - 1;
  }

  /*if(diaActual = 0)
  {

  }*/

  return diaActual;
}