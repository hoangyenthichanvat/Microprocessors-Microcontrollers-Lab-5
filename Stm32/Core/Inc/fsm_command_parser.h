/*
 * fsm_command_parser.h
 *
 *  Created on: Dec 18, 2023
 *      Author: yen
 */

#ifndef INC_FSM_COMMAND_PARSER_H_
#define INC_FSM_COMMAND_PARSER_H_

#include "global.h"

// External declarations of flags used in the FSM
extern uint8_t read_adc_flag;  // Flag indicating ADC reading
extern uint8_t ok_flag;        // Flag for handling "OK" command

// Function declaration for the command parser FSM
void command_parser_fsm();

#endif /* INC_FSM_COMMAND_PARSER_H_ */
