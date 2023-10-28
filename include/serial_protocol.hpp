#ifndef SERIAL_PROTOCOL_HPP
#define SERIAL_PROTOCOL_HPP

#include <Arduino.h>

#include "motor_controller.hpp"

// TODO: Add flags to update PID values
/**
 * @enum Flags
 * @brief Enumeration of recognized serial command flags.
 */
typedef enum {
    FLAG_CLOSE = 'c',        /**< Flag for closed-loop mode */
    FLAG_OPEN = 'o',         /**< Flag for open-loop mode */
    FLAG_POSE = 'q',         /**< Flag to request robot's pose (odometry) */
    FLAG_MOTOR_STATUS = 'm', /**< Flag to request motor status */
    FLAG_RESET = 'r',        /**< Flag to reset the robot's pose*/
} Flags;

/**
 * @class SerialProtocol
 * @brief Class responsible for parsing serial commands and interacting with
 * MotorController.
 */
class SerialProtocol {
   public:
    /**
     * @brief Constructs a new SerialProtocol object.
     * @param motorCtrl Pointer to the associated MotorController object.
     */
    SerialProtocol(MotorController* motorCtrl);

    /**
     * @brief Read serial input and process the commands.
     */
    void read_serial();

   private:
    /**
     * @brief Parses a received command string.
     * @param cmd String containing the received command.
     * @return Integer status code representing the outcome of the parsing.
     */
    int parse_cmd_(const String& cmd);

    /**
     * @brief Sends an acknowledgment message over serial.
     * @param code Integer code representing the status to acknowledge.
     */
    void send_ack(int code);

    MotorController*
        motorController_; /**< Pointer to the MotorController for motor operations. */
};

#endif  // !SERIAL_PROTOCOL_HPP
