/*
 * fsm_command_parser.c
 *
 *  Created on: Dec 18, 2023
 *      Author: yen
 */

#include "fsm_command_parser.h"

uint8_t read_adc_flag = 0;
uint8_t ok_flag = 0;

#define INIT 0
#define RESET 1
#define OK 2

uint8_t state_command = INIT;

uint8_t check_is_ok() {
    uint8_t str_ok[4] = "!OK#";

    uint8_t index_buffer_tem = (index_buffer == 0) ? MAX_BUFFER_SIZE - 1 : index_buffer - 1;

    for (int8_t i = 3; i >= 0; i--) {
        if (buffer[index_buffer_tem] != str_ok[i])
            return 0;
        index_buffer_tem = (index_buffer_tem == 0) ? MAX_BUFFER_SIZE - 1 : index_buffer_tem - 1;
    }

    uint8_t data_send[20] = "!recv OK#\r\n";
    HAL_UART_Transmit(&huart2, data_send, sizeof(data_send) - 1, 1000);

    return 1;
}

uint8_t check_is_rst() {
    uint8_t str_rst[5] = "!RST#";
    uint8_t index_buffer_tem = (index_buffer == 0) ? MAX_BUFFER_SIZE - 1 : index_buffer - 1;

    for (int8_t i = 4; i >= 0; i--) {
        if (buffer[index_buffer_tem] != str_rst[i])
            return 0;
        index_buffer_tem = (index_buffer_tem == 0) ? MAX_BUFFER_SIZE - 1 : index_buffer_tem - 1;
    }

    uint8_t data_send[20] = "!recv RST#\r\n";
    HAL_UART_Transmit(&huart2, data_send, sizeof(data_send) - 1, 1000);

    return 1;
}

void command_parser_fsm() {
    switch (state_command) {
        case INIT:
            state_command = RESET;
            break;
        case RESET:
            if (check_is_rst()) {
                read_adc_flag = 1;
                state_command = OK;
            }
            break;
        case OK:
            if (check_is_ok()) {
                ok_flag = 1;
                state_command = RESET;
                return;
            }
            if (check_is_rst()) {
                read_adc_flag = 1;
                return;
            }
            break;
        default:
            break;
    }
}
