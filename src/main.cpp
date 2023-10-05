#include <Arduino.h>

#include "configuration.hpp"
#include "motor_controller.hpp"
#include "motor_driver.hpp"
#include "timer.hpp"

Encoder left_motor_encoder(GPIO_MOTOR_LEFT_ENCODER_A, GPIO_MOTOR_LEFT_ENCODER_B, true);
Encoder right_motor_encoder(GPIO_MOTOR_RIGHT_ENCODER_A, GPIO_MOTOR_RIGHT_ENCODER_B);
MotorDriver left_motor(GPIO_MOTOR_LEFT_EN,
                       GPIO_MOTOR_LEFT_IN1,
                       GPIO_MOTOR_LEFT_IN2,
                       &left_motor_encoder,
                       WHEEL_RADIUS,
                       ENCODER_TICKS_PER_REVOLUTION,
                       true);
MotorDriver right_motor(GPIO_MOTOR_RIGHT_EN,
                        GPIO_MOTOR_RIGHT_IN1,
                        GPIO_MOTOR_RIGHT_IN2,
                        &right_motor_encoder,
                        WHEEL_RADIUS,
                        ENCODER_TICKS_PER_REVOLUTION);

MotorController motor_controller(&left_motor, &right_motor, DIST_BETWEEN_WHEELS);

void left_motor_encoder_ISR(void) { left_motor_encoder.tick_isr(); };
void right_motor_encoder_ISR(void) { right_motor_encoder.tick_isr(); };

void setup_interrupts(void) {
    attachInterrupt(digitalPinToInterrupt(GPIO_MOTOR_LEFT_ENCODER_A),
                    left_motor_encoder_ISR,
                    RISING);
    attachInterrupt(digitalPinToInterrupt(GPIO_MOTOR_RIGHT_ENCODER_A),
                    right_motor_encoder_ISR,
                    RISING);
}

CmdVel cmd_vel = {0.3, 1.0};

/**
 * The setup function is called once at startup of the sketch
 */
void setup(void) {
    Serial.begin(9600);
    Serial.println("Start");
    setup_interrupts();
    motor_controller.set_cmd_vel(cmd_vel);
}

/**
 * The loop function is called in an endless loop
 */
void loop(void) {
    motor_controller.run();
    motor_controller.print_pose();
}
