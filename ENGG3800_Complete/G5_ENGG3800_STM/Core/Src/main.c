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
#include "flash_checker.h"
#include "touch_checker.h"
#include "connectivity_status.h"
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
I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart1;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void configure_mux(float volt);
void init_ADC(int channel);
void writeTXecho(void);
void getACWave(int sampleCount, acWave* currWave);
uint32_t flash_write(uint32_t pageAddressStart, char* data, uint16_t numOfWords);
void flash_read(uint32_t pageAddressStart, uint16_t numOfWords);
void flash_checker(int check);
void renderBackground(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//Global variables
#define MAXBRIGHTNESS 5
#define ADCSAMPLES 400 //ADC sample SIze
#define ADC1115_ADDRESS 0x48 //ADC address
#define BUFFERSIZE 1 // Buffer size set at 1 to read 1 character at a time
int BRIGHTNESSLEVEL = 3; // Initiate Brightness level at 3 (Middle value)
int HOLDSTATUS = 0; // 0 -> Hold Off, 1 -> Hold On
int MODESTATUS = 0; // 0-> DC, 1-> AC, 2-> Continuity, 3 -> Resistance
int RESETFLAG = 0; // could we combine the reset flag and zero flag?--------------
int ZEROFLAG = 0;
int resMode = 0;
float voltageArray[ADCSAMPLES] = {0};
char uniqueMsg[16] = {0};

//int16_t ADC_read; //ADC read ---------------------doesnt need to be global----------------
float voltage[4]; // Initiate ADC Voltage Float array
const float voltageConversion = (6.114 / 32768.0);
char voltage1[16];
uint8_t recvX[2]; // doesn't need to be global----------------------------------------------
uint8_t recvY[2]; // doesn't need to be global----------------------------------------------
uint16_t dataX = 0; // X component data point required for touch screen functionality
uint16_t dataY = 0;// Y component data point required for touch screen functionality
uint8_t rxBuffer[BUFFERSIZE]; //initiate the receiving end buffer
uint8_t uartMessage[50]; // create string to store the messages as received from the GUI
int connectflash; // flashes the led
volatile uint8_t flag; // Flag variable
volatile uint8_t cont_flag; // Flag variable
volatile uint8_t background_flag = 0; // Flag variable
volatile uint8_t message_flag = 0; // Flag variable
//char* xCoord[32];
//char* yCoord[32];
char* string[32]; // this is not being used -----------------------------------------------------------
uint16_t xPixel = 0;
uint16_t yPixel = 0;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    //MX_DMA_Init(); // Might need this because screen breaks if DMA isn't initialized before spi
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */
    //Initialize the following GPIO pins
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // pin 9 high
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    ILI9163_init(1); // Flipped landscape
    init_ADC(0); //Initialize the ADC
    TIM1->CCR1 = (65535 / (MAXBRIGHTNESS)*BRIGHTNESSLEVEL);// split the brightness level of the bg into different levels
    time1 = HAL_GetTick();//store the values of the tick at different points
    time2 = HAL_GetTick();//store the values of the tick at different points
    Connected = 0; //initialize the the connected variable at zero for not connected
    connectflash = 0; //initialize the flash variable to flicker the connected symbol
    cont_flag = 1; //initialize flag
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_UART_Receive_IT(&huart1, rxBuffer, BUFFERSIZE);
    renderBackground();
    acWave currWave = { -99, 99, 0, 0, 0, 0, 0 }; //----------------- should we change this?---------------
    int breakpoint = 0; // do we need this for final? ---------------------------------------------------
    int sampleCount = 0;
    int lastRender = HAL_GetTick();//store the time of the last render
    int lastRead = HAL_GetTick();//store the time of the last read
    int init_flag = 1;

    flash_read(0x0803F800, 3); // read value from flash memory
    if ((atoi(flashBuffer) >= 0) && (atoi(flashBuffer) <= 200)) {
        continuity = atoi(flashBuffer); // if valid value, set as continuity
    }

    while (1) {

        // used to blink the connection indicator
        if (connectflash == 0) {
            connectflash = 1;
        } else if (connectflash == 1) {
            connectflash = 0;
        }

        // Handles the HOLD functionality
        if (!HOLDSTATUS) {
            if ((HAL_GetTick() - lastRead >= 5)) {
                read_ADC(sampleCount);
                configure_mux((voltageArray[sampleCount] / 11.00 + 1.77));
                getACWave(sampleCount, &currWave);
                sampleCount++;
                lastRead = HAL_GetTick();
            }
            if (sampleCount == ADCSAMPLES) {
                sampleCount = 0;
            }
        }

        if (sampleCount == 0) {
            currWave.DC = voltageArray[ADCSAMPLES - 1];
        } else {
            currWave.DC = voltageArray[sampleCount - 1];
        }

        // Handle button presses on the touch screen
        if (flag == 1) {
            flag = 0;
            dataX = recieveXdata();
            dataY = recieveYdata();
            dataX = dataX >> 3;
            dataY = dataY >> 3;
            xPixel = round((160 * dataY) / 3847 - 5); //Screen calibration equations
            yPixel = round(128 - (128 * dataX) / 3847);
            touch_checker();
        }

        // handles the connectivity to the PC
        connectivity_status();

        // render the background
        if (background_flag) {
            background_flag = 0;
            renderBackground();
        }

        // render initially and then every 200ms
        if ((init_flag == 1) || (HAL_GetTick() - lastRender >= 200)) {
            init_flag = 0;
            yourRenderFunction(&currWave, sampleCount);
            lastRender = HAL_GetTick();
            if (message_flag && Connected) { // displays the message over any underlying display
                renderMessage(uniqueMsg);
            }
        }
        breakpoint++; // do we still need this? --------------------------------------------------
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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
  */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x10909CEC;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Analogue filter
  */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Digital filter
  */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef sConfigOC = { 0 };
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 65535;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 0;
    sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter = 0;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
    HAL_TIM_MspPostInit(&htim1);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_13 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PC15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PA4 PA5 PA11 PA14
                           PA15 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_14
        | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PB1 PB13 PB4 PB5
                           PB8 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_13 | GPIO_PIN_4 | GPIO_PIN_5
        | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */


void configure_mux(float volt) {
	//static int resMode = 1;
	if (MODESTATUS == 0 || MODESTATUS == 1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // A = 1, B = 1, C = 0
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // red probe

//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	} if (MODESTATUS == 2 || MODESTATUS == 3) {
//		if (resMode == 2) {
//			resMode = 1;
//		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);

		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //MULTIPLEX not working

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //RELAY



		if (resMode == 1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //last relay
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //2nd last relay

//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if (volt > 1.82) {
				resMode = 0;
			} else if (volt < 0.065) { // Try 0.05812 cutoff at 22 ohm
				resMode = 2;
			}
		} else if (resMode == 2) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // A = 0, B = 0, C = 0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//			//
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if (volt > 1.5) {	//Try 1.8 cutoff at 20 ohm
                resMode = 1;
            }
        } else if (!resMode) {
			//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			if (volt < 0.16) {
				resMode = 1;
			}
		}
	}
}

