#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

/**
 * @class Encoder
 * @brief A class to interface with an encoder using two input pins.
 */
class Encoder {
   public:
    /**
     * @brief Constructor for the Encoder class.
     * @param pin_a The digital pin connected to encoder signal A.
     * @param pin_b The digital pin connected to encoder signal B.
     * @param reverse Set to true to reverse the counting direction (optional, default
     * is false).
     */
    Encoder(uint8_t pin_a, uint8_t pin_b, bool reverse = false);

    /**
     * @brief Reset the encoder's tick count to zero.
     */
    void reset(void);

    /**
     * @brief Set the counting direction of the encoder.
     * @param reverse Set to true to reverse the counting direction, false otherwise.
     */
    void set_reverse(bool reverse);

    /**
     * @brief Interrupt service routine (ISR) called when an encoder tick is detected.
     * @details This ISR should be connected to one of the encoder's input pins.
     */
    void tick_isr(void);

    /**
     * @brief Get the current tick count of the encoder.
     * @return The current tick count as a 32-bit integer.
     */
    int32_t get_ticks(void);

   private:
    /**
     * @brief Initialize the input pins and interrupt for the encoder.
     */
    void init_pins(void);

   private:
    uint8_t pin_a_;           ///!< The digital pin connected to encoder phase A.
    uint8_t pin_b_;           ///!< The digital pin connected to encoder phase B.
    bool reverse_;            ///!< Flag to reverse the counting direction.
    volatile int32_t ticks_;  ///!< The current tick count.
};

#endif  // !ENCODER_HPP
