#include <LiquidCrystal.h>
#include "DHT.h"
#include <TimerOne.h> 
#include <avr/wdt.h>

// ---------------- LCD ----------------
const int rs = 12, en = 11, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const byte customChar[] = {B00111,B01000,B11110,B01000,B11110,B01000,B00111,B00000};
const byte grado[] = {B00000,B01110,B10001,B10001,B01110,B00000,B00000,B00000};

// ---------------- LEDs ----------------
const int led1 = A5, led2 = 6;

// ------------- Ultrasonido -----------
const int trigPin = 5;
const int echoPin = A4;
float duration, distance;

// --------------- DHT -----------------
const int DHTPIN = 4;
DHT dht(DHTPIN, DHT11);
float h, t;

// -------------- Joystick -------------
const int buttonPin  = 3;
const int VRyPin = A0;
const int VRxPin = A1;
int VRx = 0;
int VRy = 0;
volatile bool joystickPressedRaw = false;
unsigned long lastJoystickPress = 0;
volatile bool joystickPressed = false;

// --------------- Botón principal --------------
const int INT_PIN = 2;
volatile bool counting = false;          
volatile unsigned long buttonTime = 0;  
volatile bool buttonReleased = false;

// ---------------  Lógica de switch --------------

volatile bool detener = false;
volatile bool admin = false;
volatile int Funcionalidad = 0;
volatile int caso = 0;
volatile int lista = 0;
int menu_opt = 0;
int r;
volatile int buttonState = 0; 
unsigned long lastActionTime = 0;
long startTime = 0;
long contador = 0;
volatile int opt = 0;


// ---------------  Variables café --------------

volatile float p_solo = 1;
volatile float p_cortado = 1.10;
volatile float p_doble = 1.25;
volatile float p_premium = 1.5;
volatile float p_chocolate = 2;
float p_solo_act;
float p_cortado_act;
float p_doble_act;
float p_premium_act;
float p_chocolate_act;


// ------------------- Funciones -------------------

// ------------------- Arranque -------------------


void arranque(){  
  if (admin == true){
    return;    
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CARGANDO...");

    for (int i = 0; i < 3; i++){
      digitalWrite(led1, HIGH); 
      delay(500);                     
      digitalWrite(led1, LOW);
      delay(500);
    }
    lcd.clear();
  }
}

// ------------------- Servicio -------------------

bool servicio_a(){
  if (admin == true){
    return;    
  }
  else{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    if (distance <=100){
      return true;
    }
    else{
      lcd.clear();
      lcd.print("ESPERANDO");
      lcd.setCursor(0, 1);
      lcd.print("CLIENTE");
      return false;
    }
  }
}

void servicio_b(){
  Serial.print("PARTE SERVICIO B: ");
  if (admin == true){
    return;    
  }
  // Leemos la humedad relativa
   if(detener){
    detener = false;
    Funcionalidad = 1;
    lcd.clear();
    return;
  }
  if(!detener && !admin){
    h = dht.readHumidity();
    // Leemos la temperatura en grados centígrados (por defecto)
    t = dht.readTemperature();

    // Comprobamos si ha habido algún error en la lectura
    if (isnan(h) || isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    }
    lcd.clear();
    wdt_enable(WDTO_8S);
    startTime = millis();  
    while (millis() - startTime < 5000 && detener == false && admin == false) {
      wdt_reset(); 
      lcd.setCursor(0, 0);
      lcd.print("H:");
      lcd.print(h);
      lcd.print("%");

      lcd.setCursor(0, 1);
      lcd.print("T:");
      lcd.print(t);
      lcd.print("C");
    }
    wdt_disable();
    lcd.clear();
  }

}

