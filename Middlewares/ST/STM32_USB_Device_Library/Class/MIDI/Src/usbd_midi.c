/*
 * usbd_midi.c
 *
 *  Created on: Mar 29, 2025
 *      Author: dress
 */

#include "usbd_ctlreq.h"
#include "usbd_midi.h"


// Estrutura da classe USB MIDI
USBD_ClassTypeDef USBD_MIDI =
{
  USBD_MIDI_Init,
  USBD_MIDI_DeInit,
  NULL,//  USBD_MIDI_Setup,
  NULL,//  USBD_MIDI_EP0_TxReady,
  NULL,//  USBD_MIDI_EP0_RxReady,
  USBD_MIDI_DataIn,
  USBD_MIDI_DataOut,
  NULL,//  USBD_MIDI_SOF,
  NULL,//  USBD_MIDI_IsoINIncomplete,
  NULL,//  USBD_MIDI_IsoOutIncomplete,
  USBD_MIDI_GetCfgDesc,
  USBD_MIDI_GetCfgDesc,
  USBD_MIDI_GetCfgDesc,
  NULL,//USBD_MIDI_GetDeviceQualifierDesc,
};

uint32_t APP_Rx_ptr_in  = 0;
uint32_t APP_Rx_ptr_out = 0;
uint32_t APP_Rx_length  = 0;
uint8_t  USB_Tx_State = USB_TX_READY;

__ALIGN_BEGIN uint8_t USB_Rx_Buffer[MIDI_DATA_OUT_PACKET_SIZE] __ALIGN_END ;
__ALIGN_BEGIN uint8_t APP_Rx_Buffer[APP_RX_DATA_SIZE] __ALIGN_END;

