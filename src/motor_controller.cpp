
#include "motor_controller.hpp"

#include "configuration.hpp"
#include "utils.hpp"

MotorController::MotorController(MotorDriver *left_motor,
                                 MotorDriver *right_motor,
                                 float dist_between_wheels)
    : dist_between_wheels_(dist_between_wheels),
      left_motor_(left_motor),
      right_motor_(right_motor),
      motor_update_timer_(hz_to_ms(MOTOR_RUN_FREQUENCY)) {
    pose_ = {0.0, 0.0, 0.0};
    cmd_vel_ = {0.0, 0.0};
}

void MotorController::set_cmd_vel(CmdVel cmd_vel) { cmd_vel_ = cmd_vel; }

void MotorController::get_pose(Pose &pose) { pose = pose_; }

void MotorController::reset() {
    left_motor_->reset();
    right_motor_->reset();
    pose_ = {0.0, 0.0, 0.0};
    cmd_vel_ = {0.0, 0.0};
}

void MotorController::run() {
    if (motor_update_timer_.has_elapsed()) {
        compute_pose_();
        compute_wheel_speeds_();
        left_motor_->run();
        right_motor_->run();
    }
}

void MotorController::move_forward() {
    cmd_vel_.x = 0.3;
    cmd_vel_.w = 0.0;
}
void MotorController::move_backward() {
    cmd_vel_.x = -0.3;
    cmd_vel_.w = 0.0;
}
void MotorController::turn_left() {
    cmd_vel_.x = 0.0;
    cmd_vel_.w = 0.5;
}
void MotorController::turn_right() {
    cmd_vel_.x = 0.0;
    cmd_vel_.w = -0.5;
}
void MotorController::stop() {
    cmd_vel_.x = 0.0;
    cmd_vel_.w = 0.0;
}

void MotorController::compute_wheel_speeds_() {
    float v_r = (2 * cmd_vel_.x + cmd_vel_.w * dist_between_wheels_) /
                (2 * right_motor_->get_wheel_radius());
    float v_l = (2 * cmd_vel_.x - cmd_vel_.w * dist_between_wheels_) /
                (2 * right_motor_->get_wheel_radius());

    // Downscale velocity for motor max velocity
    v_r = v_r * 0.0338;
    v_l = v_l * 0.0338;

    right_motor_->set_velocity(v_r);
    left_motor_->set_velocity(v_l);
}

void MotorController::compute_pose_() {
    MotorData left_motor_data;
    MotorData right_motor_data;
    left_motor_->get_motor_data(left_motor_data);
    right_motor_->get_motor_data(right_motor_data);
    float d_c = (left_motor_data.distance + right_motor_data.distance) / 2.0;

    pose_.x = d_c * cos(pose_.theta);
    pose_.y = d_c * sin(pose_.theta);
    pose_.theta =
        (right_motor_data.distance - left_motor_data.distance) / dist_between_wheels_;
}

void MotorController::print_pose() {
    Serial.print("Pose: ");
    Serial.print(pose_.x);
    Serial.print(", ");
    Serial.print(pose_.y);
    Serial.print(", ");
    Serial.println(pose_.theta);
}

void MotorController::move_open_loop(uint8_t left_pwm, uint8_t right_pwm) {
    left_motor_->set_pwm(left_pwm);
    right_motor_->set_pwm(right_pwm);
}

void MotorController::get_motor_status(MotorData &left_motor, MotorData &right_motor) {
    left_motor_->get_motor_data(left_motor);
    right_motor_->get_motor_data(right_motor);
}
