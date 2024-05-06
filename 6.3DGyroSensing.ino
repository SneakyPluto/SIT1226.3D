#include <MeAuriga.h>
#include <Wire.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

int16_t moveSpeed = 50;

MeGyro gyro(1, 0x69);

float j, f, k;

// Threshold to determine light source direction
const int threshold = 6;  // Adjust as needed based on your environment

void Forward(void) {
  Encoder_1.setMotorPwm(-moveSpeed);  // setMotorPwm writes to the encoder controller
  Encoder_2.setMotorPwm(moveSpeed);   // so setting the speed change instantly
}
void Backward(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void BackwardAndTurnLeft(void) {
  Encoder_1.setMotorPwm(moveSpeed / 4);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void BackwardAndTurnRight(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed / 4);
}

void TurnLeft(void) {
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed / 2);
}

void TurnRight() {
  Encoder_1.setMotorPwm(-moveSpeed / 2);
  Encoder_2.setMotorPwm(moveSpeed);
}

void TurnLeft1(void) {
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

void TurnRight1(void) {
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed);
}

void Stop(void) {
  Encoder_1.setMotorPwm(0);
  Encoder_2.setMotorPwm(0);
}

void ChangeSpeed(int16_t spd) {
  moveSpeed = spd;
}

void setup() {
  Serial.begin(115200);
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  gyro.begin();

}

void loop() {
  gyro.update();
  Serial.println(gyro.getAngleZ());

  double gyroVal = gyro.getAngleZ();
  double rotationThreshold = 10.0;

  if (gyroVal >= rotationThreshold) {
    findZeroAnticlockwise();
  } else if (gyroVal <= -rotationThreshold) {
    findZeroClockwise();
  }
}

void findZeroClockwise() {
  gyro.update();
  double degree = gyro.getAngleZ();
  TurnRight1();
  while (degree < 0) {
    gyro.update();
    degree = gyro.getAngleZ();
  }
  Stop();
}

void findZeroAnticlockwise() {
  gyro.update();
  double degree = gyro.getAngleZ();
  TurnLeft1();
  while (degree > 0) {
    gyro.update();
    degree = gyro.getAngleZ();
  }
  Stop();
}

void turnRight(double degree) {
  gyro.update();
  Serial.println(gyro.getAngleZ());
  double startAngle = gyro.getAngleZ() + 180.00;
  double delta;
  TurnRight1();
  while (degree > 0) {
    gyro.update();
    delta = gyro.getAngleZ() + 180.00 - startAngle;
    if (delta < -180.00) {
      delta += 360.00;
    } else if (delta > 180) {
      delta -= 360;
    }
    degree -= delta;
    startAngle = gyro.getAngleZ() + 180.00;
  }
  gyro.update();
  Serial.println(gyro.getAngleZ());
  Stop();
}

void turnLeft(double degree) {
  gyro.update();
  Serial.println(gyro.getAngleZ());
  double startAngle = gyro.getAngleZ() + 180.00;
  double delta;
  TurnLeft1();
  while (degree > 0) {
    gyro.update();
    delta = gyro.getAngleZ() + 180.00 - startAngle;
    if (delta < -180.00) {
      delta += 360.00;
    } else if (delta > 180) {
      delta -= 360;
    }
    degree += delta;
    startAngle = gyro.getAngleZ() + 180.00;
  }
  gyro.update();
  Serial.println(gyro.getAngleZ());
  Stop();
}