/*************************************************************************************************/
/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_MIDI_CfgDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{
#ifdef MIDI4X4
		/* Configuration 1 */
	/* Configuration Descriptor  */
	0x09,									/* bLength */
	USB_DESC_TYPE_CONFIGURATION,			/* bDescriptorType */
	LOBYTE(USB_MIDI_CONFIG_DESC_SIZ),		/* wTotalLength LOBYTE */
	HIBYTE(USB_MIDI_CONFIG_DESC_SIZ),		/* wTotalLength HIBYTE */
	0x02,									/* bNumInterfaces */
	0x01,									/* bConfigurationValue */
	0x03,									/* iConfiguration */
	0x80,									/* bmAttributes: Bus Powered according to user configuration */
	USBD_MAX_POWER,							/* bMaxPower (mA) */
	/* 09 byte */

	/* Interface Descriptor 0/0 Audio Control, 0 Endpoints */
	MIDI_INTERFACE_DESC_SIZE,				/* bLength */
	USB_DESC_TYPE_INTERFACE,              	/* bDescriptorType */
	0x00,									/* bInterfaceNumber */
	0x00,									/* bAlternateSetting */
	0x00,									/* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOCONTROL,			/* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,				/* bInterfaceProtocol */
	0x03,									/* iInterface */
	/* 09 byte */

	/* Audio Control Interface Header Descriptor */
	0x09,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x01,									/* bDescriptorSubtype */
	0x00,									/* bcdADC LOBYTE */
	0x01,									/* bcdADC HIBYTE */
	0x09,									/* wTotalLength LOBYTE */
	0x00,									/* wTotalLength HIBYTE */
	0x01,									/* bInCollection */
	0x01,									/* baInterfaceNr */
	/* 09 byte */

	/* Interface Descriptor 1/0 Audio Class, 2 Endpoints */
	0x09,									/* bLength */
	USB_DESC_TYPE_INTERFACE,              	/* bDescriptorType */
	0x01,									/* bInterfaceNumber */
	0x00,									/* bAlternateSetting */
	0x02,									/* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass */
	AUDIO_SUBCLASS_MIDISTREAMING,			/* bInterfaceSubClass */
	0x00,									/* bInterfaceProtocol */
	0x03,									/* iInterface */
	/* 09 byte */

	/* MIDI Streaming Interface Header Descriptor */
	0x07,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x01,									/* bDescriptorSubtype */
	0x00,									/* BcdADC Bytes LOBYTE*/
	0x01,									/* BcdADC Bytes HIBYTE*/
	0x41,									/* wTotalLength Bytes LOBYTE*/
	0x00,									/* wTotalLength Bytes HIBYTE*/
	/* 07 byte */

	/* MIDI In Jack Descriptor */
	0x06,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x02,									/* bDescriptorSubtype */
	0x01,									/* bJackType */
	0x01,									/* bJackID */
	0x03,									/* iJack */
	/* 06 byte */

	/* MIDI In Jack Descriptor */
	0x06,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x02,									/* bDescriptorSubtype */
	0x02,									/* bJackType */
	0x02,									/* bJackID */
	0x03,									/* iJack */
	/* 06 byte */

	/* MIDI Out Jack Descriptor */
	0x09,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x03,									/* bDescriptorSubtype */
	0x01,									/* bJackType */
	0x03,									/* bJackID */
	0x01,									/* bNrInputPins */
	0x02,									/* BaSourceID */
	0x01,									/* BaSourcePin */
	0x03,									/* iJack */
	/* 09 byte */

	/* MIDI Out Jack Descriptor */
	0x09,									/* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      	/* bDescriptorType */
	0x03,									/* bDescriptorSubtype */
	0x02,									/* bJackType */
	0x04,									/* bJackID */
	0x01,									/* bNrInputPins */
	0x01,									/* BaSourceID */
	0x01,									/* BaSourcePin */
	0x03,									/* iJack */
	/* 09 byte */

	/* Endpoint Descriptor 01 1 Out, Bulk, 8 bytes */
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    	/* bLength */
	USB_DESC_TYPE_ENDPOINT,               	/* bDescriptorType */
	MIDI_OUT_EP,							/* bEndpointAddress 1 out endpoint */
	0x02,									/* bmAttributes */
	0x40,									/* wMaxPacketSize in Bytes LOBYTE */
	0x00,									/* wMaxPacketSize in Bytes HIBYTE */
	0x00,									/* bInterval */
	0x00,									/* bRefresh */
	0x00,									/* bSynchAddress */
	/* 09 byte */

	/* Audio Class-Specific Descriptor */
	0x05,									/* bLength */
	AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       	/* bDescriptorType */
	0x01,									/* bDescriptorSubtype */
	0x01,									/* bNumEmbMIDIJack */
	0x01,									/* BaAssocJackID */
	/* 05 byte */

	/* Endpoint Descriptor 81 1 In, Bulk, 8 bytes */
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    	/* bLength */
	USB_DESC_TYPE_ENDPOINT,               	/* bDescriptorType */
	MIDI_IN_EP,							/* bEndpointAddress 81 in endpoint */
	0x02,									/* bmAttributes */
	0x40,									/* wMaxPacketSize in Bytes LOBYTE */
	0x00,									/* wMaxPacketSize in Bytes HIBYTE */
	0x00,									/* bInterval */
	0x00,									/* bRefresh */
	0x00,									/* bSynchAddress */
	/* 09 byte */

	/* Audio Class-Specific Descriptor */
	0x05,									/* bLength */
	AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       	/* bDescriptorType */
	0x01,									/* bDescriptorSubtype */
	0x01,									/* bNumEmbMIDIJack */
	0x03,									/* BaAssocJackID */
	/* 05 byte */
} ;
/*************************************************************************************************/
#endif
#ifndef MIDI4X4
/*************************************************************************************************/
		/* Configuration 1 */
	/* Configuration Descriptor  */
	0x09,   		//    bLength                 9
	USB_DESC_TYPE_CONFIGURATION,   		//    bDescriptorType         2
	LOBYTE(USB_MIDI_CONFIG_DESC_SIZ), 	// wTotalLength LOBYTE
	HIBYTE(USB_MIDI_CONFIG_DESC_SIZ), 	// wTotalLength HIBYTE        0x00f7
	0x02,   		//    bNumInterfaces          4 para inclusão de interface HID
	0x01,   		//    bConfigurationValue     1
	0x01,   		//    iConfiguration          0
	0x80,   		//    bmAttributes         0x80
						//      (Bus Powered)
						//      Remote Wakeup
	USBD_MAX_POWER,   		//    MaxPower              500mA



	//    Interface Descriptor:
	0x09,   		//      bLength                 9
	0x04,   		//      bDescriptorType         4
	0x00,   		//      bInterfaceNumber        0
	0x00,   		//      bAlternateSetting       0
	0x00,   		//      bNumEndpoints           0
	0x01,   		//      bInterfaceClass         1 Audio
	0x01,   		//      bInterfaceSubClass      1 Control Device
	0x00,   		//      bInterfaceProtocol      0
	0x01,   		//      iInterface              0

	//      AudioControl Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x01,   		//        bDescriptorSubtype      1 (HEADER)
	0x00, 0x01, 	//        bcdADC               1.00
	0x09, 0x00, 	//        wTotalLength       0x0009
	0x01,   		//        bInCollection           1
	0x01,   		//        baInterfaceNr(0)        1

	//    Interface Descriptor:
	0x09,   		//      bLength                 9
	0x04,   		//      bDescriptorType         4
	0x01,   		//      bInterfaceNumber        1
	0x00,   		//      bAlternateSetting       0
	0x02,   		//      bNumEndpoints           2
	0x01,    		//      bInterfaceClass         1 Audio
	0x03,   		//      bInterfaceSubClass      3 MIDI Streaming
	0x00,   		//      bInterfaceProtocol      0
	0x01,   		//      iInterface              0

	//      MIDIStreaming Interface Descriptor:
	0x07,   		//        bLength                 7
	0x24,   		//        bDescriptorType        36
	0x01,   		//        bDescriptorSubtype      1 (HEADER)
	0x00, 0x01, 	//        bcdADC               1.00
	0xA1, 0x00, 	//        wTotalLength       0x00a1 = 161

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x01,   		//        bJackID                 1
	0x03,   		//        iJack                   4 USB MIDI

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x05,   		//        bJackID                 5
	0x03,    		//        iJack                   5 MIDI DIN

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x09,   		//        bJackID                 9
	0x03,   		//        iJack                   6 Mackie/HUI

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x0D,			//        bJackID                13
	0x03,   		//        iJack                   7 Editor

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x02,   		//        bJackType               2 External
	0x02,   		//        bJackID                 2
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x02,   		//        bJackType               2 External
	0x06,   		//        bJackID                 6
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x02,   		//        bJackType               2 External
	0x0A,			//        bJackID                10
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x06,   		//        bLength                 6
	0x24,   		//        bDescriptorType        36
	0x02,   		//        bDescriptorSubtype      2 (MIDI_IN_JACK)
	0x02,   		//        bJackType               2 External
	0x0E,   		//        bJackID                14
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x03,   		//        bJackID                 3
	0x01,   		//        bNrInputPins            1
	0x02,    		//        baSourceID( 0)          2
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                   8 USB MIDI

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x07,   		//        bJackID                 7
	0x01,   		//        bNrInputPins            1
	0x06,   		//        baSourceID( 0)          6
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                   9 MIDI DIN

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x0B,   		//        bJackID                11
	0x01,   		//        bNrInputPins            1
	0x0A,   		//        baSourceID( 0)         10
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                  10 Mackie/HUI

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x01,   		//        bJackType               1 Embedded
	0x0F,   		//        bJackID                15
	0x01,   		//        bNrInputPins            1
	0x0E,   		//        baSourceID( 0)         14
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                  11 Editor

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x02,   		//        bJackType               2 External
	0x04,   		//        bJackID                 4
	0x01,   		//        bNrInputPins            1
	0x01,   		//        baSourceID( 0)          1
	0x01,   		//          BaSourcePin( 0)         1
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x02,   		//        bJackType               2 External
	0x08,   		//        bJackID                 8
	0x01,   		//        bNrInputPins            1
	0x05,   		//        baSourceID( 0)          5
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x02,   		//        bJackType               2 External
	0x0C,   		//        bJackID                12
	0x01,   		//        bNrInputPins            1
	0x09,   		//        baSourceID( 0)          9
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                   0

	//      MIDIStreaming Interface Descriptor:
	0x09,   		//        bLength                 9
	0x24,   		//        bDescriptorType        36
	0x03,   		//        bDescriptorSubtype      3 (MIDI_OUT_JACK)
	0x02,   		//        bJackType               2 External
	0x10,   		//        bJackID                16
	0x01,   		//        bNrInputPins            1
	0x0D,   		//        baSourceID( 0)         13
	0x01,   		//        BaSourcePin( 0)         1
	0x03,   		//        iJack                   0

	//      Endpoint Descriptor:
	0x09,   		//        bLength                 9
	0x05,   		//        bDescriptorType         5
	0x01,   		//        bEndpointAddress     0x02  EP 2 OUT
	0x02,   		//        bmAttributes            2
						//          Transfer Type            Bulk
						//          Synch Type               None
						//          Usage Type               Data
	0x40, 0x00, 	//        wMaxPacketSize     0x0040  1x 64 bytes
	0x00,   		//        bInterval               0
	0x00,   		//        bRefresh                0
	0x00,   		//        bSynchAddress           0

	//        MIDIStreaming Endpoint Descriptor:
	0x08,   		//          bLength                 8
	0x25,   		//          bDescriptorType        37
	0x01,   		//          bDescriptorSubtype      1 (GENERAL)
	0x04,   		//          bNumEmbMIDIJack         4
	0x01,   		//          baAssocJackID( 0)       1
	0x05,   		//          baAssocJackID( 1)       5
	0x09,   		//          baAssocJackID( 2)       9
	0x0D,   		//          baAssocJackID( 3)      13

	//      Endpoint Descriptor:
	0x09,   		//        bLength                 9
	0x05,   		//        bDescriptorType         5
	0x81,   		//        bEndpointAddress     0x82  EP 2 IN
	0x02,   		//        bmAttributes            2
						//          Transfer Type            Bulk
						//          Synch Type               None
						//          Usage Type               Data
	0x40, 0x00, 	//        wMaxPacketSize     0x0040  1x 64 bytes
	0x00,   		//        bInterval               0
	0x00,   		//        bRefresh                0
	0x00,   		//        bSynchAddress           0

	//        MIDIStreaming Endpoint Descriptor:
	0x08,   		//          bLength                 8
	0x25,   		//          bDescriptorType        37
	0x01,   		//          bDescriptorSubtype      1 (GENERAL)
	0x04,   		//          bNumEmbMIDIJack         4
	0x03,   		//          baAssocJackID( 0)       3
	0x07,   		//          baAssocJackID( 1)       7
	0x0B,   		//          baAssocJackID( 2)      11
	0x0F,   		//          baAssocJackID( 3)      15     //197
	};
