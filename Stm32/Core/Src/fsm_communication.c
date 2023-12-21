/*
 * fsm_uart_communiation.c
 *
 *  Created on: Dec 18, 2023
 *      Author: yen
 */

#include "fsm_uart_comunication.h"

// Define states
#define INIT        0
#define READ_ADC    1
#define SHOW_ADC    2

// Global variables
uint8_t state_uart_fsm = INIT;   // Initial state is INIT
uint32_t adc_value = 0;          // Variable to store ADC value

// FSM function for UART communication
void uart_communiation_fsm(){
    switch (state_uart_fsm){
        // Initialization state
        case INIT:
            state_uart_fsm = READ_ADC;  // Transition to READ_ADC state
            break;

        // Read ADC state
        case READ_ADC:
            // Check if the flag indicating ADC reading is set
            if (read_adc_flag) {
                read_adc_flag = 0;    // Reset the flag
                ok_flag = 0;          // Reset OK flag for handling "OK" command before reset

                // Read ADC value
                adc_value = HAL_ADC_GetValue(&hadc1);

                // Print ADC value through UART
                uint8_t adc_str[20];
                uint16_t size = sprintf(adc_str, "!%d#\r\n", adc_value);
                HAL_UART_Transmit(&huart2, adc_str, size, 1000);

                // Set a timer to print the value of ADC every 3 seconds until "OK" is received from UART
                set_timeout_ok(300);  // 3 seconds

                state_uart_fsm = SHOW_ADC;  // Transition to SHOW_ADC state
            }
            break;

        // Show ADC state
        case SHOW_ADC:
            // Check conditions to transition back to READ_ADC state
            if (ok_flag || read_adc_flag) {
                state_uart_fsm = READ_ADC;
                return;
            }

            // Check if the timeout for "OK" is reached
            if (get_timeout_ok()) {
                set_timeout_ok(300);  // 3 seconds

                // Print ADC value through UART
                uint8_t adc_str[20];
                uint16_t size = sprintf(adc_str, "!%d#\r\n", adc_value);
                HAL_UART_Transmit(&huart2, adc_str, size, 1000);
            }
            break;

        // Default case (should not be reached)
        default:
            break;
    }
}