uint32_t flash_write(uint32_t pageAddressStart, char* data, uint16_t numOfWords)
{
    static FLASH_EraseInitTypeDef eraseStruct;
    uint32_t pageError;
    int currWord = 0;

    HAL_FLASH_Unlock();

    eraseStruct.Banks = FLASH_BANK_1;
    eraseStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseStruct.Page = 127;
    eraseStruct.NbPages = 1;

    if (HAL_FLASHEx_Erase(&eraseStruct, &pageError) != HAL_OK) {
        /*Error occurred while page erase.*/
        return HAL_FLASH_GetError();
    }
    /* Programming the FLASH page */
    while (currWord < numOfWords) { // Currently using double word unsure if will work

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, pageAddressStart, (uint32_t)data[currWord]) == HAL_OK) {
            pageAddressStart += 8; // use StartPageAddress += 2 for half word and 8 for double word
            currWord++;
        } else {
            /* Error occurred while writing data in Flash memory*/
            return HAL_FLASH_GetError();
        }
    }

    HAL_FLASH_Lock();
    return 0;
}

void flash_read(uint32_t pageAddressStart, uint16_t numOfWords)
{
    int index = 0;
    while (index < numOfWords) {
        flashBuffer[index] = (char)(*(__IO uint32_t*)pageAddressStart);
        pageAddressStart += 8;
        index++;
    }
}