#endif
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	UNUSED(cfgidx);
    // Aloca memória para a interface MIDI antes de qualquer uso
	USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)USBD_malloc(sizeof(USBD_MIDI_HandleTypeDef));

    if (hmidi == NULL)
	{
		return USBD_FAIL;
	}

    /* Inicializar a estrutura com zeros */
    memset(hmidi, 0, sizeof(USBD_MIDI_HandleTypeDef));

    /* Associa a classe MIDI ao dispositivo */
	pdev->pClassDataCmsit[pdev->classId] = (void *)hmidi;

    // Abre os endpoints MIDI
    (void)USBD_LL_OpenEP(pdev, MIDI_IN_EP, USBD_EP_TYPE_BULK, MIDI_DATA_IN_PACKET_SIZE);
    (void)USBD_LL_OpenEP(pdev, MIDI_OUT_EP, USBD_EP_TYPE_BULK, MIDI_DATA_OUT_PACKET_SIZE);

    // Prepara a recepção de dados do host
    (void)USBD_LL_PrepareReceive(pdev, MIDI_OUT_EP, hmidi->buffer, MIDI_DATA_OUT_PACKET_SIZE);

    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Close EP OUT */
  (void)USBD_LL_CloseEP(pdev, MIDI_OUT_EP);
  pdev->ep_out[MIDI_OUT_EP & 0xFU].is_used = 0U;
  pdev->ep_out[MIDI_OUT_EP & 0xFU].bInterval = 0U;

  /* Close EP IN */
  (void)USBD_LL_CloseEP(pdev, MIDI_IN_EP);
  pdev->ep_in[MIDI_IN_EP & 0xFU].is_used = 0U;
  pdev->ep_in[MIDI_IN_EP & 0xFU].bInterval = 0U;

  /* DeInit physical Interface components */
  if (pdev->pClassDataCmsit[pdev->classId] != NULL)
  {
	  memset(pdev->pClassDataCmsit[pdev->classId], 0, sizeof(USBD_MIDI_HandleTypeDef)); // Opcional
	  (void)USBD_free(pdev->pClassDataCmsit[pdev->classId]);
	  pdev->pClassDataCmsit[pdev->classId] = NULL;
  }

  return (uint8_t)USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
