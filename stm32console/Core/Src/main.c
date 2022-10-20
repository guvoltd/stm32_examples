/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "socket.h"
#include "sntp.h"
#include "w5500/w5500.h"
#include "mb.h"
#include "mbproto.h"
#include "mbutils.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// ============================================= DHCP MODE ENABLE =============
//#define USER_MODE_DHCP
#ifdef  USER_MODE_DHCP
	#include "dhcp.h"
	#include "dns.h"
#endif
#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2
#define SNTP_SOCKET     2

#define MBTCP_PORT      		502

#define REG_INPUT_START       	0x0001
#define REG_INPUT_NREGS       	8

#define REG_HOLDING_START     	0x0001
#define REG_HOLDING_NREGS     	30

#define REG_COILS_START       	0x0001
#define REG_COILS_SIZE        	16

#define REG_DISCRETE_START    	0x0001
#define REG_DISCRETE_SIZE     	16

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t dhcp_buffer[1024];
uint8_t dns_buffer[200];
uint8_t ntp_buf[48];
volatile bool ip_assigned = false;

uint8_t ntp_domain_serv[] = "pool.ntp.org";
uint8_t ntp_ip_serv[4] = {162, 159, 200, 1};
datetime ntp_time;
volatile bool time_assigned = false;

//uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x1000,0x1001,0x1002,0x1003,0x1004,0x1005,0x1006,0x1007};
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {01,11,22,33,44,55,66,77, 88, 99};
//uint8_t ucRegCoilsBuf[REG_COILS_SIZE/8] = {0xaa,0xfe};
//uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE/8] = {0x98, 0x6e};

char msg[60];
uint16_t read_data[20];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void reg_ntp_cbfunc(void(*time_assign)(void));

void W5500_Select(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}

#ifdef USER_MODE_DHCP
void Callback_IPAssigned(void) {
    printf("Callback: IP assigned! Leased time: %ld sec\r\n", getDHCPLeasetime());
    ip_assigned = true;
}
#endif

void Callback_IPConflict(void) {
    printf("Callback: IP conflict!\r\n");
}

void Callback_TimeAssigned(void) {
    printf("Callback: Time assigned! \r\n");
    time_assigned = true;
    printf("NTP Time: %d.%d.%d %d:%d:%d \r\n",ntp_time.dd, ntp_time.mm, ntp_time.yy, ntp_time.hh, ntp_time.mm,ntp_time.ss);
}


// Reset W5500 module
void resetW5500(void)
{
  printf("%s >> Resetting Wiz W5500 Ethernet Board...\n", __func__);
//  pinMode(RESET_P, OUTPUT);
//  digitalWrite(RESET_P, HIGH);
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(250);
  //digitalWrite(RESET_P, LOW);
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(50);
  //digitalWrite(RESET_P, HIGH);
  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(350);
  printf("%s >> Done.\n", __func__);
}

void initW5500() {
    printf("\r\ninit() called!\r\n");

    printf("Registering W5500 callbacks...\r\n");
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);

    printf("Calling wizchip_init()...\r\n");
    uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    printf("Calling STATIC_init()...\r\n");
    wiz_NetInfo net_info = {
        .mac  = { 0xEA, 0x11, 0x22, 0x33, 0x44, 0xAA },
#ifdef USER_MODE_DHCP
		.dhcp = NETINFO_DHCP,
    };

    // set MAC address before using DHCP
    setSHAR(net_info.mac);
    DHCP_init(DHCP_SOCKET, dhcp_buffer);

    printf("Registering DHCP callbacks...\r\n");
    reg_dhcp_cbfunc(
        Callback_IPAssigned,
        Callback_IPAssigned,
        Callback_IPConflict
    );

    printf("Calling DHCP_run()...\r\n");
    // actually should be called in a loop, e.g. by timer
    uint32_t ctr = 10000;
    while((!ip_assigned) && (ctr > 0)) {
        DHCP_run();
        ctr--;
    }
    if(!ip_assigned) {
        printf("\r\nIP was not assigned :(\r\n");
        return;
    }

    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);

    uint8_t dns[4];
    getDNSfromDHCP(dns);

    printf("IP:  %d.%d.%d.%d\r\nGW:  %d.%d.%d.%d\r\nNet: %d.%d.%d.%d\r\nDNS: %d.%d.%d.%d\r\n",
        net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
        net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
        net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3],
        dns[0], dns[1], dns[2], dns[3]
    );

    printf("Calling wizchip_setnetinfo()...\r\n");
	wizchip_setnetinfo(&net_info);

	//printf("Calling DNS_init()...\r\n");
	//DNS_init(DNS_SOCKET, dns_buffer);

	{
		char domain_name[] = "www.google.com";
		printf("Resolving domain name \"%s\"...\r\n", domain_name);
		int8_t res = DNS_run(dns, (uint8_t*)&domain_name, addr);
		if(res != 1) {
			printf("DNS_run() failed, res = %d", res);
			return;
		}
		printf("Result: %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
	}
#else
		.dhcp = NETINFO_STATIC,
		.ip	= { 192, 168, 1, 25},
		.sn = { 255, 255, 255, 0},
		.gw	= { 192, 168, 1, 1},
		.dns = {8, 8, 8, 8},
	};
    printf("IP:  %d.%d.%d.%d\r\nGW:  %d.%d.%d.%d\r\nNet: %d.%d.%d.%d\r\n",
         net_info.ip[0], net_info.ip[1], net_info.ip[2], net_info.ip[3],
         net_info.gw[0], net_info.gw[1], net_info.gw[2], net_info.gw[3],
         net_info.sn[0], net_info.sn[1], net_info.sn[2], net_info.sn[3]
     );
    printf("Calling wizchip_setnetinfo()...\r\n");
    wizchip_setnetinfo(&net_info);
