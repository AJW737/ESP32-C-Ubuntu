#include <Arduino.h>

// Stepper motor pins
#define MOTOR_IN1 15
#define MOTOR_IN2 16
#define MOTOR_IN3 17
#define MOTOR_IN4 18

// Stepper sequence (full step, 4-phase)
int stepSequence[4][4] = {
    {1, 0, 0, 1},  // Step 0
    {1, 1, 0, 0},  // Step 1
    {0, 1, 1, 0},  // Step 2
    {0, 0, 1, 1}   // Step 3
};

void motorInit() {
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
    
    Serial.println("Stepper motor initialized");
}

void setStep(int step) {
    digitalWrite(MOTOR_IN1, stepSequence[step][0]);
    digitalWrite(MOTOR_IN2, stepSequence[step][1]);
    digitalWrite(MOTOR_IN3, stepSequence[step][2]);
    digitalWrite(MOTOR_IN4, stepSequence[step][3]);
}

void rotateSteps(int steps, int stepDelay) {
    // Positive steps = clockwise, negative = counter-clockwise
    for (int i = 0; i < abs(steps); i++) {
        int step;
        if (steps > 0) {
            step = i % 4;  // Forward
        } else {
            step = 3 - (i % 4);  // Reverse
        }
        setStep(step);
        delay(stepDelay);
    }
}

void stopMotor() {
    // Turn off all coils
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("Stepper Motor Control Starting...");
    motorInit();
}

void loop() {
    Serial.println("Rotating clockwise 200 steps");
    rotateSteps(200, 5);  // 200 steps, 5ms delay between steps
    delay(1000);
    
    Serial.println("Stopping");
    stopMotor();
    delay(1000);
    
    Serial.println("Rotating counter-clockwise 200 steps");
    rotateSteps(-200, 5);
    delay(1000);
    
    Serial.println("Stopping");
    stopMotor();
    delay(1000);
}
