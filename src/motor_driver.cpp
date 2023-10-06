#include "motor_driver.hpp"

#include "configuration.hpp"
#include "utils.hpp"

MotorDriver::MotorDriver(uint8_t pin_en, uint8_t pin_in1, uint8_t pin_in2, bool reverse)
    : pin_en_(pin_en),
      pin_in1_(pin_in1),
      pin_in2_(pin_in2),
      reverse_(reverse),
      pid_(MOTOR_DRIVER_PID_KP, MOTOR_DRIVER_PID_KI, MOTOR_DRIVER_PID_KD) {
    pid_.set_output_limits(-255, 255);
    init_pins_();
    motor_mode_ = MotorMode::OPEN_LOOP;
}

MotorDriver::MotorDriver(uint8_t pin_en,
                         uint8_t pin_in1,
                         uint8_t pin_in2_,
                         Encoder *encoder,
                         float wheel_radius,
                         uint16_t ticks_per_rev,
                         bool reverse)
    : pin_en_(pin_en),
      pin_in1_(pin_in1),
      pin_in2_(pin_in2_),
      wheel_radius_(wheel_radius),
      ticks_per_rev_(ticks_per_rev),
      reverse_(reverse),
      encoder_(encoder),
      pid_(MOTOR_DRIVER_PID_KP, MOTOR_DRIVER_PID_KI, MOTOR_DRIVER_PID_KD) {
    pid_.set_output_limits(-255, 255);
    init_pins_();
    motor_mode_ = MotorMode::CLOSED_LOOP;
    encoder_->reset();
    last_encoder_reading_ = encoder_->get_ticks();
    last_data_reading_time_ = millis();
}

void MotorDriver::init_pins_(void) {
    pinMode(pin_en_, OUTPUT);
    pinMode(pin_in1_, OUTPUT);
    pinMode(pin_in2_, OUTPUT);
}

void MotorDriver::reset() {
    if (encoder_ == nullptr) {
        return;
    }
    encoder_->reset();
    last_encoder_reading_ = encoder_->get_ticks();
    last_data_reading_time_ = millis();
    set_pwm(0);
    pid_.reset();
}

void MotorDriver::set_pwm(int pwm, MotorMode mode) {
    set_mode(mode);

    bound(pwm, -255, 255);

    if (pwm < 0) {
        set_direction(MotorDirection::CCW);
        pwm_ = -pwm;
    } else if (pwm > 0) {
        set_direction(MotorDirection::CW);
        pwm_ = pwm;
    } else {
        set_direction(MotorDirection::STOP);
        pwm_ = 0;
    }
}

void MotorDriver::set_direction(MotorDirection dir) {
    byte in1 = reverse_ ^ (dir == MotorDirection::CW);
    byte in2 = reverse_ ^ (dir == MotorDirection::CCW);

    switch (dir) {
        case MotorDirection::CW:
            digitalWrite(pin_in1_, in1);
            digitalWrite(pin_in2_, in2);
            break;
        case MotorDirection::CCW:
            digitalWrite(pin_in1_, in1);
            digitalWrite(pin_in2_, in2);
            break;
        default:
            digitalWrite(pin_in1_, LOW);
            digitalWrite(pin_in2_, LOW);
            break;
    }
}

void MotorDriver::set_mode(MotorMode mode) {
    if (encoder_ == nullptr) {
        motor_mode_ = MotorMode::OPEN_LOOP;
        return;
    }
    motor_mode_ = mode;
}

void MotorDriver::set_velocity(float velocity) {
    bound(velocity, -MOTOR_MAX_VELOCITY, MOTOR_MAX_VELOCITY);
    if (encoder_ == nullptr) {
        return;
    }
    motor_mode_ = MotorMode::CLOSED_LOOP;
    pid_.set_setpoint(velocity);
}

void MotorDriver::get_motor_data(MotorData &motor_data) { motor_data = motor_data_; }

float MotorDriver::get_wheel_radius() { return wheel_radius_; }

uint16_t MotorDriver::get_ticks_per_rev() { return ticks_per_rev_; }

void MotorDriver::send_pwm() { analogWrite(pin_en_, pwm_); }

void MotorDriver::run() {
    if (motor_mode_ == MotorMode::CLOSED_LOOP) {
        compute_motor_data_();
        auto error = pid_.compute(motor_data_.velocity);
        set_pwm(error, MotorMode::CLOSED_LOOP);
    }
    send_pwm();
}

void MotorDriver::print_status(void) {
    Serial.print("w: ");
    Serial.print(motor_data_.angular_velocity);
    Serial.print(" rad/s | v: ");
    Serial.print(motor_data_.velocity);
    Serial.print(" m/s | s: ");
    Serial.print(motor_data_.distance);
    Serial.print(" m | angel: ");
    Serial.print(motor_data_.angle);
    Serial.print(" radians | pwm: ");
    Serial.println(pwm_);
}

float MotorDriver::compute_rpm_(void) {
    auto ticks = encoder_->get_ticks();
    auto now = millis();
    auto dt_time = now - last_data_reading_time_;
    auto dt_ticks = ticks - last_encoder_reading_;
    float rpm = (float(dt_ticks) / dt_time) * 60000.0 / ticks_per_rev_;
    last_encoder_reading_ = ticks;
    last_data_reading_time_ = now;
    return rpm;
}

float MotorDriver::compute_angular_velocity_(float rpm) { return rpm * 2 * PI / 60; }

float MotorDriver::compute_velocity_(float angular_velocity) {
    return angular_velocity * wheel_radius_;
}

float MotorDriver::compute_distance_(void) {
    auto ticks = encoder_->get_ticks();
    return ticks * 2 * PI * wheel_radius_ / ticks_per_rev_;
}

void MotorDriver::compute_motor_data_(void) {
    motor_data_.rpm = compute_rpm_();
    motor_data_.angular_velocity = compute_angular_velocity_(motor_data_.rpm);
    motor_data_.velocity = compute_velocity_(motor_data_.angular_velocity);
    motor_data_.distance = compute_distance_();
}