/**
 * @brief  Função de transmissão de dados MIDI via USB
 *
 * Esta função é chamada quando o host solicita dados do dispositivo MIDI via USB.
 * Ela verifica se há dados disponíveis no buffer de transmissão e os envia pelo endpoint adequado.
 *
 * @param  pdev  Ponteiro para a estrutura do dispositivo USB.
 * @param  epnum Número do endpoint de transmissão.
 * @return USBD_OK se a transmissão foi bem-sucedida, USBD_FAIL se não há dados para enviar.
 */
uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    if (APP_Rx_ptr_out == APP_Rx_ptr_in)
    {
        // Nada para transmitir
        USB_Tx_State = USB_TX_READY;
        return USBD_FAIL;
    }

    uint16_t USB_Tx_ptr = APP_Rx_ptr_out;
    uint16_t USB_Tx_length;

    if (APP_Rx_ptr_in > APP_Rx_ptr_out)
    {
        USB_Tx_length = APP_Rx_ptr_in - APP_Rx_ptr_out;
    }
    else
    {
        USB_Tx_length = APP_RX_DATA_SIZE - APP_Rx_ptr_out;
    }

    if (USB_Tx_length > MIDI_DATA_IN_PACKET_SIZE)
        USB_Tx_length = MIDI_DATA_IN_PACKET_SIZE;

    USB_Tx_State = USB_TX_BUSY;

    if (USBD_LL_Transmit(pdev, epnum, &APP_Rx_Buffer[USB_Tx_ptr], USB_Tx_length) == USBD_OK)
    {
        // Sucesso, pega o primeiro pacote transmitido
        uint8_t cin_cn = APP_Rx_Buffer[USB_Tx_ptr]; // CN está nos 4 bits menos significativos
        uint8_t cn = cin_cn & 0xF0;

        PiscarLedPorCanal(cn);

        APP_Rx_ptr_out = (APP_Rx_ptr_out + USB_Tx_length) % APP_RX_DATA_SIZE;
        return USBD_OK;
    }

    USB_Tx_State = USB_TX_READY;
    return USBD_FAIL;
}
/*************************************************************************************************/

