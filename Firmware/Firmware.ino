#include <Ultrasonic.h>     //Carrega a biblioteca do sensor ultrassonico
#include <LiquidCrystal.h>  //Carrega a biblioteca LiquidCrystal
#include <SPI.h>      //to ethernet
#include <Ethernet.h>
#include "pinout.h"

/** STATE DEFINITIONS **/
#define WAIT        0
#define MOVE_IN     1
#define READ_COLOR  2
#define READ_HEIGHT 3
#define MOVE_OUT    4

/** GENERAL DEFINITIONS **/
#define MOTOR_OFF 0
#define MOTOR_ON  150

/** GLOBAL VARIABLES **/
char state = 0; // To the state machine
char team[]="Elvis";
char instaled[]="LTPA";
int altura;
int color;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // assign a MAC address for the ethernet controller.
char server[] = "pi2engenharia.000webhostapp.com";
IPAddress ip(145, 14, 145, 236);            // Set the static IP address to use if the DHCP fails to assign
//IPAddress myDns(192, 168, 0, 1);

/** RESOURCE DECLARATIONS **/
EthernetClient client;
LiquidCrystal lcd(30, 32, 34, 36, 38, 40);          //Define os pinos que serão utilizados para ligação ao display
Ultrasonic ultrasonic(pino_trigger, pino_echo);     //Inicializa o sensor nos pinos definidos acima

/*---------------------------------------------------------------------
  SETUP
---------------------------------------------------------------------*/
void setup() {
  pinMode(INFRA_1, INPUT);
  pinMode(INFRA_2, INPUT);
  pinMode(INFRA_3, INPUT);
  pinMode(MOTOR_A, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  analogWrite(MOTOR_A, MOTOR_OFF);

  /** SERIAL INIT **/
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println("Serial initialized");
  delay(100);

  /** START THE ETHERNET CONNECTION **/
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);  // try to congifure using IP address instead of DHCP:
  }
  Serial.println("Ethernet initialized");
  delay(1000);    // give the Ethernet shield a second to initialize

  /** START THE LCD **/
  lcd.clear();              //Limpa a tela
  lcd.begin(16, 2);         //Define o número de colunas e linhas do LCD
  lcd.print ("PI-2 Elvis / Leo ");
  Serial.println("Welcome to PI-2!!!");
  analogWrite(MOTOR_A, MOTOR_OFF);
}

/*---------------------------------------------------------------------
  LOOP
---------------------------------------------------------------------*/
void loop() {
  delay(20);
  switch (state) {
    /** State Wait: will do nothing until the infrared sensor 1 (positioned in the begin of the conveyor) detect the object*/
    case WAIT:
      analogWrite(MOTOR_A, MOTOR_OFF);
      if (analogRead(INFRA_1) < 200) {
        Serial.println("Object detected");
        lcd.setCursor(0, 1);         // (column 0) of the second line (line 1):
        lcd.clear();
        lcd.print ("motor ligado");
        sound_alert();
        analogWrite(MOTOR_A, MOTOR_ON);
        state = MOVE_IN;
      }
      break;
    
    /** State Move In: keep the DC motor running until the object reach the infrared sensor 2*/
    case MOVE_IN:
      if (analogRead(INFRA_2) < 200) {
        analogWrite(MOTOR_A, MOTOR_OFF);
        state = READ_COLOR;
      }
      break;
    
    /** State Read Color: just call the read_color() funcion*/
    case READ_COLOR:
      color = read_color();
      lcd.setCursor(0, 1);                  // (column 0) of the second line (line 1):
      lcd.clear();
      lcd.print ("Cor: ");                 //print in LCD
      lcd.print (color);  
      Serial.print("Color: ");
      Serial.println(read_color() == 1 ? "red" : "blue");
      state = READ_HEIGHT;
      break;
    
    /** State read height: just call the read_height() function. Does not calculates the volume, that is done in the server */
    case READ_HEIGHT:
      altura = 10*ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
      lcd.setCursor(0, 2);                  // (column 0) of the second line (line 1):
      lcd.print ("altura: ");                 //print in LCD
      lcd.print (altura);        
      Serial.print("Altura lida (em cm):"); //Print in serial
      Serial.println(altura);
      delay(1000); 
      analogWrite(MOTOR_A, MOTOR_ON);
    state = MOVE_OUT;
      break;
    
    /** State move out: move the motor until the object reach infrared sensor 3*/
    case MOVE_OUT:
      if (analogRead(INFRA_3) < 200) {
        analogWrite(MOTOR_A, MOTOR_OFF);
        lcd.setCursor(0, 1);         // (column 0) of the second line (line 1):
        lcd.clear();
        lcd.print ("Process finished");
        sound_alert();
        httpRequest();
        Serial.println("Process finished\n");
        state = WAIT;
      }
  }
}

/*---------------------------------------------------------------------
  FUNCTION SOUND ALERT
  Double beet in a buzzer
  ---------------------------------------------------------------------*/
void sound_alert() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

/*---------------------------------------------------------------------
  FUNCTION READ COLOR
  Sensor used: TCS 230
  Return: 1=red, 2=blue, 3=green
  ---------------------------------------------------------------------*/
int read_color() {
  return 1;
}

/*---------------------------------------------------------------------
  FUNCTION HTTP REQUEST
  Method used: GET
  ---------------------------------------------------------------------*/
void httpRequest() {
  // close any connection before send a new request. This will free the socket on the WiFi shield
  client.stop();

  Serial.print("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected!");
    // Make a HTTP request:
    client.print("GET /API_data_receive.php?");
    client.print("team=");
    client.print(team);
    client.print("&instaled=");
    client.print(instaled);
    client.print("&volume=");
    client.print(altura);
    client.print("ml");
    client.print("&color=");
    client.print(color);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    Serial.println("Dados enviados para o servidor");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}