bool productos(){
  int random_time;
  float intensidad = 0;

  unsigned long currentTime = millis();
  if (buttonState && detener == false && admin == false) { 
    Serial.println("Button pressed");
    random_time = random(4, 9);
    wdt_enable(WDTO_8S);
    startTime = millis();
    while (millis() - startTime < (random_time*1000)) {
      wdt_reset(); 
      lcd.setCursor(0, 0); 
      lcd.print("Preparando Cafe...");
      intensidad += 0.3;
      Serial.println(intensidad);
      analogWrite(led2, intensidad);
    }
    wdt_disable();
    analogWrite(led2, 0); 
    
    wdt_enable(WDTO_8S);
    startTime = millis();
    while (millis() - startTime < 3000) {
      wdt_reset(); 
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("RETIRE BEBIDA");
    }
    wdt_disable();
    lcd.clear();
    buttonState = 0;
    return true;
  } 
  if (buttonState == 0 && detener == false && admin == false) { 

    if (currentTime - lastActionTime >= 250) {
      lastActionTime = currentTime;
      VRx = analogRead(VRxPin);

      if (VRx<50){ 
        Serial.print("joystick bajado:"); 
          caso--; 
        if (caso<0) { 
          caso = 4; 
        } 
      } 
      if (VRx>800){ 
        Serial.print("joystick subido:"); 
        caso++; 
        if (caso>4) { 
          caso = 0; 
        } 
      }

      lcd.clear();
      lcd.createChar(0, customChar);
      lcd.setCursor(0, 0);

      switch(caso) { 
        case 0: 
          if (detener == false && admin == false){
            lcd.print("Cafe Solo ");
            lcd.print(p_solo);
          }        
          break;
        case 1: 
          if (detener == false && admin == false){
            lcd.print("Cafe");
            lcd.setCursor(0, 1);
            lcd.print("Cortado ");
            lcd.print(p_cortado);  
          }      
          break;
        case 2:
          if (detener == false && admin == false){
            lcd.print("Cafe Doble "); 
            lcd.print(p_doble); 
          }   
          break;
        case 3:
          if (detener == false && admin == false){
            lcd.print("Cafe");
            lcd.setCursor(0, 1);
            lcd.print("Premium ");
            lcd.print(p_premium);
          }        
          break;
        case 4:
          if (detener == false && admin == false){
            lcd.print("Chocolate ");
            lcd.print(p_chocolate);   
          }     
          break;
      }
      lcd.write(byte(0));
    }
    return false;
  }
}

// ------------------- Admin -------------------


void led_admin(){
  if (admin == true){
    Serial.print("ADMIN"); 
    digitalWrite(led1, HIGH); 
    digitalWrite(led2, HIGH); 
  }
  else{
    return;
  }
  
}
void admin_exit(){
  admin = false;
  lcd.clear();
  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW); 
}

bool menu(){
  unsigned long currentTime = millis();

  if (buttonState && admin == true) { 
    Serial.println("Button pressed");    
    lcd.clear();
    buttonState = 0;
    Funcionalidad = menu_opt + 6;
    return false;
  } 
  if (buttonState == 0 && admin == true) { 
    if (currentTime - lastActionTime >= 200) {
      lastActionTime = currentTime;

      VRx = analogRead(VRxPin);

      if (VRx<50){ 
        Serial.print("joystick bajado:"); 
          menu_opt--; 
        if (menu_opt<0) { 
          menu_opt = 3; 
        } 
      } 
      if (VRx>800){ 
        Serial.print("joystick subido:"); 
        menu_opt++; 
        if (menu_opt>3) { 
          menu_opt = 0; 
        } 
      }

      lcd.clear(); 
      lcd.setCursor(0, 0); 

      switch(menu_opt) { 
        case 0: 
          if (detener == false && admin == true){           
            lcd.print("Ver temperatura"); 
          }        
          break;
        case 1: 
          if (detener == false && admin == true){
            lcd.print("Ver distancia"); 
            lcd.setCursor(0, 1);
            lcd.print("sensor");  
          }      
          break;
        case 2:
          if (detener == false && admin == true){
            lcd.print("Ver contador"); 
          }   
          break;
        case 3:
          if (detener == false && admin == true){
            lcd.print("Modificar precios"); 
          }        
          break;
      }
    }
    return false;
  }
}