void touch_callback()
{
    if (HAL_GetTick() - time1 > 200) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        flag = 1;
    }
    time1 = HAL_GetTick();
}

void init_ADC(int channel)
{
    unsigned char ADCwrite[6]; //adc write char
    ADCwrite[0] = 0x11;
    ADCwrite[1] = 0x80;
    ADCwrite[2] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 3, 100);

    ADCwrite[0] = 0x10;
    ADCwrite[1] = 0x00;
    ADCwrite[2] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 3, 100);

    if (channel == 0) {
        ADCwrite[0] = 0x01;
        ADCwrite[1] = 0xC0;
        ADCwrite[2] = 0xE4;
        HAL_I2C_Master_Transmit(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 3, 100);
    } else if (channel == 2) {
        ADCwrite[0] = 0x01;
        ADCwrite[1] = 0xE0;
        ADCwrite[2] = 0xE4;
        HAL_I2C_Master_Transmit(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 3, 100);
    }
}

void read_ADC(int sampleCount)
{
	unsigned char ADCwrite[6]; //adc write char
	int16_t ADC_read;
	ADCwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 1, 100);
	HAL_Delay(1);

	HAL_I2C_Master_Receive(&hi2c1, ADC1115_ADDRESS << 1, ADCwrite, 2, 100);
	ADC_read = (ADCwrite[0] << 8 | ADCwrite[1]);
	if(ADC_read < 0){
		ADC_read = 0;
	}
	voltage[0] = ADC_read * voltageConversion /*(1100/100)*/;


//		for (int j = 1; j < ADCSAMPLES; j++) {
//			voltageArray[j - 1] = voltageArray[j];
//		}

	voltageArray[sampleCount] = ((ADC_read * voltageConversion) - 1.731) * (11.00); // could be 1.725
		//voltage[i] = 220 * (1/(2.7/(ADC_read* voltageConversion))-1);
	//voltage[0] = ((ADC_read * voltageConversion)-1.8)*(1100/100)		//DC voltage conversion
	voltage[2] = (ADC_read * voltageConversion);
//		voltage[2] = 220 * (1/(2.7/(ADC_read* voltageConversion))-1);	//resistance conversion
}

void getACWave(int sampleCount, acWave* currWave)
{
	float rmsVal = 0;
	float voltageSum = 0;
	float max = 0;
	float min = 0;
	if (!(sampleCount % 10)) {
		for (int i = 0; i < ADCSAMPLES; i++) {
			if (voltageArray[i] > max) {
				max = voltageArray[i];
			}
			if (voltageArray[i] < min) {
				min = voltageArray[i];
			}
		}

		currWave->offset = (max + min)/2;
		//currWave->offset = voltageSum / ADCSAMPLES;
		for (int i = 0; i < ADCSAMPLES; i++) {
			rmsVal += pow(voltageArray[i] - currWave->offset, 2);
		}
		rmsVal /= 400.0;
		currWave->rms = sqrt(rmsVal);
	}


	if (MODESTATUS == 1) {
		if (currWave->rms > currWave->max) {
			currWave->max = currWave->rms;
		}
		if (currWave->rms < currWave->min) {
			currWave->min = currWave->rms;
		}
	} else {
		if (currWave->DC > currWave->max) {
			currWave->max = currWave->DC;
		}
		if (currWave->DC < currWave->min) {
			currWave->min = currWave->DC;
		}
	}

	float resVoltage = voltageArray[sampleCount]/11.00 + 1.73; //Calibrate to Vref
	if (resMode == 0) {
		currWave->res = 750000 * (1/((1.7/resVoltage)-1)); //Think best advice is 7500000 for the big ones and 1k for small

	} else if (resMode == 1) {
		currWave->res = 1200 * (1/((1.75/resVoltage)-1)); //Think best advice is 7500000 for the big ones and 1k for small

	} else {
		currWave->res = 10 * (1/((1.7/resVoltage)-1)); //Think best advice is 7500000 for the big ones and 1k for small
	}

	//currWave->res = resVoltage;

	if (ZEROFLAG) {
		ZEROFLAG = 0;
		currWave->zero = currWave->res;
	}
	currWave->res = currWave->res - currWave->zero;

	if (RESETFLAG) {
		RESETFLAG = 0;
		currWave->max = -99;
		currWave->min = 99;
	}
}
//used to receive the X-data component value from the resistive touch screen
uint16_t recieveXdata()
{
    uint8_t startX = 0xD0;
    uint8_t recvCoordsX[2] = { 0, 0 };
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&startX, sizeof(startX), HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t*)&recvCoordsX, sizeof(recvCoordsX), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, SET);

    recvX[0] = recvCoordsX[0];
    recvX[1] = recvCoordsX[1];
    //return the recorded value
    return (recvX[0] << 0x08 | recvX[1]);
}

