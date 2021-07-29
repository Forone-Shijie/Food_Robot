/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_USART1_UART_Init(void);

static void MX_USART2_UART_Init(void);

static void MX_USART3_UART_Init(void);

static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define RXBUFFERSIZE  256     //最大接收字节数
char RxBuffer[RXBUFFERSIZE];   //接收数据
uint8_t aRxBuffer;            //接收中断缓冲
uint8_t Uart3_Rx_Cnt = 0;        //接收缓冲计数
uint8_t USART3_RX_BUF[100];
uint8_t Rxdata = 0;

uint8_t Servo_data[13] = {0xFF, 0xFF, 0x01, 0x09, 0x03, 0x2A, 0x00, 0x08, 0x00, 0x00, 0xD0, 0x07, 0xE9};//2048
void Cal_com_data_STS(uint8_t id, int position, int speed) {
    uint8_t check_data;
    uint8_t sum;

    Servo_data[2] = id;
    Servo_data[6] = position & 255;
    Servo_data[7] = position >> 8;

    Servo_data[10] = speed & 255;
    Servo_data[11] = speed >> 8;

    for (int i = 2; i < 12; i++) {
        sum += Servo_data[i];
    }

    check_data = ((sum & 255) ^ 255);
    Servo_data[12] = check_data;
}

void Cal_com_data_SCS(uint8_t id, int position, int speed) {
    uint8_t check_data;
    uint8_t sum;

    Servo_data[2] = id;
    Servo_data[7] = position & 255;
    Servo_data[6] = position >> 8;

    Servo_data[11] = speed & 255;
    Servo_data[10] = speed >> 8;

    for (int i = 2; i < 12; i++) {
        sum += Servo_data[i];
    }

    check_data = ((sum & 255) ^ 255);
    Servo_data[12] = check_data;
}


//3rd_Servos all up
void Servo_action1() {
    Cal_com_data_STS(1, 2700, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(2, 2700, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(3, 2700, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(4, 2700, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
}

//3rd_Servos all down
void Servo_action2() {
    Cal_com_data_STS(1, 2048, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(2, 2048, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(3, 2048, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    Cal_com_data_STS(4, 2048, 3400);
//    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
}

//3rd_Servos rolls
void Servo_action3() {
    Cal_com_data_STS(1, 2048, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(2, 2281, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(3, 2700, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(4, 2356, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    HAL_Delay(300);

    Cal_com_data_STS(1, 2048, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(2, 2690, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(3, 2700, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(4, 2045, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    HAL_Delay(300);

    Cal_com_data_STS(1, 2665, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(2, 2700, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(3, 2066, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(4, 2045, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    HAL_Delay(300);

    Cal_com_data_STS(1, 2501, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(2, 2051, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(3, 2066, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(4, 2497, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    HAL_Delay(300);

    Cal_com_data_STS(1, 2048, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(2, 2281, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(3, 2700, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

    Cal_com_data_STS(4, 2356, 3400);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);

}

//1st_Servos_actions  主料
void Servo_action4() {
    Cal_com_data_STS(5, 2645, 1000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(1000);

    Cal_com_data_STS(1, 3150, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(500);
    Cal_com_data_STS(1, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(2, 3200, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(500);
    Cal_com_data_STS(2, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(3, 3100, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(500);
    Cal_com_data_STS(3, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(5, 3118, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(500);

    Cal_com_data_STS(4, 3150, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(500);

    Cal_com_data_STS(4, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_STS(5, 2048, 1000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

}

//2nd_Servos_actions  辅料
void Servo_action5() {
    Cal_com_data_STS(1, 2630, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(500);

    Cal_com_data_SCS(2, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(2, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(2, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(2, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(3, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(3, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(3, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(3, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(4, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(4, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(4, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(4, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(5, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(5, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(5, 950, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);
    Cal_com_data_SCS(5, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(500);


    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);


    Cal_com_data_STS(1, 2048, 1000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(300);

}

void set_up() {
    //颠锅机构初始化
    Cal_com_data_STS(1, 2048, 2000);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(2, 2048, 2000);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(3, 2048, 2000);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(4, 2048, 2000);
    HAL_UART_Transmit(&huart1, Servo_data, 13, 100);
    HAL_Delay(100);

    //主料机构初始化
    Cal_com_data_STS(1, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(2, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(3, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(4, 2048, 2000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_STS(5, 2048, 1000);
    HAL_UART_Transmit(&huart3, Servo_data, 13, 100);
    HAL_Delay(100);

    //辅料机构初始化
    Cal_com_data_STS(1, 2048, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(2, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(3, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(4, 100, 2000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);

    Cal_com_data_SCS(5, 100, 1000);
    HAL_UART_Transmit(&huart6, Servo_data, 13, 100);
    HAL_Delay(100);


}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    RetargetInit(&huart1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *) &aRxBuffer, 1);
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_USART6_UART_Init();
    /* USER CODE BEGIN 2 */

    //FF FF 01 09 03 2A 00 08 00 00 D0 07 E9	2048
    //uint8_t data[14]={0xFF,0xFF,0x01,0x0A,0x03,0x29,0x32,0xD8,0x0E,0x00,0x00,0xD0,0x07,0xD9};//3800
    //HAL_UART_Transmit(&huart1, insruction(1, 50, 3000, 1000), 14, 100);



    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_UART_Receive_IT(&huart2, USART3_RX_BUF, 1);
    set_up();
    //HAL_UART_Transmit(&huart1, USART3_RX_BUF, sizeof(USART3_RX_BUF), 100);

    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
//        Servo_action1();
//        HAL_Delay(2000);
//        Servo_action2();
//        HAL_Delay(2000);
//        Servo_action3();
//        HAL_Delay(2000);

        while (Rxdata) {
            if (Rxdata == 3) {
                //颠锅机构
                Servo_action1();
                HAL_Delay(1000);
                Servo_action2();
                HAL_Delay(500);
                Servo_action1();
                HAL_Delay(1000);
                Servo_action2();//上下上下归位

                HAL_Delay(300);

                Servo_action3();
                HAL_Delay(500);
                Servo_action3();
                Servo_action2();//翻滚归位

                Rxdata = 0;
            }
            if (Rxdata == 1) {
                //主料机构
                Servo_action4();
                Rxdata = 0;
            }

            if (Rxdata == 2) {
                //辅料机构
                Servo_action4();
                Rxdata = 0;
            }
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void) {

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void) {

    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void) {

    /* USER CODE BEGIN USART6_Init 0 */

    /* USER CODE END USART6_Init 0 */

    /* USER CODE BEGIN USART6_Init 1 */

    /* USER CODE END USART6_Init 1 */
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 115200;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart6) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART6_Init 2 */

    /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_RESET);

    /*Configure GPIO pins : PF9 PF10 */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : PA4 PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PB1 PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == USART2) {
//        Rxdata = RxBuffer[0];
//        printf("%02x", RxBuffer[0]);
        //HAL_UART_Transmit(&huart1, USART3_RX_BUF, 1, 100);//串口1发送接收buff里的东西
        printf("%x", USART3_RX_BUF[0]);
        Rxdata = USART3_RX_BUF[0];
        HAL_UART_Receive_IT(&huart2, USART3_RX_BUF, 1);    //重新开启串口3接收中断
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
