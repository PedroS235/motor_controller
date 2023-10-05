#ifndef MOTOR_CONTROLLER_HPP
#define MOTOR_CONTROLLER_HPP

#include <Arduino.h>

#include "configuration.hpp"
#include "motor_driver.hpp"
#include "timer.hpp"

typedef struct {
    float x;
    float y;
    float w;
} Pose;

typedef struct {
    float x;
    float w;
} CmdVel;

class MotorController {
   public:
    MotorController(MotorDriver *left_motor,
                    MotorDriver *right_motor,
                    float dist_between_wheels);
    void get_pose(Pose *odometry);
    void set_cmd_vel(CmdVel cmd_vel);
    void run();
    void print_pose();

   private:
    void compute_pose_();
    void compute_wheel_speeds_();

   private:
    Pose pose_;
    CmdVel cmd_vel_;
    float dist_between_wheels_;

   private:
    MotorDriver *left_motor_;
    MotorDriver *right_motor_;
    Timer motor_update_timer_;
};

#endif  // MOTOR_CONTROLLER_HPP
