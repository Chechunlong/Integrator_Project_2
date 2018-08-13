#include <Servo.h>

//State definitions
#define WAIT        0
#define MOVE_IN     1
#define READ_COLOR  2
#define READ_HEIGHT 3
#define MOVE_OUT    4

//Pin definitions
#define INFRA_1 2
#define INFRA_2 3
#define INFRA_3 4
#define BUZZER  5
#define POT     A0
//Global variables 
Servo myservo;  // Create servo object to control a servo
char state = 0; // To the state machine

/*---------------------------------------------------------------------
  FUNCTION MAIN
---------------------------------------------------------------------*/
void setup() {
  myservo.attach(9);
  pinMode(INFRA_1, INPUT);
  pinMode(INFRA_2, INPUT);
  pinMode(INFRA_3, INPUT);  
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  Serial.println("Welcome to PI-2!!!");
  Serial.println("waiting...");
}

void loop() {
  switch (state) {
    /* State Wait: will do nothing until the infrared sensor 1 (positioned in the begin of the conveyor) detect the object*/
    case WAIT:
      if (digitalRead(INFRA_1) == 1) {
        Serial.println("Object detected");
        sound_alert();
        state = MOVE_IN;
      }
      break;
    /* State Move In: just accionates the DC motor until the object reach the infrared sensor 2*/
    case MOVE_IN:
      move_motor();
      if (digitalRead(INFRA_2) == 1) {
        Serial.println("Reading color...");
        state = READ_COLOR;
      }
      break;
    /* State Read Color: just call the read_color() funcion*/
    case READ_COLOR:
      Serial.print("Color: ");
      Serial.println(read_color()==1 ? "red" : "blue");
      state = READ_HEIGHT;
      break;
    /* State read height: just call the read_height() function. Does not calculates the volume, that is done in the server */
    case READ_HEIGHT:
      Serial.print("Object height: ");
      Serial.println(read_height());
      state = MOVE_OUT;
      delay(2000); //just for debug...
      break;
    /* State move out: will move the motor until the object reach infrared sensor 3*/
    case MOVE_OUT:
      move_motor();
      if (digitalRead(INFRA_3) == 1) {
        Serial.println("Process finished\n");
        sound_alert();
        state = WAIT;
      }
  }
}

/*---------------------------------------------------------------------
  FUNCTION MOVE MOTOR
  Now is being done with a servo, but will move a DC motor
---------------------------------------------------------------------*/
void move_motor() {
  static int pos = 0;    // variable to store the servo position
  static int i = 1;
  if (pos >= 180) i = -1; else if (pos <= 0)i = 1;
  myservo.write(pos += i);
  delay (15);
}

/*---------------------------------------------------------------------
  FUNCTION SOUND ALERT
  Double beet in a buzzer
---------------------------------------------------------------------*/
void sound_alert(){
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
  FUNCTION READ HEIGHT
  Sensor used: HC-SR04
---------------------------------------------------------------------*/
int read_height() {
  return analogRead(POT);
}

