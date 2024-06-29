/*
 * mh_z19.c
 *
 *  Created on: Jun 28, 2024
 *      Author: jules
 */

#include "mh_z19.h"

const uint8_t read_command[MH_Z19_COMMAND_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

bool mh_z19_check_crc(uint8_t *data)
{
	uint8_t sum = 0;

	for( uint8_t i = 1; i < MH_Z19_COMMAND_SIZE - 1; i++)
	{
		sum += data[i];
	}

	sum = 0xFF - sum;
	sum ++;
	return sum == data[8];
}

uint8_t mh_z19_get_co2_value(MH_Z19 *mh_z19_handle)
{
	HAL_StatusTypeDef tx_res = HAL_UART_Transmit(mh_z19_handle->uart_handle, read_command, MH_Z19_COMMAND_SIZE, MH_Z19_UART_TIMEOUT);

	HAL_Delay(50);

	uint8_t counter = 0;
	while (counter < 10000)
	{
      counter++;
	  if(HAL_UART_Receive(mh_z19_handle->uart_handle, mh_z19_handle->rx_buffer, MH_Z19_COMMAND_SIZE, MH_Z19_UART_TIMEOUT)==HAL_OK)
	  {
		__NOP();
		uint16_t result = 0;
		result = mh_z19_handle->rx_buffer[2];
		result <<= 8;
		result |= mh_z19_handle->rx_buffer[3];
		mh_z19_handle->co2_ppm = result;
	  } else {
		__NOP();
		return MH_Z19_ERROR_READ;
	  }

	  HAL_Delay(50);
	}

	return MH_Z19_ERROR_OK;
}