bool ver_temperatura(){
  unsigned long currentTime = millis();
  VRy = analogRead(VRyPin);

  if (VRy>800){ 
    Serial.print("joystick izq:"); 
    return false;
  }
  else{
    h = dht.readHumidity();
    // Leemos la temperatura en grados centígrados (por defecto)
    t = dht.readTemperature();

    // Comprobamos si ha habido algún error en la lectura
    if (isnan(h) || isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    }    
    
    lcd.createChar(0, grado);
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(t);
    lcd.write(byte(0));
    lcd.print("C");
    
    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print(h);
    lcd.print("%");    
    
    if (currentTime - lastActionTime >= 150) {
      lastActionTime = currentTime;
      lcd.clear(); 
    }
  }  
  return true;
}

bool ver_dist(){
  unsigned long currentTime = millis();
  VRy = analogRead(VRyPin);

  if (VRy>800){ 
    Serial.print("joystick izq:"); 
    return false;
  }
  else{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("cm");
    if (currentTime - lastActionTime >= 150) {
      lastActionTime = currentTime;
      lcd.clear(); 
    }
  }  
  return true;  
}

bool ver_contador(){
  unsigned long currentTime = millis();
  VRy = analogRead(VRyPin);

  if (VRy>800){ 
    Serial.print("joystick izq:"); 
    return false;
  }
  else{

    unsigned long segundosTot =  (millis() - contador) / 1000;

    int horas = segundosTot / 3600;
    int minutos = (segundosTot % 3600) / 60;
    int segundos = segundosTot % 60;

    lcd.setCursor(0, 0);
    if (horas < 10) lcd.print("0");
    lcd.print(horas);
    lcd.print(":");

    if (minutos < 10) lcd.print("0");
    lcd.print(minutos);
    lcd.print(":");

    if (segundos < 10) lcd.print("0");
    lcd.print(segundos);

    if (currentTime - lastActionTime >= 150) {
      lastActionTime = currentTime;
      lcd.clear(); 
    }
  }  
  return true;
}

int ver_precios(){
  unsigned long currentTime = millis();
  
  if (buttonState && admin == true) { 
    Serial.println("Button pressed");    
    lcd.clear();
    buttonState = 0;
    opt = lista;
    p_solo_act = p_solo;
    p_cortado_act = p_cortado;
    p_doble_act = p_doble;
    p_premium_act = p_premium;
    p_chocolate_act = p_chocolate;
    return 0;
  } 
  
  if (buttonState == 0 && admin == true) {
    if (currentTime - lastActionTime >= 200 && admin == true) {
      lastActionTime = currentTime;
      VRx = analogRead(VRxPin);
      VRy = analogRead(VRyPin);

      if (VRx<50){ 
        Serial.print("joystick bajado:"); 
          lista--; 
        if (lista<0) { 
          lista = 4; 
        } 
      } 
      if (VRx>800){ 
        Serial.print("joystick subido:"); 
        lista++; 
        if (lista>4) { 
          lista = 0; 
        } 
      }
      if (VRy>800){ 
        Serial.print("joystick izq:");
        return 2;
      }
      lcd.clear(); 
      lcd.createChar(0, customChar);
      lcd.setCursor(0, 0); 

      switch(lista) { 
        case 0: 
          if (admin == true){
            lcd.print("Cafe Solo ");
            lcd.print(p_solo);
          }        
          break;
        case 1: 
          if (admin == true){
            lcd.print("Cafe");
            lcd.setCursor(0, 1);
            lcd.print("Cortado ");
            lcd.print(p_cortado);  
          }      
          break;
        case 2:
          if (admin == true){
            lcd.print("Cafe Doble "); 
            lcd.print(p_doble); 
          }   
          break;
        case 3:
          if (admin == true){
            lcd.print("Cafe");
            lcd.setCursor(0, 1);
            lcd.print("Premium ");
            lcd.print(p_premium);
          }        
          break;
        case 4:
          if (admin == true){
            lcd.print("Chocolate ");
            lcd.print(p_chocolate);   
          }     
          break;
      }
      lcd.write(byte(0));
    }
    return 1;
  }
}

