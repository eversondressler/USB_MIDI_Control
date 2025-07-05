/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_audio_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_audio_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_AUDIO_IF_H__
#define __USBD_AUDIO_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi.h"
/* USER CODE BEGIN INCLUDE */
typedef union
{
	uint32_t midiData;   // Acessa os primeiros 4 bytes como inteiro de 32 bits
	uint8_t  midiData_v[64];  // Buffer completo de 64 bytes (tamanho típico de um pacote MIDI USB)

	struct
	{
		uint8_t status;  // Combinação de CIN (4 bits) e CN (4 bits)
		uint8_t MIDI_0;  // Primeiro byte de dados MIDI
		uint8_t MIDI_1;  // Segundo byte de dados MIDI
		uint8_t MIDI_2;  // Terceiro byte de dados MIDI
	};

}USB_MIDI_PACKET;


/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_AUDIO_IF USBD_AUDIO_IF
  * @brief Usb audio interface device module.
  * @{
  */

/** @defgroup USBD_AUDIO_IF_Exported_Defines USBD_AUDIO_IF_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_Types USBD_AUDIO_IF_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_Macros USBD_AUDIO_IF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_Variables USBD_AUDIO_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** AUDIO_IF Interface callback. */
extern USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
extern uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_FunctionsPrototype USBD_AUDIO_IF_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/**
  * @brief  Manages the DMA full transfer complete event.
  * @retval None
  */


/**
  * @brief  Manages the DMA half transfer complete event.
  * @retval None
  */


/* USER CODE BEGIN EXPORTED_FUNCTIONS */
//Create MIDI Message buffer

void USBD_MidiMessage(uint8_t cable, uint8_t byte1, uint8_t byte2, uint8_t byte3);
/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_AUDIO_IF_H__ */
