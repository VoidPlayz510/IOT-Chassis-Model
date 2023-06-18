#include <Servo.h>

Servo servo;

int angle = 50;

//Motor Pins
#define Front_EN_A 5   //Enable pin for first motor
#define Front_IN1 6    //control pin for first motor
#define Front_IN2 7    //control pin for first motor
#define Front_IN3 8    //control pin for second motor
#define Front_IN4 9    //control pin for second motor
#define Front_EN_B 10  //Enable pin for second motor

#define Back_EN_A 3   //Enable pin for first motor
#define Back_IN1 2    //control pin for first motor
#define Back_IN2 4    //control pin for first motor
#define Back_IN3 12   //control pin for second motor
#define Back_IN4 13   //control pin for second motor
#define Back_EN_B 11  //Enable pin for second motor

#define echoPin A0  // Echo pin of the ultrasonic sensor
#define trigPin A1  // Trigger pin of the ultrasonic sensor

#define Forward A3  //
#define Obs A4      //
#define Stop A5     //

//Initializing variables to store data
int motor_speed_FL = 0;

int motor_speed_FR = 0;
int motor_speed_BL = 0;
int motor_speed_BR = 0;

long duration, distance;
unsigned int detectionLim = 7000;
unsigned long currentTime, elapsedTime, previousTime;
const unsigned long delayTime = 1000;
float starttime = millis();
float endtime = starttime;

void setup() {
  Serial.begin(9600);  // Initialize the Serial communication

  servo.attach(A2);
  servo.write(angle);

  pinMode(Front_EN_A, OUTPUT);
  pinMode(Front_IN1, OUTPUT);
  pinMode(Front_IN2, OUTPUT);
  pinMode(Front_IN3, OUTPUT);
  pinMode(Front_IN4, OUTPUT);
  pinMode(Front_EN_B, OUTPUT);

  pinMode(Back_EN_A, OUTPUT);
  pinMode(Back_IN1, OUTPUT);
  pinMode(Back_IN2, OUTPUT);
  pinMode(Back_IN3, OUTPUT);
  pinMode(Back_IN4, OUTPUT);
  pinMode(Back_EN_B, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(Forward, OUTPUT);
  pinMode(Obs, OUTPUT);
  pinMode(Stop, OUTPUT);

  digitalWrite(Forward, LOW);
  digitalWrite(Obs, LOW);
  digitalWrite(Stop, LOW);

  digitalWrite(Front_IN1, HIGH);
  digitalWrite(Front_IN2, LOW);
  digitalWrite(Front_IN3, HIGH);
  digitalWrite(Front_IN4, LOW);
  digitalWrite(Back_IN1, HIGH);
  digitalWrite(Back_IN2, LOW);
  digitalWrite(Back_IN3, HIGH);
  digitalWrite(Back_IN4, LOW);

  analogWrite(Front_EN_A, motor_speed_FL);
  analogWrite(Front_EN_B, motor_speed_FR);
  analogWrite(Back_EN_A, motor_speed_BL);
  analogWrite(Back_EN_B, motor_speed_BR);

  delay(500);

  analogWrite(Front_EN_A, 255);
  analogWrite(Front_EN_B, 255);
  analogWrite(Back_EN_A, 255);
  analogWrite(Back_EN_B, 255);
  delay(5000);
}

void loop() {
  currentTime = millis();


  // Check if the desired delay has passed
  if (currentTime - previousTime >= delayTime) {
    // Display the elapsed time in seconds
    elapsedTime = (currentTime - previousTime) / 1000;

        if (elapsedTime >= 7) {
      // Stop the program
    }
    // Send a trigger pulse to the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the duration of the pulse from the echo pin
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in centimeters
    distance = duration * 0.034 / 2;

    // Print the distance to the Serial monitor
    //Serial.print("Distance: ");
    //Serial.print(distance);
    //Serial.println(" cm");

    if (distance <= 15) {
      digitalWrite(Forward, LOW);
      digitalWrite(Obs, HIGH);

      analogWrite(Front_EN_A, 0);
      analogWrite(Front_EN_B, 0);
      analogWrite(Back_EN_A, 0);
      analogWrite(Back_EN_B, 0);
      delay(500);
      digitalWrite(Front_IN1, LOW);
      digitalWrite(Front_IN2, HIGH);
      digitalWrite(Front_IN3, LOW);
      digitalWrite(Front_IN4, HIGH);
      digitalWrite(Back_IN1, LOW);
      digitalWrite(Back_IN2, HIGH);
      digitalWrite(Back_IN3, LOW);
      digitalWrite(Back_IN4, HIGH);

      analogWrite(Front_EN_A, 200);
      analogWrite(Front_EN_B, 200);
      analogWrite(Back_EN_A, 200);
      analogWrite(Back_EN_B, 200);


      //digitalWrite(Reverse, HIGH);
      delay(1000);
      //digitalWrite(Reverse, LOW);


      analogWrite(Front_EN_A, 0);
      analogWrite(Front_EN_B, 0);
      analogWrite(Back_EN_A, 0);
      analogWrite(Back_EN_B, 0);

      delay(500);

      digitalWrite(Front_IN1, HIGH);
      digitalWrite(Front_IN2, LOW);
      digitalWrite(Front_IN3, HIGH);
      digitalWrite(Front_IN4, LOW);
      digitalWrite(Back_IN1, HIGH);
      digitalWrite(Back_IN2, LOW);
      digitalWrite(Back_IN3, HIGH);
      digitalWrite(Back_IN4, LOW);

      analogWrite(Front_EN_A, 255);
      analogWrite(Front_EN_B, 255);
      analogWrite(Back_EN_A, 255);
      analogWrite(Back_EN_B, 255);

      servo.write(30);
      delay(1500);
      servo.write(70);
      delay(1750);
      servo.write(30);
      delay(1500);
      servo.write(50);

      digitalWrite(Obs, LOW);
    }
    if (distance >= 16 && distance <= 25) {
      analogWrite(Front_EN_A, 90);
      analogWrite(Front_EN_B, 90);
      analogWrite(Back_EN_A, 90);
      analogWrite(Back_EN_B, 90);

      digitalWrite(Forward, HIGH);
    }
    if (distance >= 26 && distance <= 30) {
      analogWrite(Front_EN_A, 130);
      analogWrite(Front_EN_B, 130);
      analogWrite(Back_EN_A, 130);
      analogWrite(Back_EN_B, 130);

      digitalWrite(Forward, HIGH);
    }
    if (distance > 31) {
      analogWrite(Front_EN_A, 255);
      analogWrite(Front_EN_B, 255);
      analogWrite(Back_EN_A, 255);
      analogWrite(Back_EN_B, 255);

      digitalWrite(Forward, HIGH);
    }
    previousTime = currentTime;
    Serial.println(distance);
  }
}

void steering() {
  // scan from 0 to 180 degrees
  for (angle = 30; angle < 70; angle++) {
    Serial.print("angle: ");
    Serial.println(angle);
    servo.write(angle);
    delay(15);
  }
  // now scan back from 180 to 0 degrees
  delay(1000);
  for (angle = 70; angle > 30; angle--) {
    Serial.print("angle: ");
    Serial.println(angle);
    servo.write(angle);
    delay(15);
  }
  delay(1000);
}
