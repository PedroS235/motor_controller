#include "timer.hpp"

#include "Arduino.h"

Timer::Timer(unsigned long interval, bool start, bool auto_reset)
    : interval_(interval), auto_reset_(auto_reset) {
    if (start) {
        reset();
    }
}

void Timer::start(void) { reset(); }

void Timer::stop(void) { target_time_ = 0; }

void Timer::reset(void) { target_time_ = millis() + interval_; }

void Timer::set_auto_reset(bool auto_reset) { auto_reset_ = auto_reset; }

void Timer::set_interval(unsigned long interval) { interval_ = interval; }

bool Timer::has_elapsed() {
    unsigned long now = millis();
    bool elapsed = now >= target_time_;
    if (elapsed && auto_reset_) {
        unsigned long drift = now - target_time_;
        target_time_ = now + interval_ - drift;
    }
    return elapsed;
}
