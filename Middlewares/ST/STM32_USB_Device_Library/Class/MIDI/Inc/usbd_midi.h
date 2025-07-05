/*
 * usbd_midi.h
 *
 *  Created on: Mar 28, 2025
 *      Author: dress
 */

#ifndef __USBD_MIDI_H
#define __USBD_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_ioreq.h"

// Intervalo de polling para endpoints
#ifndef MIDI_HS_BINTERVAL
#define MIDI_HS_BINTERVAL                            0x01U
#endif /* MIDI_HS_BINTERVAL */

#ifndef MIDI_FS_BINTERVAL
#define MIDI_FS_BINTERVAL                            0x01U
#endif /* MIDI_FS_BINTERVAL */

// Definição do tamanho do buffer MIDI
#define MIDI_PACKET_SIZE 64  // Tamanho padrão para USB Full-Speed

// Definição dos Endpoints MIDI
#ifndef MIDI_IN_EP
#define MIDI_IN_EP  0x81  // Endpoint de entrada (Dispositivo -> Host)
#endif /* MIDI_IN_EP */

#ifndef MIDI_OUT_EP
#define MIDI_OUT_EP 0x01  // Endpoint de saída (Host -> Dispositivo)
#endif /* MIDI_OUT_EP */

#define USB_MIDI_CONFIG_DESC_SIZ                      197//0x65U
#define MIDI_INTERFACE_DESC_SIZE                      0x09U
//#define USB_MIDI_DESC_SIZ                             0x09U
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09U

//#define AUDIO_DESCRIPTOR_TYPE                         0x21U
#define USB_DEVICE_CLASS_AUDIO                        0x01U
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01U
//#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02U
#define AUDIO_SUBCLASS_MIDISTREAMING                  0x03U
#define AUDIO_PROTOCOL_UNDEFINED                      0x00U

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24U
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25U



#define MIDI_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */

#define APP_RX_DATA_SIZE                             MIDI_DATA_FS_MAX_PACKET_SIZE//(MIDI_DATA_FS_MAX_PACKET_SIZE * 32) //buffer to send

// Definição do buffer MIDI
#define MIDI_TOTAL_BUF_SIZE                          MIDI_PACKET_SIZE//(MIDI_PACKET_SIZE * 32)  // Buffer para 32 pacotes MIDI

// Estados de transmissão USB
#define USB_TX_BUSY  1  // USB está ocupado transmitindo
#define USB_TX_READY 0  // USB pronto para nova transferência


#define MIDI_DATA_OUT_PACKET_SIZE                    MIDI_DATA_FS_MAX_PACKET_SIZE

#define MIDI_DATA_IN_PACKET_SIZE                     MIDI_DATA_FS_MAX_PACKET_SIZE


// Estrutura para armazenar dados MIDI e estado do buffer

typedef struct {
    uint8_t buffer[MIDI_TOTAL_BUF_SIZE];  // Buffer para dados MIDI
    uint16_t rd_ptr;                      // Ponteiro de leitura
    uint16_t wr_ptr;                      // Ponteiro de escrita
    uint8_t rd_enable;                     // Flag para indicar leitura ativa
} USBD_MIDI_HandleTypeDef;

// Estrutura que contém as funções da interface MIDI
typedef struct
{
    uint8_t (*Init)(void);               // Função de inicialização
    uint8_t (*DeInit)(void);             // Função de desinicialização
    uint8_t (*DataIn)(uint8_t *data, uint16_t length);  // Função para enviar dados MIDI
    uint8_t (*DataOut)(uint8_t *data, uint16_t length); // Função para processar dados MIDI recebidos
    uint8_t (*Receive)(uint8_t *data, uint16_t length); // Função para receber pacotes MIDI
    uint8_t (*GetState)(void);           // Função para obter o estado da interface MIDI
} USBD_MIDI_ItfTypeDef;

extern uint8_t APP_Rx_Buffer [APP_RX_DATA_SIZE];
extern uint32_t APP_Rx_ptr_in;
extern uint32_t APP_Rx_ptr_out;
extern uint8_t  USB_Tx_State;

// Declaração da classe USB MIDI
extern USBD_ClassTypeDef USBD_MIDI;
//#define USBD_MIDI_CLASS &USBD_MIDI

// Registra uma função de callback para receber dados MIDI
extern uint8_t MIDI_DataOut_FS(uint8_t *data, uint16_t length);
extern USBD_HandleTypeDef hUsbDeviceFS;

void PiscarLedPorCanal(uint8_t cn);

uint8_t USBD_MIDI_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_MIDI_ItfTypeDef *fops);

uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t *USBD_MIDI_GetCfgDesc(uint16_t *length);

// Funções públicas
uint8_t USBD_MIDI_SendPacket(USBD_HandleTypeDef *pdev, uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MIDI_H */
