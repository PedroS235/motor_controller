#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>

/**
 * @brief Bound an integer value within a specified range.
 * @param value The integer value to be bounded.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @details If the input `value` is less than `min`, it is set to `min`.
 * If it is greater than `max`, it is set to `max`.
 */
inline void bound(int &value, int min, int max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

/**
 * @brief Bound a double value within a specified range.
 * @param value The double value to be bounded.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @details If the input `value` is less than `min`, it is set to `min`.
 * If it is greater than `max`, it is set to `max`.
 */
inline void bound(double &value, double min, double max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

/**
 * @brief Bound a float value within a specified range.
 * @param value The float value to be bounded.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @details If the input `value` is less than `min`, it is set to `min`.
 * If it is greater than `max`, it is set to `max`.
 */
inline void bound(float &value, float min, float max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

/**
 * @brief Bound an unsigned 8-bit integer value within a specified range.
 * @param value The uint8_t value to be bounded.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @details If the input `value` is less than `min`, it is set to `min`.
 * If it is greater than `max`, it is set to `max`.
 */
inline void bound(uint8_t &value, uint8_t min, uint8_t max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

/**
 * @brief Transform Hz to milliseconds.
 * @param hz The frequency in Hz.
 * @return The period in milliseconds.
 *
 */
inline unsigned long hz_to_ms(uint8_t hz) { return 1000 / hz; }

/**
 * @brief Transform Hz to secons.
 * @param hz The frequency in Hz.
 * @return The period in seconds.
 *
 */
inline unsigned long hz_to_s(uint8_t hz) { return 1 / hz; }

#endif  // !UTILS_HPP