uint16_t recieveYdata()
{
    uint8_t startY = 0x90;
    uint8_t recvCoordsY[2] = { 0, 0 };
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&startY, sizeof(startY), HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t*)&recvCoordsY, sizeof(recvCoordsY), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, SET);

    recvY[0] = recvCoordsY[0];
    recvY[1] = recvCoordsY[1];

    return (recvY[0] << 0x08 | recvY[1]);
}

//render background function is used to update and redraw the background when called
void renderBackground()
{
	ILI9163_newFrame(); // Call this to clear the frame buffer out
	ILI9163_fillRect(0, 0, 160, 128, DARKGRAY);
	ILI9163_drawString(10, 3, Font_11x18, WHITE, "Multimeter");

	// Vertical bar graph (dependent on brightness)
	int minx = 10;
	int maxx = 20;
	int miny = 100;
	int maxy = 110;

	ILI9163_fillRect(minx, miny-56, maxx, maxy-56, WHITE);
	ILI9163_fillRect(minx, miny-42, maxx, maxy-42, WHITE);
	ILI9163_fillRect(minx, miny-28, maxx, maxy-28, WHITE);
	ILI9163_fillRect(minx, miny-14, maxx, maxy-14, WHITE);
	ILI9163_fillRect(minx, miny, maxx, maxy, YELLOW);

	if (BRIGHTNESSLEVEL == 5) {
		ILI9163_fillRect(minx, miny-56, maxx, maxy-56, YELLOW);
		ILI9163_fillRect(minx, miny-42, maxx, maxy-42, YELLOW);
		ILI9163_fillRect(minx, miny-28, maxx, maxy-28, YELLOW);
		ILI9163_fillRect(minx, miny-14, maxx, maxy-14, YELLOW);
		ILI9163_fillRect(minx, miny, maxx, maxy, YELLOW);
	} else if (BRIGHTNESSLEVEL == 4) {
		ILI9163_fillRect(minx, miny-42, maxx, maxy-42, YELLOW);
		ILI9163_fillRect(minx, miny-28, maxx, maxy-28, YELLOW);
		ILI9163_fillRect(minx, miny-14, maxx, maxy-14, YELLOW);
		ILI9163_fillRect(minx, miny, maxx, maxy, YELLOW);
	}else if (BRIGHTNESSLEVEL == 3) {
		ILI9163_fillRect(minx, miny-28, maxx, maxy-28, YELLOW);
		ILI9163_fillRect(minx, miny-14, maxx, maxy-14, YELLOW);
		ILI9163_fillRect(minx, miny, maxx, maxy, YELLOW);
	}else if (BRIGHTNESSLEVEL == 2) {
		ILI9163_fillRect(minx, miny-14, maxx, maxy-14, YELLOW);
		ILI9163_fillRect(minx, miny, maxx, maxy, YELLOW);
	}
	// Custom draw triangle functions (ILI9163)
	ILI9163_drawTriangleUP(15,maxy-83, WHITE); 		// Up Arrow
	ILI9163_drawTriangleDOWN(15,maxy+15, WHITE); 	// Down Arrow

	// Reset

	ILI9163_fillCircle(145, 15, 15, RED);
	if (MODESTATUS == 0 || MODESTATUS == 1) {
		ILI9163_drawString(138, 5, Font_7x10, WHITE, "RE");
		ILI9163_drawString(135, 15, Font_7x10, WHITE, "SET");
	} else {
		ILI9163_drawString(138, 5, Font_7x10, WHITE, "ZE");
		ILI9163_drawString(138, 15, Font_7x10, WHITE, "RO");
	}
}