/*************************************************************************************************/
void PiscarLedPorCanal(uint8_t cn)
{
    switch (cn)
    {
        case 0x00: HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); break; // LED1
//        case 0x1: HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14); break; // LED2
//        case 0x2: HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15); break; // LED3
//        case 0x3: HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);  break; // LED4
        default: break;
    }
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];
    if (hmidi == NULL)
    {
        return USBD_FAIL;
    }

    if (epnum == (MIDI_OUT_EP & 0x0F)) // garante que está tratando apenas o número do endpoint (sem direção)
    {
        /* Tamanho do pacote recebido */
        uint16_t packet_size = USBD_LL_GetRxDataSize(pdev, epnum);

        /* Espaço restante no buffer circular */
        uint16_t space_available = MIDI_TOTAL_BUF_SIZE - hmidi->wr_ptr;
        uint16_t bytes_to_copy = (packet_size > space_available) ? space_available : packet_size;

        /* Processa os dados recebidos */
        if (MIDI_DataOut_FS(&hmidi->buffer[hmidi->wr_ptr], bytes_to_copy) == USBD_OK)
        {
            // Dados processados com sucesso
        }

        /* Atualiza o ponteiro de escrita com wrap-around */
        hmidi->wr_ptr = (hmidi->wr_ptr + bytes_to_copy) % MIDI_TOTAL_BUF_SIZE;

        /* Prepara o próximo recebimento USB */
        uint16_t remaining_space = MIDI_TOTAL_BUF_SIZE - hmidi->wr_ptr;
        USBD_LL_PrepareReceive(pdev, epnum, &hmidi->buffer[hmidi->wr_ptr], remaining_space);
    }

    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
uint8_t USBD_MIDI_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_MIDI_ItfTypeDef *fops)
{
    // Verifica se o ponteiro pdev ou fops é nulo
    if (pdev == NULL || fops == NULL)
    {
        return USBD_FAIL;
    }

    // Atribui as funções MIDI à estrutura interna
    pdev->pClassDataCmsit[pdev->classId] = (void *)fops;

    return USBD_OK;
}
/*************************************************************************************************/

/*************************************************************************************************/
/**
  * @brief  USBD_AUDIO_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MIDI_GetCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MIDI_CfgDesc);

  return USBD_MIDI_CfgDesc;
}
/*************************************************************************************************/


