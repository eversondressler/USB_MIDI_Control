/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_audio_if.c
  * @version        : v1.0_Cube
  * @brief          : Generic media access layer.
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

/* Includes ------------------------------------------------------------------*/


/* USER CODE BEGIN INCLUDE */
//#include "usbd_midi.h"
#include "usbd_midi_if.h"
//#include "ssd1306.h"
//#include "ssd1306_fonts.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
// Compile-time Assert
#define TU_ATTR_PACKED	__attribute__ ((packed))

//#if defined (__cplusplus) && __cplusplus >= 201103L
//  #define TU_VERIFY_STATIC   static_assert
//#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
//  #define TU_VERIFY_STATIC Static_assert
//#elif defined(__CCRX__)
//  #define TU_VERIFY_STATIC(const_expr, _mess) typedef char TU_XSTRCAT(_verify_static_, _TU_COUNTER_)[(const_expr) ? 1 : 0];
//#else
//  #define TU_VERIFY_STATIC(const_expr, _mess) enum { TU_XSTRCAT(_verify_static_, _TU_COUNTER_) = 1/(!!(const_expr)) }
//#endif
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_AUDIO_IF
  * @{
  */

/** @defgroup USBD_AUDIO_IF_Private_TypesDefinitions USBD_AUDIO_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

//--------------------------------------------------------------------+
// Constants
//--------------------------------------------------------------------+

/* Code Index Number (CIN) values */
/*   Table 4-1 of midi10.pdf      */

enum{
	MIDI_VERSION_1_0 = 0x0100,
	MIDI_VERSION_2_0 = 0x0200,
};

typedef enum{
	MIDI_CS_INTERFACE_HEADER    = 0x01,
	MIDI_CS_INTERFACE_IN_JACK   = 0x02,
	MIDI_CS_INTERFACE_OUT_JACK  = 0x03,
	MIDI_CS_INTERFACE_ELEMENT   = 0x04,
}midi_cs_interface_subtype_t;

typedef enum{
	MIDI_CS_ENDPOINT_GENERAL = 0x01,
	MIDI_CS_ENDPOINT_GENERAL_2_0 = 0x02,
}midi_cs_endpoint_subtype_t;

typedef enum{
	MIDI_JACK_EMBEDDED = 0x01,
	MIDI_JACK_EXTERNAL = 0x02
}midi_jack_type_t;

typedef enum{
	MIDI_CIN_MISC              	= 0,
	MIDI_CIN_CABLE_EVENT       	= 1,
	MIDI_CIN_SYSCOM_2BYTE      	= 2, // 2 byte system common message e.g MTC, SongSelect
	MIDI_CIN_SYSCOM_3BYTE      	= 3, // 3 byte system common message e.g SPP
	MIDI_CIN_SYSEX_START       	= 4, // SysEx starts or continue
	MIDI_CIN_SYSEX_END_1BYTE   	= 5, // SysEx ends with 1 data, or 1 byte system common message
	MIDI_CIN_SYSEX_END_2BYTE   	= 6, // SysEx ends with 2 data
	MIDI_CIN_SYSEX_END_3BYTE   	= 7, // SysEx ends with 3 data
	MIDI_CIN_NOTE_OFF          	= 8,
	MIDI_CIN_NOTE_ON           	= 9,
	MIDI_CIN_POLY_KEYPRESS     	= 10,
	MIDI_CIN_CONTROL_CHANGE    	= 11,
	MIDI_CIN_PROGRAM_CHANGE    	= 12,
	MIDI_CIN_CHANNEL_PRESSURE  	= 13,
	MIDI_CIN_PITCH_BEND_CHANGE 	= 14,
	MIDI_CIN_1BYTE_DATA 		= 15
}midi_code_index_number_t;

// MIDI 1.0 status byte
enum{
	//------------- System Exclusive -------------//
	MIDI_STATUS_SYSEX_START                    = 0xF0,
	MIDI_STATUS_SYSEX_END                      = 0xF7,

	//------------- System Common -------------//
	MIDI_STATUS_SYSCOM_TIME_CODE_QUARTER_FRAME = 0xF1,
	MIDI_STATUS_SYSCOM_SONG_POSITION_POINTER   = 0xF2,
	MIDI_STATUS_SYSCOM_SONG_SELECT             = 0xF3,
	// F4, F5 is undefined
	MIDI_STATUS_SYSCOM_TUNE_REQUEST            = 0xF6,

	//------------- System RealTime  -------------//
	MIDI_STATUS_SYSREAL_TIMING_CLOCK           = 0xF8,
	// 0xF9 is undefined
	MIDI_STATUS_SYSREAL_START                  = 0xFA,
	MIDI_STATUS_SYSREAL_CONTINUE               = 0xFB,
	MIDI_STATUS_SYSREAL_STOP                   = 0xFC,
	// 0xFD is undefined
	MIDI_STATUS_SYSREAL_ACTIVE_SENSING         = 0xFE,
	MIDI_STATUS_SYSREAL_SYSTEM_RESET           = 0xFF,
};

enum{
	MIDI_MAX_DATA_VAL = 0x7F,
};

//--------------------------------------------------------------------+
// Class Specific Descriptor
//--------------------------------------------------------------------+

/// MIDI Interface Header Descriptor
typedef struct TU_ATTR_PACKED{
	uint8_t  bLength;           ///< Size of this descriptor in bytes.
	uint8_t  bDescriptorType;   ///< must be TUSB_DESC_CS_INTERFACE
	uint8_t  bDescriptorSubType;///< Descriptor SubType
	uint16_t bcdMSC;            ///< MidiStreaming SubClass release number in Binary-Coded Decimal
	uint16_t wTotalLength;
}midi_desc_header_t;
//TU_VERIFY_STATIC(sizeof(midi_desc_header_t) == 7, "size is not correct");

/// MIDI In Jack Descriptor
typedef struct TU_ATTR_PACKED{
	uint8_t bLength;           ///< Size of this descriptor in bytes.
	uint8_t bDescriptorType;   ///< Descriptor Type, must be Class-Specific
	uint8_t bDescriptorSubType;///< Descriptor SubType
	uint8_t bJackType;         ///< Embedded or External
	uint8_t bJackID;           ///< Unique ID for MIDI IN Jack
	uint8_t iJack;             ///< string descriptor
}midi_desc_in_jack_t;
//TU_VERIFY_STATIC(sizeof(midi_desc_in_jack_t) == 6, "size is not correct");

/// MIDI Out Jack Descriptor with multiple input pins
#define midi_desc_out_jack_n_t(input_num)	\
	struct TU_ATTR_PACKED{                 	\
		uint8_t bLength;                    \
		uint8_t bDescriptorType;            \
		uint8_t bDescriptorSubType;         \
		uint8_t bJackType;                  \
		uint8_t bJackID;                    \
		uint8_t bNrInputPins;               \
		struct TU_ATTR_PACKED{              \
			uint8_t baSourceID;             \
			uint8_t baSourcePin;            \
		}input[input_num];                  \
		uint8_t iJack;                      \
	}

typedef midi_desc_out_jack_n_t(1)  midi_desc_out_jack_1in_t; // 1 input
typedef midi_desc_out_jack_1in_t midi_desc_out_jack_t; // backward compatible
//TU_VERIFY_STATIC(sizeof(midi_desc_out_jack_1in_t) == 7 + 2 * 1, "size is not correct");

/// MIDI Element Descriptor with multiple pins
#define midi_desc_element_n_t(input_num) \
  struct TU_ATTR_PACKED {       \
    uint8_t bLength;            \
    uint8_t bDescriptorType;    \
    uint8_t bDescriptorSubType; \
    uint8_t bElementID;         \
    uint8_t bNrInputPins;       \
    struct TU_ATTR_PACKED {     \
        uint8_t baSourceID;     \
        uint8_t baSourcePin;    \
    } pins[input_num];          \
    uint8_t bNrOutputPins;      \
    uint8_t bInTerminalLink;    \
    uint8_t bOutTerminalLink;   \
    uint8_t bElCapsSize;        \
    uint16_t bmElementCaps;     \
    uint8_t  iElement;          \
 }

// This descriptor follows the standard bulk data endpoint descriptor
#define midi_desc_cs_endpoint_n_t(jack_num) \
  struct TU_ATTR_PACKED {                   \
    uint8_t bLength;                        \
    uint8_t bDescriptorType;                \
    uint8_t bDescriptorSubType;             \
    uint8_t bNumEmbMIDIJack;                \
    uint8_t baAssocJackID[jack_num];        \
  }

typedef midi_desc_cs_endpoint_n_t() midi_desc_cs_endpoint_t; // empty/flexible jack list
typedef midi_desc_cs_endpoint_n_t(1) midi_desc_cs_endpoint_1jack_t;

//TU_VERIFY_STATIC(sizeof(midi_desc_cs_endpoint_1jack_t) == 4+1, "size is not correct");

//--------------------------------------------------------------------+
// For Internal Driver Use
//--------------------------------------------------------------------+
typedef struct {
  uint8_t buffer[4];
  uint8_t index;
  uint8_t total;
} midi_driver_stream_t;

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Private_Defines USBD_AUDIO_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Private_Macros USBD_AUDIO_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Private_Variables USBD_AUDIO_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Exported_Variables USBD_AUDIO_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
uint8_t MIDI_Init_FS(void);           // Função para inicializar a interface MIDI
uint8_t MIDI_DeInit_FS(void);         // Função para desinicializar a interface MIDI
uint8_t MIDI_DataIn_FS(uint8_t *data, uint16_t length);  // Função para enviar dados MIDI para o host
uint8_t MIDI_DataOut_FS(uint8_t *data, uint16_t length); // Função para processar dados MIDI recebidos do host
uint8_t MIDI_Receive_FS(uint8_t *data, uint16_t length); // Função para receber pacotes MIDI
uint8_t MIDI_GetState_FS(void);       // Função para obter o estado da interface MIDI
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_AUDIO_IF_Private_FunctionPrototypes USBD_AUDIO_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */



/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
//static uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length);

USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS =
{
	MIDI_Init_FS,         // Função para inicializar a interface MIDI
	MIDI_DeInit_FS,       // Função para desinicializar a interface MIDI
	MIDI_DataIn_FS,       // Função para enviar dados MIDI para o host
	MIDI_DataOut_FS,      // Função para processar dados MIDI recebidos do host
	MIDI_Receive_FS,      // Função para receber pacotes MIDI
	MIDI_GetState_FS      // Função para obter o estado da interface MIDI
};
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */
// Definição da interface MIDI


/* Private functions ---------------------------------------------------------*/
/*************************************************************************************************/
/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void USBD_MidiMessage(uint8_t cable, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
	uint8_t txBuff[4];

	txBuff[0] = (cable<<=4) + (byte1>>4);
	txBuff[1] = byte1;
	txBuff[2] = byte2;
	txBuff[3] = byte3;

	MIDI_DataIn_FS(txBuff, 4);
}
/*************************************************************************************************/

/*************************************************************************************************/

/*************************************************************************************************/

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */
// Definições das funções conforme o protótipo
/*************************************************************************************************/
uint8_t MIDI_Init_FS(void)
{
    // Inicializa os endpoints e a comunicação USB
    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t MIDI_DeInit_FS(void)
{
    // Fecha os endpoints e desinicializa os recursos da interface MIDI
    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t MIDI_DataIn_FS(uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        uint16_t next_ptr = (APP_Rx_ptr_in + 1) % APP_RX_DATA_SIZE;

        // Evita sobrescrever dados não transmitidos
        if (next_ptr == APP_Rx_ptr_out)
        {
            // Buffer cheio — descarta dado ou retorna erro
            return USBD_BUSY;
        }

        APP_Rx_Buffer[APP_Rx_ptr_in] = data[i];
        APP_Rx_ptr_in = next_ptr;
    }

    // Inicia transmissão se USB estiver configurado e não estiver transmitindo
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED && USB_Tx_State == USB_TX_READY)
    {
        USBD_MIDI_DataIn(&hUsbDeviceFS, MIDI_IN_EP);
    }

    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t MIDI_DataOut_FS(uint8_t *data, uint16_t length)
{
	uint8_t Ping[] = {0x09, 0x90, 0x00, 0x00};
	uint8_t pingEcho[] = {0x09, 0x90, 0x00, 0x7F};

    if(data == NULL || length == 0)
    {
        return USBD_FAIL;  // Retorna falha se não houver dados ou se o comprimento for inválido
    }

    for(int i = 0; i + 3 < length; i += 4)
    {
        uint8_t cin = data[i] & 0x0F;

        // Ignore SysEx completamente por enquanto
        if (cin >= 0x4 && cin <= 0x7)
            continue;

        // Testa apenas mensagens normais
        switch(cin)
        {
        case 0x8:
        case 0x9:

        	if(memcmp(data, Ping, sizeof(Ping)) == 0)
        	{
				// Envia os dados MIDI via USB
				MIDI_DataIn_FS(pingEcho, sizeof(pingEcho));
        	}

        	break;

        case 0xB:
			uint8_t midi[8] = {
				0x3B,//0x39,
				data[i + 1],
				data[i + 2],
				data[i + 3],
				0x0B,//0x39,
				0xB0,
				data[i + 6]+1,
				data[i + 7]
			};
			MIDI_DataIn_FS(midi, 8);
			break;
        }
    }

    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t MIDI_Receive_FS(uint8_t *data, uint16_t length)
{
	UNUSED(data);
	UNUSED(length);
    // Configura a recepção de dados MIDI de entrada
    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t MIDI_GetState_FS(void)
{
    // Retorna o estado da interface MIDI
    return USBD_OK;
}
/*************************************************************************************************/
/**
  * @}
  */
