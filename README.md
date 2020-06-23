# stm32uartdemo
stm32f103 + freeRTOS + uart ll driver loopback test

- baudrate    : 115200
- word length : 8 Bits
- parity      : None
- Stop Bits   : 1

#### first,enable rx interrupt at main user code 2:

```
/* USER CODE BEGIN 2 */
LL_USART_EnableIT_RXNE(USART1);   
/* USER CODE END 2 */
  ```
  
#### second, enqueue received data at USART1_IRQHandler
```
uint8_t data;
osStatus_t status;
if(LL_USART_IsActiveFlag_RXNE(USART1))
{
	data = LL_USART_ReceiveData8(USART1);
	osMessageQueuePut(uartRxQueueHandle,&data,0U,0U);
}
```
#### third, dequeue received data at task and enqueue it to tx queue, then enable tx interrupt
````
uint8_t data;
osStatus_t status;
for(;;)
{
	status = osMessageQueueGet(uartRxQueueHandle, &data, NULL, 0U);
	if (status == osOK)
	{
		osMessageQueuePut(uartTxQueueHandle,&data,NULL,0U);
		LL_USART_EnableIT_TXE(USART1);
	}
	osDelay(1);
}
````
#### last, dequeue send data at USART1_IRQHandler and transmit it, if tx queue is empty, disable tx interrupt
````
if(LL_USART_IsActiveFlag_TXE(USART1))
{
	status = osMessageQueueGet(uartTxQueueHandle, &data, NULL, 0U);
	if (status == osOK)
	{
		LL_USART_TransmitData8(USART1,data);
	}
	else				
	{
		LL_USART_DisableIT_TXE(USART1);
	}			
}
````