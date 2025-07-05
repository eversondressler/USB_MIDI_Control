/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
/*************************************************************************************************/
#define NUM_CANAIS_ADC 8
//#define ALPHA 0.1f  // Ajuste conforme desejado (entre 0.0 e 1.0)

/*************************************************************************************************/

/*************************************************************************************************/
void divide_adc_12bit(uint32_t adc_value, uint8_t *msb7, uint8_t *lsb7)
{
    if (adc_value > 4095) adc_value = 4095; // Garante faixa válida

    *msb7 = (adc_value >> 5) & 0x7F;  // 7 bits mais significativos (bits 11 a 5)

    uint8_t remainder = adc_value & 0x1F;  // 5 bits menos significativos (bits 4 a 0)

    // Justifica os 5 bits restantes para ocupar até 7 bits (escalonamento)
    // Multiplica por 4 para expandir de 0-31 para 0-124 (mantém dentro de 7 bits)
    *lsb7 = (remainder << 2) & 0x7F;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint32_t calibrate_adc(uint32_t raw)
{
    const uint16_t raw_min = 2;
    const uint16_t raw_max = 4095;
    if (raw < raw_min) raw = raw_min;
    if (raw > raw_max) raw = raw_max;
    return (raw - raw_min) * 4095 / (raw_max - raw_min);
}
/*************************************************************************************************/

/*************************************************************************************************/
void touch_fader(uint8_t canal, uint32_t *AD)
{
	uint8_t touchs_in[] = {0x0B, 0xB0, 0x0F, 0x00, 0x0B, 0xB0, 0x2F, 0x40, 0x0B, 0xB0, 0x00, 0x00, 0x0B, 0xB0, 0x20, 0x00};
	uint8_t touchs_out[] = {0x0B, 0xB0, 0x0F, 0x00, 0x0B, 0xB0, 0x2F, 0x00};
	uint8_t faders[] = {0x0B, 0xB0, 0x00, 0x00, 0x0B, 0xB0, 0x20, 0x00};
	static bool touch[NUM_CANAIS_ADC];
	static uint32_t AD_last[NUM_CANAIS_ADC];
	uint8_t msb, lsb;
	static uint16_t debouce[NUM_CANAIS_ADC];
	char c[NUM_CANAIS_ADC];
	USB_MIDI_PACKET fader;

	if (AD[canal] != AD_last[canal])
	    {
	    	if(debouce[canal] >= 400)
			{
	    		AD_last[canal] = AD[canal];

	    		divide_adc_12bit(AD[canal], &msb, &lsb);

	            if(touch[canal] == false)
	            {
	            	touch[canal] = true;

	            	// Copia tudo de uma vez para `fader.midiData_v`
	            	memcpy(fader.midiData_v, touchs_in, sizeof(touchs_in));

	            	// Insere apenas os bytes variáveis
	            	fader.midiData_v[11] = msb;
	            	fader.midiData_v[15] = lsb;

					MIDI_DataIn_FS(fader.midiData_v, sizeof(touchs_in));
	            }
	            else
	            {
	            	memcpy(fader.midiData_v, faders, sizeof(faders));

	            	fader.midiData_v[3] = msb;
					fader.midiData_v[7] = lsb;

					MIDI_DataIn_FS(fader.midiData_v, sizeof(faders));

					sprintf(c,"%ld", AD[canal]);
					ssd1306_Fill(Black);
					ssd1306_SetCursor(0, 40);
					ssd1306_WriteString(c, Font_11x18, White);
					ssd1306_UpdateScreen();
	            }
	        }
	    	else debouce[canal]++;
	    }
	    else
	    {
	    	if(touch[canal] == true)
	    	{
	    		if(debouce[canal] > 0)
	    		{
	    			touch[canal] = false;
	    			debouce[canal] = 0;

	    			memcpy(fader.midiData_v, touchs_out, sizeof(touchs_out));

					MIDI_DataIn_FS(fader.midiData_v, sizeof(touchs_out));
	    		}
	    		else debouce[canal] --;
	    	}
	    	else debouce[canal] = 0;
	    }
}
/*************************************************************************************************/

/*************************************************************************************************/
void analog(void)
{
	static uint32_t AD[NUM_CANAIS_ADC];
	uint8_t canal = 0;

	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
//		AD[canal] = aplicaFiltroIIR(canal, calibrate_adc(HAL_ADC_GetValue(&hadc1)));
		AD[canal] = calibrate_adc(HAL_ADC_GetValue(&hadc1));
	}
	HAL_ADC_Stop(&hadc1);

	touch_fader(canal, &AD[canal]);

    if(canal >= NUM_CANAIS_ADC) canal = 0;
    else canal++;
}
/*************************************************************************************************/

/*************************************************************************************************/
//uint32_t aplicaFiltroIIR(uint8_t canal, uint32_t sample)
//{
//	static float adc_filtrado[NUM_CANAIS_ADC];  // Valor suavizado de cada canal
//
//	adc_filtrado[canal] = (ALPHA * sample) + ((1.0f - ALPHA) * adc_filtrado[canal]);
//	return (uint32_t)adc_filtrado[canal];
//}
/*************************************************************************************************/

/*************************************************************************************************/
//uint16_t buffer[N];
//uint8_t indexx = 0;
//uint32_t get_filtered_adc(uint32_t new_sample)
//{
//    buffer[indexx++] = new_sample;
//
//    if(indexx >= N) indexx = 0;
//
//    uint32_t sum = 0;
//
//    for(int i = 0; i < N; i++) sum += buffer[i];
//
//    return (uint32_t)(sum / N);
//}
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = ENABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = AN0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AN0_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(AN0_GPIO_Port, AN0_Pin);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
