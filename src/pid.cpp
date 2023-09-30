#include "pid.hpp"

PID::PID(pid_gains_t pid_gains, output_limits_t output_limits) {
    _pid_gains = pid_gains;
    _output_limits = output_limits;
    _error_sum = 0;
    _prev_error = 0;
    _setpoint = 0;
}

PID::PID(float kp, float ki, float kd, float min_output, float max_output) {
    _pid_gains = {kp, ki, kd};
    _output_limits = {min_output, max_output};
    _error_sum = 0;
    _prev_error = 0;
    _setpoint = 0;
}

void PID::set_pid_gains(pid_gains_t pid_gains) {
    _pid_gains = pid_gains;

    _check_pid_gains(_pid_gains);
}
void PID::set_pid_gains(float kp, float ki, float kd) {
    _pid_gains = {kp, ki, kd};
    _check_pid_gains(_pid_gains);
}

void PID::set_output_limits(output_limits_t output_limits) {
    _output_limits = output_limits;
    _check_output_limits(_output_limits);
}
void PID::set_output_limits(float min_output, float max_output) {
    _output_limits = {min_output, max_output};
    _check_output_limits(_output_limits);
}

void PID::set_setpoint(float setpoint) {
    _setpoint = setpoint;
    reset();
}

pid_gains_t PID::get_pid_gains() { return _pid_gains; }
float PID::get_p_gain() { return _pid_gains.kp; }
float PID::get_i_gain() { return _pid_gains.ki; }
float PID::get_d_gain() { return _pid_gains.kd; }

output_limits_t PID::get_output_limits() { return _output_limits; }
float PID::get_min_output_limit() { return _output_limits.min_output; }
float PID::get_max_output_limit() { return _output_limits.max_output; }

float PID::get_setpoint() { return _setpoint; }

void PID::reset() {
    _error_sum = 0;
    _prev_error = 0;
}

float PID::compute(float measured_value) {
    const float kp = _pid_gains.kp;
    const float ki = _pid_gains.ki;
    const float kd = _pid_gains.kd;
    const float min_output = _output_limits.min_output;
    const float max_output = _output_limits.max_output;

    const float error = _setpoint - measured_value;

    _error_sum = _bound_value(_error_sum + error, min_output, max_output);

    const float p = kp * error;
    const float i = ki * _error_sum;
    const float d = kd * (error - _prev_error);

    float correction = p + i + d;
    return _bound_value(correction, min_output, max_output);
}

float PID::_bound_value(float value, float min_value, float max_value) {
    if (value > max_value)
        return max_value;
    else if (value < min_value)
        return min_value;
    else
        return value;
    ;
}

bool PID::_check_output_limits(output_limits_t &output_limits) {
    if (output_limits.min_output > output_limits.max_output) {
        float temp = output_limits.max_output;
        output_limits.max_output = output_limits.min_output;
        output_limits.min_output = temp;
        return true;
    }
    return false;
}

bool PID::_check_pid_gains(pid_gains_t &pid_gains) {
    if (pid_gains.kp < 0 || pid_gains.ki < 0 || pid_gains.kd < 0) {
        pid_gains.kp = pid_gains.kp < 0 ? -pid_gains.kp : pid_gains.kp;
        pid_gains.ki = pid_gains.ki < 0 ? -pid_gains.ki : pid_gains.ki;
        pid_gains.kd = pid_gains.kd < 0 ? -pid_gains.kd : pid_gains.kd;
        return true;
    }
    return false;
}
