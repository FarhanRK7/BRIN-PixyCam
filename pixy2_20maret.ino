#include <Pixy2.h>
#include <ESP32Servo.h>

Pixy2 pixy;
Servo servoLeft, servoRight;

#define LEFT_SERVO_PIN  21
#define RIGHT_SERVO_PIN 22

#define PIXY_MISO 19
#define PIXY_MOSI 23
#define PIXY_SCK  18
#define PIXY_SS   5

const int CENTER_Y = 100;
const float SMOOTH_FACTOR = 0.4; // Lebih responsif

float targetPitch = 0;
float currentPitch = 0;

void setup() {
    Serial.begin(115200);
    pixy.init();

    servoLeft.attach(LEFT_SERVO_PIN, 500, 2400);
    servoRight.attach(RIGHT_SERVO_PIN, 500, 2400);

    servoLeft.write(90);
    servoRight.write(90);

    Serial.println("Pixy2 Pitch Control Initialized...");
}

void loop() {
    pixy.ccc.getBlocks();
    if (pixy.ccc.numBlocks) {
        int y = pixy.ccc.blocks[0].m_y;

        Serial.print("Detected Object - Y: "); Serial.println(y);

        // Hitung error terhadap pusat
        int errorY = y - CENTER_Y;

        // Hitung target pitch
        targetPitch = map(errorY, -120, 120, 50, -50);

        // Smoothing untuk menghindari gerakan kasar
        currentPitch += SMOOTH_FACTOR * (targetPitch - currentPitch);

        // Memastikan rentang gerakan servo
        int servoPos = constrain(90 - currentPitch, 20, 180);

        Serial.print("Elevon Adjusting - Servo Position: "); Serial.println(servoPos);

        servoLeft.write(servoPos);
        servoRight.write(servoPos);
    } else {
        Serial.println("No object detected");
    }
    delay(5); // Delay kecil agar lebih halus
}
