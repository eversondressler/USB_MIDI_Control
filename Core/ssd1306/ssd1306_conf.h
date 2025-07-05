/*
 * Arquivo de configuração privado para a biblioteca SSD1306.
 * Este exemplo está configurado para STM32F0, I2C e incluindo todas as fontes.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

/* Escolha uma família de microcontroladores */

//#define STM32F0
//#define STM32F1
#define STM32F4
//#define STM32L0
//#define STM32L1
//#define STM32L4
//#define STM32F3
//#define STM32H7
//#define STM32F7
//#define STM32G0


/* Escolha um barramento */

#define SSD1306_USE_I2C
//#define SSD1306_USE_SPI


/* Configuração I2C */

#define SSD1306_I2C_PORT	hi2c1
#define SSD1306_I2C_ADDR	(0x3C << 1)


/* Configuração SPI */

//#define SSD1306_SPI_PORT        hspi1
//#define SSD1306_CS_Port         OLED_CS_GPIO_Port
//#define SSD1306_CS_Pin          OLED_CS_Pin
//#define SSD1306_DC_Port         OLED_DC_GPIO_Port
//#define SSD1306_DC_Pin          OLED_DC_Pin
//#define SSD1306_Reset_Port      OLED_Res_GPIO_Port
//#define SSD1306_Reset_Pin       OLED_Res_Pin


/* Espelhe a tela se necessário */

//#define SSD1306_MIRROR_VERT
//#define SSD1306_MIRROR_HORIZ


/* Defina a cor inversa se necessário */

//#define SSD1306_INVERSE_COLOR


/* Incluir apenas fontes necessárias */

#define SSD1306_INCLUDE_FONT_6x8
//#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
//#define SSD1306_INCLUDE_FONT_16x15
//#define SSD1306_INCLUDE_FONT_16x24
//#define SSD1306_INCLUDE_FONT_16x26


/* A largura da tela pode ser definida usando este define. O valor padrão é 128 */

//#define SSD1306_WIDTH	64


/* Se o eixo horizontal da sua tela não começar na coluna 0, você pode usar esta definição para ajustar o deslocamento horizontal */

//#define SSD1306_X_OFFSET


// A altura também pode ser alterada se necessário. Pode ser 32, 64 ou 128. O valor padrão é 64 */

//#define SSD1306_HEIGHT	64


#endif /* __SSD1306_CONF_H__ */