bool mod_precios(){
  unsigned long currentTime = millis();

  VRy = analogRead(VRyPin);
  if (VRy>800){ 
    Serial.print("joystick izq:");
    return false;
  }
  else{
    if (buttonState && admin == true) { 
      Serial.println("Button pressed");    
      lcd.clear();
      buttonState = 0;
      p_solo = p_solo_act;
      p_cortado = p_cortado_act;
      p_doble = p_doble_act;
      p_premium = p_premium_act;
      p_chocolate = p_chocolate_act;
    } 
    if (buttonState == 0 && admin == true) {
      if (currentTime - lastActionTime >= 200) {
        lastActionTime = currentTime;
        lcd.clear(); 
        lcd.createChar(0, customChar);
        lcd.setCursor(0, 0); 
        switch(opt) { 
          case 0: 
            if (admin == true){            
              VRx = analogRead(VRxPin);
              if (VRx<50){ 
                  p_solo_act+=0.05; 
              } 
              if (VRx>800){ 
                p_solo_act-=0.05;
                if (p_solo_act < 0.0){
                  p_solo_act = 0.0;
                }
              }            
              lcd.print("Cafe Solo ");
              lcd.print(p_solo_act);              
            }        
            break;

          case 1: 
            if (admin == true){ 
              VRx = analogRead(VRxPin);
              if (VRx<50){ 
                  p_cortado_act+=0.05; 
              } 
              if (VRx>800){ 
                p_cortado_act-=0.05;
                if (p_cortado_act < 0){
                  p_cortado_act = 0;
                }
              }
              lcd.print("Cafe");
              lcd.setCursor(0, 1);
              lcd.print("Cortado ");
              lcd.print(p_cortado_act);
            }      
            break;
          case 2:
            if (admin == true){
              VRx = analogRead(VRxPin);
              if (VRx<50){ 
                  p_doble_act+=0.05; 
              } 
              if (VRx>800){ 
                p_doble_act-=0.05;
                if (p_doble_act < 0){
                  p_doble_act = 0;
                }
              }
              lcd.print("Cafe Doble ");
              lcd.print(p_doble_act);
            }   
            break;
          case 3:
            if (admin == true){
              VRx = analogRead(VRxPin);
              if (VRx<50){ 
                  p_premium_act+=0.05; 
              } 
              if (VRx>800){ 
                p_premium_act-=0.05;
                if (p_premium_act < 0){
                  p_premium_act = 0;
                } 
              }
              lcd.print("Cafe");
              lcd.setCursor(0, 1);
              lcd.print("Premium ");
              lcd.print(p_premium_act);
              
            }        
            break;

          case 4:
            if (admin == true){
              VRx = analogRead(VRxPin);
              if (VRx<50){ 
                  p_chocolate_act+=0.05; 
              } 
              if (VRx>800){ 
                p_chocolate_act-=0.05;
                if (p_chocolate_act < 0){
                  p_chocolate_act = 0;
                } 
              }
              lcd.print("Chocolate ");
              lcd.print(p_chocolate_act);
            }        
            break;          
        }
        lcd.write(byte(0));
      }
    }
  }  
  return true;
}

// ------------------- ISR del Timer -------------------
void timerISR() {
  if (counting) {
    buttonTime++;
  }
}

// ------------------- ISR del botón principal -------------------
void buttonChange() {
  int state = digitalRead(INT_PIN);

  if (state == LOW) {
    counting = true;
    buttonTime = 0;
  } else {
    counting = false;
    buttonReleased = true;
  }
}
// ------------------- ISR del botón joystick -------------------

