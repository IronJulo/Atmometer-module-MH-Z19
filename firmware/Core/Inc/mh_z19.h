/*
 * mh_z19.h
 *
 *  Created on: Jun 28, 2024
 *      Author: jules
 */

#ifndef INC_MH_Z19_H_
#define INC_MH_Z19_H_

#include <stdbool.h>
#include "stm32c0xx_hal.h"

#define MH_Z19_COMMAND_SIZE 9

#define MH_Z19_ERROR_OK (0 << 0)
#define MH_Z19_ERROR_INVALID_CRC (1 << 0)
#define MH_Z19_ERROR_READ (1 << 1)
#define MH_Z19_ERROR_NOT_IMPL (-1)

#define MH_Z19_UART_TIMEOUT 1000

typedef struct {
	UART_HandleTypeDef *uart_handle;
	uint8_t rx_buffer[MH_Z19_COMMAND_SIZE];
	bool received_data_newer;
	bool received_data_valid;
	uint16_t co2_ppm;
} MH_Z19;

uint8_t mh_z19_get_co2_value(MH_Z19 *mh_z19_handle);
bool mh_z19_check_crc(uint8_t *data);

#endif /* INC_MH_Z19_H_ */
