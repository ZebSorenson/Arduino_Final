#include <Servo.h> 
  
#include "Arduino_SensorKit.h"

#define Environment Environment_I2C

Servo myServo; //This is the servo for the motor on the feeder 

Servo DoorServo; //Servo for the door



const int motorPin = 10; // Pin connected to the motor

const int fanPin = 7;

const int tempSensorPin = A0; 

const int doorPin = 6;

const int lightPin = 8;

const int buzzerPin = 12;

const int lightSensorPin = A3;
 
boolean buzzerHasBeeped = false; //this will help us keep track of whether or not the buzzer has already beeped...helping to keep the cycle in order 

boolean feederHasStarted = false; //this helps us keep track of whether or not the feeder has already started...Helping to keep the cycle in order

unsigned long servoStartTime = 0; //this will be used as a timer

boolean continueRunning = true; // this is used to keep us in a null state that helps us present the photoresistor and door servo after the cycle has ran 


void setup() {

  //get everything set up and ready to go! Set the pins
  Wire.begin();

  pinMode(lightSensorPin, INPUT);

  pinMode(lightPin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  myServo.write(0);

  DoorServo.attach(doorPin);
  
  pinMode(doorPin, OUTPUT);

  myServo.attach(10);
  
  pinMode(fanPin, OUTPUT);
  
  Serial.begin(9600);
  
  Environment.begin();
  
 
}

void loop() { //the master loop



int lightValue = analogRead(lightSensorPin); // this is what we used to read the values of the heat sensor and help us determine whether or not to turn on the fan


if(lightValue<250){
  //TURN ON THE LIGHT IF IT IS NIGHT TIME AKA, the photoresistor is covered
  digitalWrite(lightPin, HIGH);

  //Buzz so the dog is called back in give a delay so he has time to get back
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);

  //CLOSE THE DOOR
   delay(5000);
   DoorServo.write(0);
   delay(1000);


}else{
  //TURN off THE LIGHT BECAUSE IT IS DAYTIME 
  digitalWrite(lightPin, LOW);
  //OPEN THE DOOR SO THE DOGGY CAN GO OUT 

  if (!buzzerHasBeeped) { //this will be the first beep in the cycle, so we will keep the door closed so the dog has to finish his food
      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
      buzzerHasBeeped = true;
      delay(500);
      
    }

    if (servoStartTime == 0) {
      servoStartTime = millis(); // Start the timer
      myServo.write(160); // Start the servo motor for the feeder
    }

    // Check if 15 seconds have elapsed in order to stop the ffeder
    if (millis() - servoStartTime >= 15000 && continueRunning == true) {
      myServo.write(0); // Stop the servo motor
      delay(5000);
      digitalWrite(buzzerPin,HIGH);
      delay(2000);
      digitalWrite(buzzerPin, LOW);
      delay(3000);
      DoorServo.write(0);
      delay(15000);
       continueRunning = false; 
      
    }

   //this is the kind of null state where the door opens and closes based on the photo resistor...This is more for demonstration purposes.
  delay(500);
  DoorServo.write(90);
  delay(500);
  
}

  //you can uncomment this code out to check the values the photoresistor values are printing 
 
  // Serial.print("Light Value: ");
  // Serial.print(lightValue);
  // Serial.println(" ");
   
   
   
   

  int sensorValue = analogRead(tempSensorPin);

  //Here you can print the heat values to make sure it's working...just uncomment out the code
  
  
  // Print the temperature for debugging
  // Serial.print("Temperature: ");
  // Serial.print(sensorValue);
  // Serial.println(" ");

  
  // If the temperature exceeds the threshold, turn on the fan; otherwise, turn it off
  if (sensorValue >=290) {
    digitalWrite(fanPin, HIGH); // Turn on the fan
  } else {
    digitalWrite(fanPin, LOW); // Turn off the fan
  }

  delay(1000);
  

}