void yourRenderFunction(acWave* acWave, int sampleCount) {

	//Mode Button
	ILI9163_fillRect(35, 94, 83, 122, YELLOW);
	ILI9163_drawString(37, 95, Font_11x18, BLACK, "MODE");
	if (MODESTATUS == 0) {
		ILI9163_drawString(35, 112, Font_7x10, BLACK, "DC");
	} else if (MODESTATUS == 1) {
		ILI9163_drawString(35, 112, Font_7x10, BLACK, "AC");
	} else if (MODESTATUS == 2) {
		ILI9163_drawString(35, 112, Font_7x10, BLACK, "Rstance");
	} else if (MODESTATUS == 3) {
		ILI9163_drawString(35, 112, Font_7x10, BLACK, "Cont");
	}

	// Hold Button
	if (HOLDSTATUS == 0) {
		ILI9163_fillRect(88, 94, 135, 122, RED);
		ILI9163_drawString(90, 112, Font_7x10, WHITE, "Off");
	} else if (HOLDSTATUS == 1) {
		ILI9163_fillRect(88, 94, 135, 122, GREEN);
		ILI9163_drawString(90, 112, Font_7x10, WHITE, "On");
	}
	ILI9163_drawString(90, 95, Font_11x18, WHITE, "HOLD");

	// Display Current Value

	float acVal = acWave->rms;
	float dcVal = (acWave->DC);
	float res = acWave->res;
	int high, diff, low;
	char measVal[8];
	char prefix = ' ';
	if (MODESTATUS == 0) {
		high = (int)dcVal;
		diff = (dcVal -  high)*100;

	} else if (MODESTATUS == 1) {
		high = (int)acVal;
		diff = (acVal -  high)*100;

	} else if (MODESTATUS == 2) {
		if (res >= 1000 && res < 500000) {
			high = (int)(res/1000);
			diff = (res -  (high*1000))/10;
			prefix = 'k';
		} else if (res >= 500000 && res <= 2000000) {
			high = (int)(res/1000000);
			diff = (res -  high)/1000;
			prefix = 'M';
		} else if (res >= 0 && res <= 1000) {
			high = (int)res;
			if (res < 1) {
				diff = (res - high)*1000;
			} else {
				diff = (res - high)*100;
			}
		} else if (res > 2000000 || res < 0) {
			high = 0;
			diff = 0;
		}
	} else if (MODESTATUS == 3){
		high = (int)continuity/10;
		diff = (continuity - (high*10))*10;
	}
	low = abs((int)diff);
	if (MODESTATUS == 0 && dcVal < 0 && dcVal > -1) {
		sprintf(measVal, "-%d.%02d%c", high, low, prefix);
	} else {
		sprintf(measVal, "%02d.%02d%c", high, low, prefix);
	}

	if (MODESTATUS == 0) {
		ILI9163_drawString(70, 50, Font_7x10, WHITE, "Current");
		ILI9163_fillRect(71, 60, 115, 78, WHITE);
		ILI9163_drawString(71, 80, Font_7x10, WHITE, "Volts");
		ILI9163_drawString(72, 65, Font_7x10, BLACK, measVal);
	} else if (MODESTATUS == 1) {
		ILI9163_drawString(70, 50, Font_7x10, WHITE, "Current");
		ILI9163_fillRect(71, 60, 115, 78, WHITE);
		ILI9163_drawString(71, 80, Font_7x10, WHITE, "Vrms");
		ILI9163_drawString(72, 65, Font_7x10, BLACK, measVal);
	} else if (MODESTATUS == 2) {
		ILI9163_fillRect(45, 38, 142, 68, WHITE);
		ILI9163_drawString(92, 70, Font_7x10, WHITE, "?");
		ILI9163_drawString(65, 50, Font_7x10, BLACK, measVal);
	} else if (MODESTATUS == 3) {
		ILI9163_fillRect(35, 38, 85, 58, WHITE);
		ILI9163_drawString(57, 60, Font_7x10, WHITE, "?");
		ILI9163_drawString(38, 45, Font_7x10, BLACK, measVal);
	}

	// Minimum Value
	float min = acWave->min;
	char minVal[6];
	if (MODESTATUS == 0) {
		high = (int)min;
		diff = (min -  high)*100;
	} else if (MODESTATUS == 1) {
		high = (int)min;
		diff = (min -  high)*100;
	} else {
		high = 0;
		diff = 0;
	}
	low = abs((int)diff);
	if (MODESTATUS == 0 && min < 0 && min > -1) {
		sprintf(minVal, "-%d.%02d%c", high, low, prefix);
	} else {
		sprintf(minVal, "%02d.%02d%c", high, low, prefix);
	}

	if (MODESTATUS == 0) {
		ILI9163_fillRect(27, 60, 70, 78, WHITE);
		ILI9163_drawString(27, 50, Font_7x10, WHITE, "MIN");
		ILI9163_drawString(28, 65, Font_7x10, BLACK, minVal);
		ILI9163_drawString(27, 80, Font_7x10, WHITE, "Volts");
	} else if (MODESTATUS == 1) {
		ILI9163_fillRect(27, 60, 70, 78, WHITE);
		ILI9163_drawString(27, 50, Font_7x10, WHITE, "MIN");
		ILI9163_drawString(28, 65, Font_7x10, BLACK, minVal);
		ILI9163_drawString(27, 80, Font_7x10, WHITE, "Vrms");
	}

	// Maximum Value
	float max = acWave->max;
	char maxVal[6];
	if (MODESTATUS == 0) {
		high = (int)max;
		diff = (max -  high)*100;
	} else if (MODESTATUS == 1) {
		high = (int)max;
		diff = (max -  high)*100;
	} else {
		high = 0;
		diff = 0;
	}
	low = abs((int)diff);
	if (MODESTATUS == 0 && max < 0 && max > -1) {
		sprintf(maxVal, "-%d.%02d%c", high, low, prefix);
	} else {
		sprintf(maxVal, "%02d.%02d%c", high, low, prefix);
	}
	int shortFlag = 0;

	if (MODESTATUS == 0) {
		ILI9163_fillRect(116, 60, 158, 78, WHITE);
		ILI9163_drawString(135, 50, Font_7x10, WHITE, "MAX");
		ILI9163_drawString(116, 65, Font_7x10, BLACK, maxVal);
		ILI9163_drawString(118, 80, Font_7x10, WHITE, "Volts");
	} else if (MODESTATUS == 1) {
		ILI9163_fillRect(116, 60, 158, 78, WHITE);
		ILI9163_drawString(135, 50, Font_7x10, WHITE, "MAX");
		ILI9163_drawString(116, 65, Font_7x10, BLACK, maxVal);
		ILI9163_drawString(120, 80, Font_7x10, WHITE, "Vrms");
	}

	if (MODESTATUS == 3) {
		ILI9163_fillRect(100, 38, 150, 58, WHITE);
		if (res > (continuity/10.0)) {
			ILI9163_drawString(105, 45, Font_7x10, BLACK, "Open");
			shortFlag = 1;
		} else {
			ILI9163_drawString(105, 45, Font_7x10, BLACK, "Short");
			shortFlag = 2;
		}

	}

	// Display offset
	float offsetVal = acWave->offset;
	char offset[7];
	high = (int)offsetVal;
	diff = (offsetVal -  high)*100;
	low = abs((int)diff);
	if (offsetVal < 0 && offsetVal > - 1) {
		prefix = '-';
	} else {
		prefix = ' ';
	}
	sprintf(offset, "%c%02d.%02d", prefix, high, low);

	if (MODESTATUS == 0) {
		ILI9163_drawString(30, 30, Font_7x10, WHITE, "DC offset:");
		ILI9163_drawString(100, 30, Font_7x10, WHITE, "N/A");
	} else if (MODESTATUS == 1) {
		ILI9163_drawString(30, 30, Font_7x10, WHITE, "DC offset:");
		ILI9163_fillRect(100, 30, 142, 40, BLACK);
		ILI9163_drawString(100, 30, Font_7x10, WHITE, offset);
	}

	// Connectivity
	if (Connected == 0) {
		if (connectflash == 0) {
			ILI9163_fillRect(143, 95, 157, 110, BLACK);
			ILI9163_fillCircle(150, 120, 4, RED);
			ILI9163_drawString(143, 95, Font_11x18, WHITE, "!");
		} else if (connectflash == 1) {
			ILI9163_fillCircle(150, 120, 4, BLACK);
			ILI9163_drawString(143, 95, Font_11x18, WHITE, "!");
		}

	} else if (Connected == 1) {
		ILI9163_fillRect(143, 95, 157, 110, BLACK);
		if (connectflash == 0) {
			ILI9163_fillCircle(150, 120, 4, GREEN);
			ILI9163_drawString(143, 95, Font_11x18, WHITE, "#");

		} else if (connectflash == 1) {
			ILI9163_fillCircle(150, 120, 4, BLACK);
			ILI9163_drawString(143, 95, Font_11x18, WHITE, "#");
		}
	}

	//-----------------------Used for testing Touch Screen ----------------------------------

//	char* xCoord[32];
//	char* yCoord[32];
//	uint16_t xPixel;
//	uint16_t yPixel;
//
//	xPixel = (160 * dataY)/3847 - 5;
//	yPixel = 128  - (128 * dataX)/3847;
//
//	sprintf(xCoord, "X coord = %d", xPixel);
//	sprintf(yCoord, "Y coord = %d", yPixel);
//	ILI9163_fillRect(0, 0, 160, 128, WHITE);
//	ILI9163_drawString(47, 54, Font_7x10, BLACK, xCoord);
//	ILI9163_drawString(47, 94, Font_7x10, BLACK, yCoord);
//	ILI9163_fillRect(xPixel-2, yPixel-2, xPixel+2, yPixel+2, BLACK);


	// Used for UART
	unsigned char uartString[40] = {0};

	if (HAL_GetTick() > 2000 && sampleCount % 10) {
		if (shortFlag == 1) {
			sprintf(uartString, ":Open:%s:%s:%d:%d:%d:\r\n", minVal, maxVal, BRIGHTNESSLEVEL, MODESTATUS, HOLDSTATUS);
		} else if (shortFlag == 2) {
			sprintf(uartString, ":Short:%s:%s:%d:%d:%d:\r\n", minVal, maxVal, BRIGHTNESSLEVEL, MODESTATUS, HOLDSTATUS);
		} else {

			sprintf(uartString, ":%s:%s:%s:%d:%d:%d:\r\n", measVal, minVal, maxVal, BRIGHTNESSLEVEL, MODESTATUS, HOLDSTATUS);
		}
		HAL_UART_Transmit(&huart1, uartString , sizeof(uartString), 50);
	}

	ILI9163_render(); // This draws the whole frame buffer via DMA at once
}

