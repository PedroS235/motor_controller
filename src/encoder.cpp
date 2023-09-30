#include "encoder.hpp"

#include "Arduino.h"

Encoder::Encoder(uint8_t pin_a, uint8_t pin_b, bool reverse)
    : pin_a_(pin_a), pin_b_(pin_b), reverse_(reverse), ticks_(0) {
    init_pins();
}

void Encoder::init_pins() {
    pinMode(pin_a_, INPUT_PULLUP);
    pinMode(pin_b_, INPUT_PULLUP);
}

void Encoder::reset() { ticks_ = 0; }

void Encoder::set_reverse(bool reverse) { reverse_ = reverse; }

int32_t Encoder::get_ticks() {
    int32_t ticks = 0;
    { ticks = ticks_; }
    return reverse_ ? -ticks : ticks;
}

void Encoder::tick_isr() {
    if (digitalRead(pin_b_) == HIGH) {
        ticks_--;
    } else {
        ticks_++;
    }
}
