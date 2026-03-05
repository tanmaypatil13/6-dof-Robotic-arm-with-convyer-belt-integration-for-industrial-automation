:
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <WebServer.h>
// Create an instance of the PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_MIN 150 // Min pulse length
#define SERVO_MAX 600 // Max pulse length
// Servo channel definitions
#define BASE_SERVO 0
#define SHOULDER_SERVO 1
#define ELBOW_SERVO 2
#define WRIST_SERVO 3
#define GRIPPERroll_SERVO 4
#define GRIPPER_SERVO 5
//functions to call oeration of ARM
void inital_position();
void pick_pos_move();
void to_color_recognization();
void close_gripper();
void open_gripper();
void sort_red();
void sort_blue();
void sort_green();
void sort_unknown();
void inter_move();
int color_recognisition();
//tcs3200 colour sensor pin config
#define S0 26
#define S1 25
#define S2 33
#define S3 32
#define OUT 27
#define OE 13
// Conveyor Belt Control with Motor and Relay using ESP32
#define MOTOR_PIN 5 // Relay connected to motor driver
#define ENTRY_IR 18 // Entry IR sensor pin
#define EXIT_IR 19 // Exit IR sensor pin
// WiFi credentials
const char* ssid = "Tanmay@patil";
const char* password = "00000000";
WebServer server(80); // Web server instance
//colour recognization inital logic
int redFreq, greenFreq, blueFreq;
int objectCount = 0; // Object counter
int getFrequency(int s2Val, int s3Val) {
digitalWrite(S2, s2Val);
digitalWrite(S3, s3Val);
delay(100); // short delay for settling
long sum = 0;
for (int i = 0; i < 10; i++) {
sum += pulseIn(OUT, LOW);
}
return sum / 10; // average of 10 samples
};
void setup() {
// Pin configuration of TCS3200
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(OUT, INPUT);
// Set TCS3200 scaling to 20% (recommended)
digitalWrite(S0, HIGH);
digitalWrite(S1, LOW);
Serial.begin(115200);
// Initialize the PCA9685
pwm.begin();
pwm.setPWMFreq(50); // Set frequency to 50 Hz
// pinmode config for convyer drive
pinMode(MOTOR_PIN, OUTPUT);
pinMode(ENTRY_IR, INPUT);
pinMode(EXIT_IR, INPUT);
digitalWrite(MOTOR_PIN, LOW); // Motor off initially
// WiFi connection
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println(" Connected!");
Serial.println(WiFi.localIP());
// Web server route
server.on("/", []() {
String html = "<html><body><h1>Object Counter</h1><p>Total Objects Sorted: " +
String(objectCount) + "</p></body></html>";
server.send(200, "text/html", html);
});
server.begin();
// Move servos to initial positions
inital_position();
}
void loop() {
server.handleClient();
// Handle web client
//variables to read colors
redFreq = getFrequency(LOW, LOW); // Red
greenFreq = getFrequency(HIGH, HIGH); // Green
blueFreq = getFrequency(LOW, HIGH); // Blue
bool object_detected= false;
bool entryDetected = digitalRead(ENTRY_IR) == LOW; // IR sensor active LOW
bool exitDetected = digitalRead(EXIT_IR) == LOW;
if (entryDetected && !object_detected)
{
object_detected=true;
digitalWrite(MOTOR_PIN, HIGH); // Turn on conveyor
}
if (exitDetected && object_detected)
{
object_detected=false;
digitalWrite(MOTOR_PIN, LOW); // Turn off conveyor
}
pick_pos_move();
delay(1000);
to_color_recognization();
delay(2000);
digitalWrite(OE, LOW); //enable sensor LOW=enable,HIGH=disable
int color_val = color_recognisition();
switch (color_val) {
case 1:
Serial.println("red obj");
sort_red();
break;
case 2:
Serial.println("blue obj");
sort_blue();
break;
case 3:
Serial.println("green obj");
sort_green();
break;
default:
Serial.println("unknown obj");
sort_unknown();
break;
}
open_gripper(); // release object
inter_move(); // return to initial position
objectCount++; // increment counter
delay(100); // Small delay for stability
}
void moveServo(uint8_t servoNum, uint16_t angle) {
uint16_t pulseLength = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
pwm.setPWM(servoNum, 0, pulseLength);
}
int color_recognisition() {
// Basic color detection using thresholds
if (redFreq < greenFreq * 0.8 && redFreq < blueFreq * 0.8) {
Serial.println("Detected Color: RED");
return 1;
} else if (greenFreq < redFreq && greenFreq < blueFreq) {
Serial.println("Detected Color: GREEN");
return 3;
} else if (blueFreq < redFreq * 0.8 && blueFreq < greenFreq * 0.8) {
Serial.println("Detected Color: BLUE");
return 2;
} else {
Serial.println("Detected Color: Black/white");
return 0;
}
}
void inital_position() {
// Move servos to initial positions
moveServo(BASE_SERVO, 0);
moveServo(SHOULDER_SERVO, 120);
moveServo(ELBOW_SERVO, 150);
moveServo(GRIPPERroll_SERVO, 100);
moveServo(WRIST_SERVO, 0);
moveServo(GRIPPER_SERVO, 80); // gripper open inital position
delay(3000);
}
void pick_pos_move() {
// Move to pick positionn
moveServo(BASE_SERVO, 0); // Rotate base
moveServo(SHOULDER_SERVO, 50); // Move shoulder down
moveServo(ELBOW_SERVO, 180); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 90); // roll gripper Wrist
inter_move();
delay(3000);
}
void to_color_recognization() {
// Move to colour recognization position
inter_move(); //intermediate
moveServo(BASE_SERVO, 90); // Rotate base
moveServo(SHOULDER_SERVO, 50); // Move shoulder down
moveServo(ELBOW_SERVO, 170); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 100); // roll gripper Wristusx
inter_move();
delay(3000);
}
void close_gripper() {
// Close gripper to pick
moveServo(GRIPPER_SERVO, 35);
delay(2000);
}
void open_gripper() {
// Release object
moveServo(GRIPPER_SERVO, 80);
delay(2000); //open gripper to release object
}
void sort_red() {
// sort to red coloured object
moveServo(BASE_SERVO, 120); // Rotate base
moveServo(SHOULDER_SERVO, 50); // Move shoulder down
moveServo(ELBOW_SERVO, 180); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 110); // roll gripper Wrist
delay(900);
}
void sort_blue() {
// sort to blue coloured object
moveServo(BASE_SERVO, 120); // Rotate base
moveServo(SHOULDER_SERVO, 20); // Move shoulder down
moveServo(ELBOW_SERVO, 60); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 50); // roll gripper Wrist
inter_move();
delay(3000);
}
void sort_green() {
// sort to green coloured object
// sort to green coloured object
moveServo(BASE_SERVO, 145); // Rotate base
moveServo(SHOULDER_SERVO, 50); // Move shoulder down
moveServo(ELBOW_SERVO, 180); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 110); // roll gripper Wrist
delay(900);;
}
void sort_unknown() {
moveServo(BASE_SERVO, 160); // Rotate base
moveServo(SHOULDER_SERVO, 50); // Move shoulder down
moveServo(ELBOW_SERVO, 180); // Adjust elbow
moveServo(WRIST_SERVO, 0); // Adjust wrist
moveServo(GRIPPERroll_SERVO, 110); // roll gripper Wrist
delay(900);
}
void inter_move()
{
//intermediate
moveServo(BASE_SERVO, 0);
moveServo(SHOULDER_SERVO, 120);
moveServo(ELBOW_SERVO, 160);
moveServo(WRIST_SERVO, 0);
moveServo(GRIPPERroll_SERVO, 110 );
delay(1000);
}