void renderMessage(char* message) {
	ILI9163_fillRect(0, 0, 125, 20, WHITE);
	ILI9163_drawString(5, 7, Font_7x10, BLACK, message);
	ILI9163_render();

}

void modecontrol(){
	if ((xPixel > 33 && xPixel < 85) && (yPixel > 95 && yPixel < 128)) {
		HOLDSTATUS = 0;
		RESETFLAG = 1;
		if (MODESTATUS != 3) {
			MODESTATUS += 1;
			if (MODESTATUS == 2 || MODESTATUS == 3) {
				init_ADC(2);
			} else if (MODESTATUS == 1) {
				init_ADC(0);
			}
		} else {
			MODESTATUS = 0;
			init_ADC(0);
		}
		background_flag = 1;
	}
}

//function is used to detect when the hold button has been pressed on the LCD resistive touch display
void holdcontrol()
{
    if ((xPixel > 86 && xPixel < 138) && (yPixel > 95 && yPixel < 138)) {
        if (HOLDSTATUS == 1) {
            HOLDSTATUS = 0;
        } else if (HOLDSTATUS == 0) {
            HOLDSTATUS = 1;
        }
        background_flag = 1;
    }
}

//function is used to detect when the reset button has been pressed on the LCD resistive touch display
void resetcontrol()
{
    if ((xPixel > 100 && xPixel < 160) && (yPixel > 0 && yPixel < 60)) {
        if (MODESTATUS == 0 || MODESTATUS == 1) {
            RESETFLAG = 1;
        } else {
            ZEROFLAG = 1;
        }
    }
}

