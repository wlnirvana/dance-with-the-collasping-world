#include "lcd/lcd.h"
#include <string.h>
#include<gd32v_pjt_include.h>

// unsigned char image[12800];
int gif_offset = 0;

void init_uart0(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    usart_interrupt_enable(USART0, USART_INT_RBNE);
}

int main(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);

    init_uart0();

    Lcd_Init(); // init OLED
    LCD_Clear(WHITE);
    BACK_COLOR = WHITE;

    LEDR(1);
    LEDG(1);
    LEDB(1);

    {
        LCD_ShowStringWithDelay(16, 8, (u8 *)("Hey, Songyu"), BLACK, 100);
        delay_1ms(500);
        LCD_ShowStringWithDelay(16, 24, (u8 *)("my little baby"), BLACK, 100);
        delay_1ms(500);
        LCD_ShowStringWithDelay(16, 48, (u8 *)("Welcome to the w-orld."), BLACK, 100);
        delay_1ms(1000);
        LCD_Clear(WHITE);
        LCD_ShowStringWithDelay(16, 8, (u8 *)("I know it is spi-nning apart now,"), BLACK, 100);
        delay_1ms(500);
        LCD_ShowStringWithDelay(16, 40, (u8 *)("but why not just dance with it?"), BLACK, 100);
        delay_1ms(1000);
        LCD_Clear(WHITE);
        LCD_ShowStringWithDelay(16, 8, (u8 *)("Mom and I will a-lways be with u,"), BLACK, 100);
        delay_1ms(500);
        LCD_ShowStringWithDelay(16, 40, (u8 *)("we promise."), BLACK, 100);
        delay_1ms(1000);
        LCD_Clear(WHITE);
        LCD_ShowStringWithDelay(16, 24, (u8 *)("-- A gift to my 1-month-old girl"), BLACK, 100);
        delay_1ms(1000);

        while (1)
        {
            LCD_ShowGif(0, 0, 159, 79);
            delay_1ms(33);
        }
    }
}

int _put_char(int ch)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET)
    {
    }

    return ch;
}
