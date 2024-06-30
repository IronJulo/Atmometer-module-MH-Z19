/*
 * mh_z19.c
 *
 *  Created on: Jun 28, 2024
 *      Author: jules
 */

#include "mh_z19.h"

const uint8_t read_command[MH_Z19_COMMAND_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void mh_z19_clear_rx_buffer(MH_Z19 *mh_z19_handle)
{
	mh_z19_handle->rx_buffer[0] = 0;
	mh_z19_handle->rx_buffer[1] = 0;
	mh_z19_handle->rx_buffer[2] = 0;
	mh_z19_handle->rx_buffer[3] = 0;
	mh_z19_handle->rx_buffer[4] = 0;
	mh_z19_handle->rx_buffer[5] = 0;
	mh_z19_handle->rx_buffer[6] = 0;
	mh_z19_handle->rx_buffer[7] = 0;
	mh_z19_handle->rx_buffer[8] = 0;
}

bool mh_z19_check_rx_crc(MH_Z19 *mh_z19_handle)
{
	uint8_t sum = 0;

	for( uint8_t i = 1; i < MH_Z19_COMMAND_SIZE - 1; i++)
	{
		sum += mh_z19_handle->rx_buffer[i];
	}

	sum = 0xFF - sum;
	sum ++;
	return sum == mh_z19_handle->rx_buffer[8];
}

uint8_t mh_z19_init(MH_Z19 *mh_z19_handle, UART_HandleTypeDef *uart_handle)
{
	mh_z19_handle->uart_handle = uart_handle;
	mh_z19_clear_rx_buffer(mh_z19_handle);
	mh_z19_handle->rx_complete = false;
	mh_z19_handle->rx_requested = false;
	mh_z19_handle->continuous_mode = false;
	mh_z19_handle->co2_ppm = 0;

	return MH_Z19_ERROR_OK;
}

uint8_t mh_z19_request_data(MH_Z19 *mh_z19_handle)
{
	//mh_z19_clear_rx_buffer(mh_z19_handle);
	HAL_UART_Transmit(mh_z19_handle->uart_handle, read_command, MH_Z19_COMMAND_SIZE, MH_Z19_UART_TIMEOUT);
	HAL_UART_Receive_IT(mh_z19_handle->uart_handle, mh_z19_handle->rx_buffer, MH_Z19_COMMAND_SIZE);

	return MH_Z19_ERROR_OK;
}

uint8_t mh_z19_on_rx_cplt(MH_Z19 *mh_z19_handle)
{
	if (mh_z19_check_rx_crc(mh_z19_handle))
	{
		uint16_t result = 0;
		result = mh_z19_handle->rx_buffer[2];
		result <<= 8;
		result |= mh_z19_handle->rx_buffer[3];
		mh_z19_handle->co2_ppm = result;
	}
	else
	{
		return MH_Z19_ERROR_INVALID_CRC;
	}

	//mh_z19_clear_rx_buffer(mh_z19_handle);

	if (mh_z19_handle->continuous_mode)
		mh_z19_request_data(mh_z19_handle);

	return MH_Z19_ERROR_OK;
}