void button_ISR() {
  joystickPressedRaw = true;
}

void handleJoystickButton() {
  if (joystickPressedRaw == true) {
    joystickPressedRaw = false;
    unsigned long now = millis();
    if (now - lastJoystickPress > 200) {
      joystickPressed = true;
      lastJoystickPress = now;
    }
  }
}

// ------------------- Setup -------------------
void setup() {
  // LCD
  lcd.begin(16,2);

  // LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Ultrasonido
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // DHT
  dht.begin();

  // Joystick
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_ISR, FALLING);

  // Botón principal
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), buttonChange, CHANGE);

  // Timer1 cada 1ms
  Timer1.initialize(1000);  // 1000us = 1ms
  Timer1.attachInterrupt(timerISR);

  Serial.begin(9600);
}

// ------------------- Loop principal -------------------
void loop() {
  handleJoystickButton();
  if (joystickPressed) {
        joystickPressed = false;
        buttonState = 1;
  }
  // Comprobar si el botón principal fue liberado
  if (buttonReleased) {
    buttonReleased = false;

    Serial.print("Botón pulsado durante: ");
    Serial.print(buttonTime);
    Serial.println(" ms");

    // Condiciones de pulsación
    if (buttonTime >= 2000 && buttonTime < 3000 && !admin) {
      Serial.println("Volvemos a servicio_a");
      detener = true; 
    }
    else if (buttonTime >= 5000 && !admin) {
      Serial.println("MODO ADMIN");
      admin = true;
    }
    else if (buttonTime >= 5000 && admin) {
      Serial.println("EXIT ADMIN");
      admin = false;
    }

    // Reiniciar contador
    buttonTime = 0;
  }

  // ---------------- estados del programa ----------------
  switch(Funcionalidad) { 
    case 0:
       if (admin == true){
        Funcionalidad = 4;
      } else{
        arranque();
        Funcionalidad = 1;
      }      
      break;

    case 1:
      if (admin == true){
        Funcionalidad = 4;
      }
      else{
        if (servicio_a() == true){
        Funcionalidad = 2;
        } 
      }             
      break;

    case 2:
      if (admin == true){
        Funcionalidad = 4;
      }
      else{
        if (detener == false){
        Funcionalidad = 3;
        caso = 0;
        }
        servicio_b();
      }         
      break;

    case 3:
      if (admin == true){
        Funcionalidad = 4;
      } else {
        if (detener == true || productos() == true) {
          Funcionalidad = 1;
        }
      }          
      break;

    case 4:
      if (admin == false){
        Funcionalidad = 11;
      } else{   
        led_admin();
        Funcionalidad = 5;
      }
      break;

    case 5:
      if (admin == false){
        Funcionalidad = 11;
      } else{
        menu();
      }      
      break;

    case 6:
      if (admin == false){
        Funcionalidad = 11;
      } else{
        if (ver_temperatura() == false){
          Funcionalidad=5;
        }
      }      
      break;

    case 7:
      if (admin == false){
        Funcionalidad = 11;
      } else{
        if (ver_dist() == false){
          Funcionalidad=5;
        }        
      }      
      break;

    case 8:
      if (admin == false){
        Funcionalidad = 11;
      } else if (ver_contador() == false){
          Funcionalidad=5;
        }     
      break;   

    case 9:
      if (admin == false){
        Funcionalidad = 11;
      }
      else{
        r = ver_precios();
        if (r == 0){
          Funcionalidad=10;
        }
        if (r == 2){
          Funcionalidad=5;
        }
      }      
      break;

    case 10:
      if (admin == false){
        Funcionalidad = 11;
      }
      else{
        if (mod_precios() == false){
          lista = 0;
          Funcionalidad=9;
        }
      }      
      break;

    case 11:      
      admin_exit();
      Funcionalidad = 1;
      break;
  }
}

