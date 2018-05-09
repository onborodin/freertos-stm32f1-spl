
/* $Id$ */

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <stm32f10x_can.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
#include <misc.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define STR_LEN 86

static QueueHandle_t uart_txq;

void delay(void) {
    volatile uint32_t i;
    for (i = 0; i != 0x70000; i++);
}

void usart_putc(uint8_t data) {
    while (!(USART1->SR & USART_SR_TC));
    USART1->DR = data;
}

void usart_puts(uint8_t * str) {
    uint32_t i = 0;
    while (str[i] != 0) {
        usart_putc(str[i]);
        i++;
    }
}

void RCC_Setup(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}


void USART1_Setup(void) {

    USART_InitTypeDef usart1;
    GPIO_InitTypeDef gpioa;


    gpioa.GPIO_Pin = GPIO_Pin_9;
    gpioa.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa);

    gpioa.GPIO_Pin = GPIO_Pin_10;
    gpioa.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa);

    USART_Cmd(USART1, ENABLE);
    usart1.USART_BaudRate = 115200;
    usart1.USART_WordLength = USART_WordLength_8b;
    usart1.USART_StopBits = USART_StopBits_1;
    usart1.USART_Parity = USART_Parity_No;
    usart1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart1);

    NVIC_InitTypeDef nvic;

    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    NVIC_EnableIRQ(USART1_IRQn);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1);
        USART_SendData(USART1, data);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

static void hello0_task(void *args __attribute__ ((unused))) {
    while(1) {
        usart_puts("Hello, World!\r\n");
    }
}


static void usart_task(void *args __attribute__ ((unused))) {
    uint8_t c;
    while(1) {
        if (xQueueReceive(uart_txq, &c, 10) == pdPASS) {
            usart_putc(c);
        } else {
            taskYIELD();
        }
    }
}

static void hello_task(void *args __attribute__ ((unused))) {
    while(1) {
        uint8_t str[] = "Hello, World!\r\n";
        uint16_t i = 0;
        while (str[i] != 0) {
            xQueueSend(uart_txq, &str[i], portMAX_DELAY);
            i++;
        }
        taskYIELD();
    }
}

int main(void) {
    RCC_Setup();
    USART1_Setup();

    uint32_t i = 0;

    uart_txq = xQueueCreate(1024, sizeof(uint8_t));
    xTaskCreate(hello_task, "SENDER", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL);
    xTaskCreate(usart_task, "USART", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL);

    vTaskStartScheduler();

    while (1) {
        delay();
        i++;
    }
}
/* EOF */
