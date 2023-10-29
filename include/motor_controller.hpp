#ifndef MOTOR_CONTROLLER_HPP
#define MOTOR_CONTROLLER_HPP

#include <Arduino.h>

#include "configuration.hpp"
#include "motor_driver.hpp"
#include "timer.hpp"

// TODO: Add methods to update motor PID values.

/**
 * @struct Pose
 * @brief Represents the pose of a differential drive robot with x, y, and theta
 * components.
 */
typedef struct {
    float x;      ///< The x-coordinate of the robot's position.
    float y;      ///< The y-coordinate of the robot's position.
    float theta;  ///< The orientation angle (theta) of the robot.
} Pose;

/**
 * @struct CmdVel
 * @brief Represents the commanded velocity for a differential drive robot with x and w
 * components.
 */
typedef struct {
    float x;  ///< Linear velocity along the x-axis.
    float w;  ///< Angular velocity around the z-axis.
} CmdVel;

/**
 * @class MotorController
 * @brief A class for controlling two motors and computing the robot's pose.
 */
class MotorController {
   public:
    /**
     * @brief Constructor for the MotorController class.
     *
     * @param left_motor A pointer to the left motor driver.
     * @param right_motor A pointer to the right motor driver.
     * @param dist_between_wheels The distance between the robot's two wheels.
     */
    MotorController(MotorDriver *left_motor,
                    MotorDriver *right_motor,
                    float dist_between_wheels);

    /**
     * @brief Get the current pose of the robot.
     *
     * @param pose Pointer to a Pose structure to store the current pose.
     */
    void get_pose(Pose &pose);

    /**
     * @brief Set the commanded velocity for the robot.
     *
     * @param cmd_vel The desired linear and angular velocity.
     */
    void set_cmd_vel(CmdVel cmd_vel);

    /**
     * @brief Reset the motor controller to its initial state.
     */
    void reset(void);

    /**
     * @brief Run the motor controller's control loop.
     * This function should be called on every iteration of the main loop.
     */
    void run(void);

    /**
     * @brief Print the current pose of the robot.
     */
    void print_pose(void);

    /**
     * @brief Moves the wheels forward at 0.3 m/s
     */
    void move_forward();
    /**
     * @brief Moves the wheels backward at -0.3 m/s
     */
    void move_backward();
    /**
     * @brief Turns the wheels left at 0.5 rad/s
     */
    void turn_left();
    /**
     * @brief Turns the wheels right at -0.5 rad/s
     */
    void turn_right();

    /**
     * @brief Stops the wheels
     */
    void stop();

    /**
     * @brief Moves the motors in open-loop mode.
     *
     * @param left_pwm The PWM value for the left motor.
     * @param right_pwm The PWM value for the right motor.
     */
    void move_open_loop(uint8_t left_pwm, uint8_t right_pwm);

    /**
     * @brief Get the current status of the motors.
     *
     * @param left_motor The status of the left motor.
     * @param right_motor The status of the right motor.
     */
    void get_motor_status(MotorData &left_motor, MotorData &right_motor);

    /**
     * @brief Reset the robot's pose to (0, 0, 0).
     */
    void reset_pose();

   private:
    /**
     * @brief Compute and update the robot's pose based on wheel travelled distances.
     */
    void compute_pose_();

    /**
     * @brief Compute the wheel speeds required to achieve the commanded velocity,
     * based on the Unicycle Kinematic Model.
     */
    void compute_wheel_speeds_();

   private:
    Pose pose_;                  ///< The current pose of the robot.
    CmdVel cmd_vel_;             ///< The commanded velocity for the robot.
    float dist_between_wheels_;  ///< The distance between the robot's two wheels.
    float prev_left_dist_;       ///< The previous distance travelled by the left wheel.
    float prev_right_dist_;  ///< The previous distance travelled by the right wheel.

   private:
    MotorDriver *left_motor_;   ///< Pointer to the left motor driver.
    MotorDriver *right_motor_;  ///< Pointer to the right motor driver.
    Timer motor_update_timer_;  ///< Timer for motor control updates.
};

#endif  // MOTOR_CONTROLLER_HPP