#endif

}

void getNtpServerTimeStamp(void)
{

    printf("Creating socket...\r\n");
    reg_ntp_cbfunc( Callback_TimeAssigned );

    SNTP_init(SNTP_SOCKET, ntp_ip_serv, 28, ntp_buf);
	uint32_t ctr_t = 100000;
	while ((!time_assigned) && (ctr_t > 0)) {
	  SNTP_run(&ntp_time);
	  ctr_t--;
	}
	if (!time_assigned) {
	  printf("\r\nTime was not assigned :(\r\n");
	  return;
	}
//
}

void loop() {
    HAL_Delay(1000);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	eMBErrorCode eMBStatus;
	uint8_t u8FirstBoot = 1;
	//uint8_t rcvBuf[20], bufSize[] = {2, 2, 2, 2, 2, 2, 2, 2};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  	resetW5500();
	initW5500();
	HAL_Delay(200);

	eMBStatus = eMBTCPInit(MBTCP_PORT);
	printf("%s MB TCP Init completed [%s]..\n", __func__, ((eMBStatus == MB_EPORTERR)?"porting layer error":"MB TCP Init Success"));
	HAL_Delay(200);

	eMBStatus = eMBEnable();
	printf("%s MB TCP Init completed [%s]..\n", __func__, ((eMBStatus == MB_EILLSTATE)?"protocol stack in illegal state":"protocol stack Success"));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	getNtpServerTimeStamp();
	HAL_Delay(1000);
	printf("\r\nModbus-TCP Start!\r\n");

  while (1)
  {
      modbus_tcps(HTTP_SOCKET, MBTCP_PORT);
      //HAL_Delay(500);
      if(u8FirstBoot == 1)
      {
    	  // Put TCP Init LED on state...
    	  HAL_GPIO_WritePin(TCP_MB_INIT_GPIO_Port, TCP_MB_INIT_Pin, GPIO_PIN_SET);
    	  u8FirstBoot = 0;
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
  eMBErrorCode eStatus = MB_ENOERR;
  int iRegIndex;

  if ((usAddress >= REG_HOLDING_START) &&
      (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
  {
    iRegIndex = (int)(usAddress - REG_HOLDING_START);
    switch (eMode)
    {
    /* Pass current register values to the protocol stack. */
    case MB_REG_READ:
      while (usNRegs > 0)
      {
        *pucRegBuffer++ =
            (unsigned char)(usRegHoldingBuf[iRegIndex] >> 8);
        *pucRegBuffer++ =
            (unsigned char)(usRegHoldingBuf[iRegIndex] & 0xFF);
        iRegIndex++;
        usNRegs--;
      }
      break;
    /* Update current register values with new values from the protocol stack. */
    case MB_REG_WRITE:
      while (usNRegs > 0)
      {
        usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
        usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
        iRegIndex++;
        usNRegs--;
      }
    }
  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

//eMBErrorCode
//eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
//{
//  eMBErrorCode eStatus = MB_ENOERR;
//  int iRegIndex;
//
//  if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
//  {
//    iRegIndex = (int)(usAddress - REG_INPUT_START);
//    while (usNRegs > 0)
//    {
//      *pucRegBuffer++ =
//          (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
//      *pucRegBuffer++ =
//          (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
//      iRegIndex++;
//      usNRegs--;
//    }
//  }
//  else
//  {
//    eStatus = MB_ENOREG;
//  }
//
//  return eStatus;
//}
//
//eMBErrorCode
//eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
//              eMBRegisterMode eMode)
//{
//  eMBErrorCode eStatus = MB_ENOERR;
//  int iNCoils = (int)usNCoils;
//  unsigned short usBitOffset;
//
//  /* Check if we have registers mapped at this block. */
//  if ((usAddress >= REG_COILS_START) &&
//      (usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE))
//  {
//    usBitOffset = (unsigned short)(usAddress - REG_COILS_START);
//    switch (eMode)
//    {
//    /* Read current values and pass to protocol stack. */
//    case MB_REG_READ:
//      while (iNCoils > 0)
//      {
//        *pucRegBuffer++ =
//            xMBUtilGetBits(ucRegCoilsBuf, usBitOffset,
//                           (unsigned char)(iNCoils >
//                                                   8
//                                               ? 8
//                                               : iNCoils));
//        iNCoils -= 8;
//        usBitOffset += 8;
//      }
//      break;
//      /* Update current register values. */
//    case MB_REG_WRITE:
//      while (iNCoils > 0)
//      {
//        xMBUtilSetBits(ucRegCoilsBuf, usBitOffset,
//                       (unsigned char)(iNCoils > 8 ? 8 : iNCoils),
//                       *pucRegBuffer++);
//        iNCoils -= 8;
//        usBitOffset += 8;
//      }
//      break;
//    }
//  }
//  else
//  {
//    eStatus = MB_ENOREG;
//  }
//  return eStatus;
//}
//
//eMBErrorCode
//eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
//{
//  eMBErrorCode eStatus = MB_ENOERR;
//  short iNDiscrete = (short)usNDiscrete;
//  unsigned short usBitOffset;
//
//  /* Check if we have registers mapped at this block. */
//  if ((usAddress >= REG_DISCRETE_START) &&
//      (usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE))
//  {
//    usBitOffset = (unsigned short)(usAddress - REG_DISCRETE_START);
//    while (iNDiscrete > 0)
//    {
//      *pucRegBuffer++ =
//          xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
//                                    ( unsigned char )( iNDiscrete >
//                                                       8 ? 8 : iNDiscrete ) );
//      iNDiscrete -= 8;
//      usBitOffset += 8;
//    }
//  }
//  else
//  {
//    eStatus = MB_ENOREG;
//  }
//  return eStatus;
//}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
