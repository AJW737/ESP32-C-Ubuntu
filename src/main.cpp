#include <Arduino.h>

// Stepper motor pins
#define MOTOR_IN1 19
#define MOTOR_IN2 20
#define MOTOR_IN3 21
#define MOTOR_IN4 47

// Stepper sequence (full step, 4-phase)
int stepSequence[4][4] = {
    {1, 0, 0, 1},  // Step 0
    {1, 1, 0, 0},  // Step 1
    {0, 1, 1, 0},  // Step 2
    {0, 0, 1, 1}   // Step 3
};

int currentStep = 0;  // Track current step position

void motorInit() {
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
    
    Serial.println("Stepper motor initialized");
    Serial.println("Use arrow keys to control:");
    Serial.println("  UP/RIGHT arrow = Clockwise");
    Serial.println("  DOWN/LEFT arrow = Counter-clockwise");
    Serial.println("  SPACE = Stop motor");
}

void setStep(int step) {
    digitalWrite(MOTOR_IN1, stepSequence[step][0]);
    digitalWrite(MOTOR_IN2, stepSequence[step][1]);
    digitalWrite(MOTOR_IN3, stepSequence[step][2]);
    digitalWrite(MOTOR_IN4, stepSequence[step][3]);
}

void stepClockwise() {
    currentStep = (currentStep + 1) % 4;
    setStep(currentStep);
}

void stepCounterClockwise() {
    currentStep = (currentStep - 1 + 4) % 4;
    setStep(currentStep);
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
    Serial.println("Stepper Motor Control - Keyboard Control");
    motorInit();
}

void loop() {
    if (Serial.available() > 0) {
        int incomingByte = Serial.read();
        
        // Handle arrow key sequences
        // Arrow keys send escape sequences: ESC [ A/B/C/D
        if (incomingByte == 27) {  // ESC character
            delay(10);  // Small delay to receive the full sequence
            if (Serial.available() > 0) {
                int next = Serial.read();
                if (next == 91) {  // '[' character
                    if (Serial.available() > 0) {
                        int arrow = Serial.read();
                        
                        switch (arrow) {
                            case 65:  // Up arrow
                            case 67:  // Right arrow
                                stepClockwise();
                                Serial.println("→ Clockwise");
                                break;
                                
                            case 66:  // Down arrow
                            case 68:  // Left arrow
                                stepCounterClockwise();
                                Serial.println("← Counter-clockwise");
                                break;
                        }
                    }
                }
            }
        }
        // Alternative: use regular keys if arrow keys don't work
        else if (incomingByte == 'w' || incomingByte == 'd') {
            stepClockwise();
            Serial.println("→ Clockwise");
        }
        else if (incomingByte == 's' || incomingByte == 'a') {
            stepCounterClockwise();
            Serial.println("← Counter-clockwise");
        }
        else if (incomingByte == ' ') {  // Space bar
            stopMotor();
            Serial.println("■ Motor stopped");
        }
    }
    
    delay(5);  // Small delay for smooth operation
}
