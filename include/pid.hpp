#ifndef PID_HPP
#define PID_HPP

/**
 * @struct pid_gains_t
 * @brief Structure to store PID controller gains (proportional, integral, and
 * derivative).
 */
typedef struct {
    float kp;  ///< Proportional gain.
    float ki;  ///< Integral gain.
    float kd;  ///< Derivative gain.
} pid_gains_t;

/**
 * @struct output_limits_t
 * @brief Structure to define output limits for the PID controller.
 */
typedef struct {
    float min_output;  ///< Minimum allowable output value.
    float max_output;  ///< Maximum allowable output value.
} output_limits_t;

/**
 * @class PID
 * @brief A class to implement a Proportional-Integral-Derivative (PID) controller.
 */
class PID {
   private:
    pid_gains_t _pid_gains;
    output_limits_t _output_limits;
    float _error_sum, _prev_error, _setpoint;

   public:
    /**
     * @brief Constructor for the PID class.
     * @param pid_gains Structure containing PID gains (kp, ki, kd).
     * @param output_limits Structure containing output limits (min_output, max_output).
     */
    PID(pid_gains_t pid_gains, output_limits_t output_limits = {0, 255});

    /**
     * @brief Constructor for the PID class.
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     * @param min_output Minimum allowable output value (optional, default is 0).
     * @param max_output Maximum allowable output value (optional, default is 255).
     */
    PID(float kp, float ki, float kd, float min_output = 0, float max_output = 255);

    /**
     * @brief Set the output limits for the PID controller.
     * @param output_limits Structure containing output limits (min_output, max_output).
     */
    void set_output_limits(output_limits_t output_limits);

    /**
     * @brief Set the output limits for the PID controller.
     * @param min_output Minimum allowable output value.
     * @param max_output Maximum allowable output value.
     */
    void set_output_limits(float min_output, float max_output);

    /**
     * @brief Set the PID gains for the controller.
     * @param pid_gains Structure containing PID gains (kp, ki, kd).
     */
    void set_pid_gains(pid_gains_t pid_gains);

    /**
     * @brief Set the PID gains for the controller.
     * @param kp Proportional gain.
     * @param ki Integral gain.
     * @param kd Derivative gain.
     */
    void set_pid_gains(float kp, float ki, float kd);

    /**
     * @brief Set the setpoint (target value) for the PID controller.
     * @param setpoint The desired setpoint value.
     */
    void set_setpoint(float setpoint);

    /**
     * @brief Get the PID gains for the controller.
     * @return Structure containing PID gains (kp, ki, kd).
     */
    pid_gains_t get_pid_gains(void);

    /**
     * @brief Get the proportional gain (kp) for the PID controller.
     * @return The proportional gain value.
     */
    float get_p_gain(void);

    /**
     * @brief Get the integral gain (ki) for the PID controller.
     * @return The integral gain value.
     */
    float get_i_gain(void);

    /**
     * @brief Get the derivative gain (kd) for the PID controller.
     * @return The derivative gain value.
     */
    float get_d_gain(void);

    /**
     * @brief Get the output limits for the PID controller.
     * @return Structure containing output limits (min_output, max_output).
     */
    output_limits_t get_output_limits(void);

    /**
     * @brief Get the minimum allowable output limit for the PID controller.
     * @return The minimum allowable output value.
     */
    float get_min_output_limit(void);

    /**
     * @brief Get the maximum allowable output limit for the PID controller.
     * @return The maximum allowable output value.
     */
    float get_max_output_limit(void);

    /**
     * @brief Get the current setpoint (target value) for the PID controller.
     * @return The current setpoint value.
     */
    float get_setpoint(void);

    /**
     * @brief Reset the PID controller to its initial state.
     */
    void reset(void);

    /**
     * @brief Compute the control output based on the measured value.
     * @param measured_value The current measured value from the system.
     * @return The computed control output.
     */
    float compute(float measured_value);

   private:
    /**
     * @brief Bound a value within the specified range.
     * @param value The value to be bounded.
     * @param min_value The minimum allowable value.
     * @param max_value The maximum allowable value.
     * @return The bounded value.
     */
    float _bound_value(float value, float min_value, float max_value);

    /**
     * @brief Check if the provided output limits are valid (min < max).
     * @param output_limits Structure containing output limits (min_output, max_output).
     * @return True if the output limits are valid, false otherwise.
     */
    bool _check_output_limits(output_limits_t &output_limits);

    /**
     * @brief Check if the provided PID gains are valid (kp, ki, kd > 0).
     * @param pid_gains Structure containing PID gains (kp, ki, kd).
     * @return True if the PID gains are valid, false otherwise.
     */
    bool _check_pid_gains(pid_gains_t &pid_gains);
};

#endif  // !PID_HPP
