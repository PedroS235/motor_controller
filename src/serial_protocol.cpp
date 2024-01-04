#include "serial_protocol.hpp"

SerialProtocol::SerialProtocol(MotorController* motorCtrl) {
    motorController_ = motorCtrl;
}

int SerialProtocol::parse_cmd_(const String& cmd) {
    char flag = cmd.charAt(0);

    switch (flag) {
        case FLAG_CLOSE:
            int x, w;
            if (sscanf(cmd.c_str(), "c %d %d", &x, &w) == 2) {
                CmdVel cmd_vel;
                cmd_vel.x = x / 1000.0;
                cmd_vel.w = w / 1000.0;
                motorController_->set_cmd_vel(cmd_vel);
                return 0;  // Success
            }
            break;

        case FLAG_OPEN:
            unsigned int left_pwm_val, right_pwm_val;
            if (sscanf(cmd.c_str(), "o %u %u", &left_pwm_val, &right_pwm_val) == 2) {
                if (left_pwm_val <= 255 && right_pwm_val <= 255) {
                    motorController_->move_open_loop(left_pwm_val, right_pwm_val);
                    return 0;  // Success
                } else {
                    return -2;  // Error: PWM values out of range
                }
            }
            break;

        case FLAG_POSE:
            Pose pose;
            motorController_->get_pose(pose);
            Serial.println(String(pose.x) + " " + String(pose.y) + " " +
                           String(pose.theta));
            return 1;  // Success and returned pose
            break;

        case FLAG_MOTOR_STATUS:
            MotorData left_motor;
            MotorData right_motor;
            motorController_->get_motor_status(left_motor, right_motor);
            Serial.print(left_motor.rpm);
            Serial.print(" ");
            Serial.print(left_motor.velocity);
            Serial.print(" ");
            Serial.print(left_motor.angular_velocity);
            Serial.print(" ");
            Serial.print(left_motor.distance);
            Serial.print(" ");
            Serial.print(left_motor.angle);
            Serial.print(",");
            Serial.print(right_motor.rpm);
            Serial.print(" ");
            Serial.print(right_motor.velocity);
            Serial.print(" ");
            Serial.print(right_motor.angular_velocity);
            Serial.print(" ");
            Serial.print(right_motor.distance);
            Serial.print(" ");
            Serial.print(right_motor.angle);

            return 1;  // Success and returned pose
            break;
        case FLAG_RESET:
            motorController_->reset_pose();
            return 0;  // Success
            break;

        case FLAG_PID_GAINS: {
            int kp = 0, ki = 0, kd = 0;
            if (sscanf(cmd.c_str(), "p %d %d %d", &kp, &ki, &kd) == 3) {
                pid_gains_t pid_gains;
                pid_gains.kp = kp;
                pid_gains.ki = ki;
                pid_gains.kd = kd;
                motorController_->update_motor_pids(pid_gains);
                return 0;  // Success
            } else {
                return -1;  // Error: Invalid command
            }
            break;
        }

        case FLAG_PID_GET: {
            auto pid_gains = motorController_->get_motor_pids();
            Serial.print(pid_gains.kp);
            Serial.print(" ");
            Serial.print(pid_gains.ki);
            Serial.print(" ");
            Serial.print(pid_gains.kd);
            return 1;  // Success and return pid gains
            break;
        }

        default:
            return -1;  // Error: Invalid command
            break;
    }
    return -1;
}

void SerialProtocol::read_serial() {
    while (Serial.available()) {
        char c = Serial.read();
        static String inputBuffer = "";

        if (c == '\n') {
            int ret = parse_cmd_(inputBuffer);
            send_ack(ret);
            inputBuffer = "";
        } else {
            inputBuffer += c;
        }
    }
}

void SerialProtocol::send_ack(int code) {
    switch (code) {
        case 0:
            Serial.println("OK");
            break;
        case 1:
            break;
        case -1:
            Serial.println("ERR: Invalid command");
            break;
        case -2:
            Serial.println("ERR: PWM values out of range");
            break;
        default:
            Serial.println("ERR: Unknown error");
            break;
    }
}
