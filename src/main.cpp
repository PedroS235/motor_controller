#include <Arduino.h>

#include "configuration.hpp"
#include "motor_driver.hpp"
#include "timer.hpp"

Encoder encoder(GPIO_MOTOR_LEFT_ENCODER_A, GPIO_MOTOR_LEFT_ENCODER_B, true);
MotorDriver motor(GPIO_MOTOR_LEFT_EN,
                  GPIO_MOTOR_LEFT_IN1,
                  GPIO_MOTOR_LEFT_IN2,
                  &encoder,
                  WHEEL_RADIUS,
                  ENCODER_TICKS_PER_REVOLUTION,
                  true);
Timer timer(50);

void encoder_isr(void) { encoder.tick_isr(); }

/**
 * The setup function is called once at startup of the sketch
 */
void setup(void) {
    Serial.begin(9600);
    Serial.println("Start");
    attachInterrupt(
        digitalPinToInterrupt(GPIO_MOTOR_LEFT_ENCODER_A), encoder_isr, RISING);
    motor.set_velocity(0.5);
}

/**
 * The loop function is called in an endless loop
 */
void loop(void) {
    if (timer.has_elapsed()) {
        motor.run();
        MotorData data;
        motor.get_motor_data(data);
        Serial.print("Setpoint:");
        Serial.print(0.5);
        Serial.print(",");
        Serial.print("velocity:");
        Serial.println(data.velocity);
        motor.print_status();
    }
}
