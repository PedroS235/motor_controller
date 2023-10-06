#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP

#include <Arduino.h>

#include "encoder.hpp"
#include "pid.hpp"

/**
 * @struct MotorData
 * @brief Structure to store motor-related data such as velocity, angular velocity,
 * distance, angle, and RPM.
 */
typedef struct {
    float velocity;          ///< Linear velocity of the motor (in m/s).
    float angular_velocity;  ///< Angular velocity of the motor (in rad/s).
    float distance;          ///< Total Traveled distance (in m).
    float angle;             ///< Angular position of the motor (in radians).
    float rpm;               ///< Revolutions per minute (RPM) of the motor.
} MotorData;

/**
 * @enum MotorDirection
 * @brief Enumerates the possible motor rotation directions: Clockwise (CW),
 * Counter-Clockwise (CCW), and STOP.
 *
 * @details CW can also be referred to as Forward, and CCW can also be referred to as
 * Reverse.
 */
enum class MotorDirection { CW, CCW, STOP };

/**
 * @enum MotorMode
 * @brief Enumerates the possible motor operation modes: Open-Loop and Closed-Loop.
 */
enum class MotorMode { OPEN_LOOP, CLOSED_LOOP };

/**
 * @class MotorDriver
 * @brief A class to control a motor with optional closed-loop feedback using an encoder
 * and PID controller.
 */
class MotorDriver {
   public:
    /**
     * @brief Constructor for the MotorDriver class (Open-Loop mode). No ability switch
     * to closed-loop mode.
     * @param pin_en The motor enable pin.
     * @param pin_in1 The motor input 1 pin.
     * @param pin_in2 The motor input 2 pin.
     * @param reverse Set to true to reverse motor direction (optional, default is
     * false).
     */
    MotorDriver(uint8_t pin_en, uint8_t pin_in1, uint8_t pin_in2, bool reverse = false);

    /**
     * @brief Constructor for the MotorDriver class (Closed-Loop mode).
     * @param pin_en The motor enable pin.
     * @param pin_in1 The motor input 1 pin.
     * @param pin_in2 The motor input 2 pin.
     * @param encoder Pointer to an Encoder object for closed-loop control.
     * @param wheel_radius_ The radius of the wheel connected to the motor (in meters).
     * @param ticks_per_rev The number of encoder ticks per revolution
     * @param reverse Set to true to reverse motor direction (optional, default is
     * false).
     */
    MotorDriver(uint8_t pin_en,
                uint8_t pin_in1,
                uint8_t pin_in2,
                Encoder *encoder,
                float wheel_radius_,
                uint16_t ticks_per_rev,
                bool reverse = false);

    /**
     * @brief Run the motor control loop. Needs to be called on every iteration of the
     * main loop. For better performance, use a timer to call this function at a fixed
     * frequency like 20Hz.
     */
    void run(void);

    /**
     * @brief Print the current status and data of the motor.
     */
    void print_status(void);

    /**
     * @brief Reset the motor-related data such as velocity, angular velocity, distance,
     * angle, and RPM.
     */
    void reset(void);

    /**
     * @brief Set the desired linear velocity of the motor (Closed-Loop mode).
     * @param velocity The desired linear velocity (in meters per second).
     */
    void set_velocity(float velocity);

    /**
     * @brief Set the PWM value and operation mode for motor control.
     * @param pwm The PWM value (-255-255). Negative values correspond to CCW rotation,
     * @param mode The motor operation mode (Open-Loop or Closed-Loop).
     */
    void set_pwm(int pwm, MotorMode mode = MotorMode::OPEN_LOOP);

    /**
     * @brief Set the operation mode of the motor (Open-Loop or Closed-Loop).
     * @param mode The motor operation mode.
     */
    void set_mode(MotorMode mode);

    /**
     * @brief Get motor-related data such as velocity, angular velocity, distance,
     * angle, and RPM.
     * @param motor_data Reference to a MotorData structure to store the data.
     */
    void get_motor_data(MotorData &motor_data);

    /**
     * @brief Get the wheel radius of the wheel attached to the motor.
     */
    float get_wheel_radius();

    /**
     * @brief Get the number of encoder ticks per revolution.
     * @return The number of encoder ticks per revolution.
     */
    uint16_t get_ticks_per_rev();

   private:
    /**
     * @brief Send the PWM signal to control the motor (L298N Driver).
     */
    void send_pwm(void);

    /**
     * @brief Set the direction of motor rotation (CW, CCW, or STOP).
     * @param dir The desired motor direction.
     */
    void set_direction(MotorDirection dir);

    /**
     * @brief Initialize the motor control pins.
     */
    void init_pins_(void);

    /**
     * @brief Compute the RPM (Revolutions Per Minute) of the motor based on encoder
     * readings.
     * @return The calculated RPM value.
     */
    float compute_rpm_(void);

    /**
     * @brief Compute the angular velocity (radians per second) of the motor.
     * @param rpm The RPM value of the motor.
     * @return The calculated angular velocity.
     */
    float compute_angular_velocity_(float rpm);

    /**
     * @brief Compute the linear velocity (meters per second) of the motor.
     * @param angular_velocity The angular velocity (radians per second).
     * @return The calculated linear velocity.
     */
    float compute_velocity_(float angular_velocity);

    /**
     * @brief Compute the total distance traveled by the motor.
     * @return The calculated distance (in meters).
     */
    float compute_distance_(void);

    /**
     * @brief Compute the angle (position) of the motor wheel.
     * @return The calculated angle (in radians).
     */
    float compute_wheel_angle_(void);

    /**
     * @brief Compute motor-related data such as velocity, angular velocity, distance,
     * angle, and RPM.
     */
    void compute_motor_data_(void);

   private:
    // Pins
    uint8_t pin_en_;   ///< Motor enable pin (PWM).
    uint8_t pin_in1_;  ///< Motor input 1 pin.
    uint8_t pin_in2_;  ///< Motor input 2 pin.

    // Parameters
    float wheel_radius_;  ///< Radius of the wheel connected to the motor (in meters).
    uint16_t ticks_per_rev_;  ///< Number of encoder ticks per revolution.
    bool reverse_;            ///< Flag to reverse motor direction.

    // Sensor Readings
    MotorData motor_data_;                  ///< Motor-related data.
    int32_t last_encoder_reading_;          ///< Last encoder reading.
    unsigned long last_data_reading_time_;  ///< Time of the last data reading.

    MotorDirection motor_dir_;  ///< Current motor direction.
    MotorMode motor_mode_;      ///< Current motor operation mode.
    Encoder *encoder_;          ///< Pointer to the encoder object.
    PID pid_;                   ///< PID controller for closed-loop control.
    uint8_t pwm_;               ///< PWM value for motor control.
};

#endif  // MOTOR_DRIVER_HPP