//function is used to detect when the brightness control buttons has been pressed on the LCD resistive touch display
void brightnesscontrol()
{
    // Increase
    if ((xPixel > 0 && xPixel < 40) && (yPixel > 20 && yPixel < 50)) {
        if (BRIGHTNESSLEVEL != 5) {
            BRIGHTNESSLEVEL = BRIGHTNESSLEVEL + 1;
        }
        TIM1->CCR1 = (65535 / (MAXBRIGHTNESS)*BRIGHTNESSLEVEL);
        background_flag = 1;
    }
    // Decrease
    if ((xPixel > 0 && xPixel < 30) && (yPixel > 90 && yPixel < 128)) {

        if (BRIGHTNESSLEVEL != 0) {
            BRIGHTNESSLEVEL = BRIGHTNESSLEVEL - 1;
        }
        TIM1->CCR1 = (65535 / (MAXBRIGHTNESS)*BRIGHTNESSLEVEL);
        background_flag = 1;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    //initiate length and an array in order to store messages from UART inside
    static uint8_t message[50] = { 0 };
    static int length = 0;
    //Use the HAL library to receive data From the UART IR line
    HAL_UART_Receive_IT(&huart1, rxBuffer, BUFFERSIZE);
    //once the end of line characters are detected the message is then handled
    if (rxBuffer[0] == '\r') {
        cont_flag = 1;
        memcpy(uartMessage, message, 100);
        // Extract the first token
        char* token = strtok(message, ":");
        //create char double array to store array of messages
        char* msg[6];
        int i = 0;
        // loop through the string to extract all other tokens
        while (token != NULL) {
            msg[i] = token;
            token = strtok(NULL, ":");
            i++;
        }
        i = 0;

        //check if a message string is sent and if so display the message
        if (msg[5][0] != '-' && msg[5][1] != '-' && msg[5][2] != '-') {
            // check if valid message
        	for (int i =0; i < 3; i++){
        		if ((isalpha(msg[5][i]) != 0) || (isdigit(msg[5][i]) != 0) || (msg[5][i] != '\0') || (strcmp(msg[5][i],""))){
        			message_flag = 1;
        			strcpy(uniqueMsg, msg[5]);
        		}
            }

        }
        //set timer2 variable for connectivity functionality
        time2 = HAL_GetTick();

        //check to see if the string being sent from GUI matches Hold On and if so to then
        //carry out the functionality required
        if (!strcmp(msg[3], "Hold On")) {
            //if Hold is sent from PC change hold status to 1
            HOLDSTATUS = 1;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Hold Off and if so to then
        //carry out the functionality required
        if (!strcmp(msg[3], "Hold Off")) {
            //if Hold is sent from PC change holdstatus to 0
            HOLDSTATUS = 0;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Up and if so to then
        //carry out the functionality required
        if (!strcmp(msg[2], "Up")) {
            if (BRIGHTNESSLEVEL < 5) {
                BRIGHTNESSLEVEL++;
                TIM1->CCR1 = (65535 / (MAXBRIGHTNESS)*BRIGHTNESSLEVEL);
            }
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Down and if so to then
        //carry out the functionality required
        if (!strcmp(msg[2], "Down")) {
            if (BRIGHTNESSLEVEL > 0) {
                BRIGHTNESSLEVEL--;
                TIM1->CCR1 = (65535 / (MAXBRIGHTNESS)*BRIGHTNESSLEVEL);
            }
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Mode 0 and if so to then
        //carry out the functionality required
        if (!strcmp(msg[0], "Mode 0")) {
            init_ADC(0);
            MODESTATUS = 0;
            HOLDSTATUS = 0;
            RESETFLAG = 1;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Mode 1 and if so to then
        //carry out the functionality required
        if (!strcmp(msg[0], "Mode 1")) {
            init_ADC(0);
            MODESTATUS = 1;
            HOLDSTATUS = 0;
            RESETFLAG = 1;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches mode 2 and if so to then
        //carry out the functionality required
        if (!strcmp(msg[0], "Mode 2")) {
            init_ADC(2);
            MODESTATUS = 2;
            HOLDSTATUS = 0;
            RESETFLAG = 1;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches mode 3 and if so to then
        //carry out the functionality required
        if (!strcmp(msg[0], "Mode 3")) {
            init_ADC(2);
            MODESTATUS = 3;
            HOLDSTATUS = 0;
            RESETFLAG = 1;
            background_flag = 1;
        }

        //check to see if the string being sent from GUI matches Reset and if so to then
        //carry out the functionality required
        if (!strcmp(msg[1], "Reset")) {
            RESETFLAG = 1;
        }

        //		continuity = atoi(msg[4]);
        flash_checker(atoi(msg[4])); // check the continuity value from the GUI


        //reset & and clear the message string
        for (int i = 0; i < length; i++) {
            message[i] = 0;
        }
        length = 0;
    } else {
        message[length] = rxBuffer[0];
        length++;
    }
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
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
