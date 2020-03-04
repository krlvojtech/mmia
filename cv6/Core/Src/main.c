/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main body of DS18B20 and NTC
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "sct.h"
#include "1wire.h"
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
ADC_HandleTypeDef hadc;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// round to closest integer
uint16_t round_closest(uint16_t dividend, uint16_t divisor)
{
    return (dividend + (divisor / 2)) / divisor;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int16_t temp_18b20;
  int16_t adc_val;
  //temperature values based on adc values 0..1023, saturated to min -50 and max 120 degrees celsius
  //multiplied by ten for accuracy
  //used from my friend
  static const int16_t temp_val[] = {-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,
		  -500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-500,-484,
		  -417,-352,-290,-229,-170,-113,-58,-5,47,96,145,191,236,280,322,363,402,440,476,512,546,578,610,
		  640,670,698,725,751,777,801,824,846,868,888,908,927,945,963,979,995,1010,1025,1039,1052,1065,
		  1077,1088,1099,1110,1120,1129,1138,1146,1154,1162,1169,1176,1182,1188,1193,1199,1203,1208,1212,
		  1216,1220,1223,1226,1229,1232,1234,1236,1238,1239,1241,1242,1243,1244,1245,1245,1246,1246,1246,
		  1246,1246,1246,1245,1245,1244,1243,1243,1242,1241,1240,1238,1237,1236,1235,1233,1232,1230,1229,
		  1227,1225,1224,1222,1220,1218,1217,1215,1213,1211,1209,1207,1205,1203,1201,1199,1197,1195,1193,
		  1191,1189,1187,1185,1183,1181,1179,1177,1175,1173,1171,1169,1167,1165,1163,1162,1160,1158,1156,
		  1154,1152,1150,1148,1147,1145,1143,1141,1139,1138,1136,1134,1132,1131,1129,1127,1126,1124,1122,
		  1121,1119,1117,1116,1114,1113,1111,1109,1108,1106,1105,1103,1102,1100,1099,1097,1096,1095,1093,
		  1092,1090,1089,1087,1086,1085,1083,1082,1080,1079,1078,1076,1075,1074,1072,1071,1070,1068,1067,
		  1066,1064,1063,1062,1060,1059,1058,1056,1055,1054,1052,1051,1050,1048,1047,1046,1045,1043,1042,
		  1041,1039,1038,1037,1035,1034,1033,1031,1030,1029,1028,1026,1025,1024,1022,1021,1020,1018,1017,
		  1016,1014,1013,1012,1010,1009,1008,1006,1005,1003,1002,1001,999,998,997,995,994,993,991,990,989,
		  987,986,984,983,982,980,979,978,976,975,973,972,971,969,968,966,965,964,962,961,960,958,957,955,
		  954,953,951,950,948,947,946,944,943,942,940,939,938,936,935,933,932,931,929,928,927,925,924,923,
		  921,920,919,917,916,915,913,912,911,909,908,907,905,904,903,902,900,899,898,896,895,894,893,891,
		  890,889,888,886,885,884,883,881,880,879,878,877,875,874,873,872,871,869,868,867,866,865,863,862,
		  861,860,859,858,857,855,854,853,852,851,850,849,848,846,845,844,843,842,841,840,839,838,836,835,
		  834,833,832,831,830,829,828,827,826,825,824,822,821,820,819,818,817,816,815,814,813,812,811,810,
		  809,808,807,805,804,803,802,801,800,799,798,797,796,795,794,793,792,791,790,788,787,786,785,784,
		  783,782,781,780,779,778,777,775,774,773,772,771,770,769,768,767,765,764,763,762,761,760,759,758,
		  756,755,754,753,752,751,750,748,747,746,745,744,742,741,740,739,738,737,735,734,733,732,731,729,
		  728,727,726,724,723,722,721,719,718,717,716,714,713,712,711,709,708,707,706,704,703,702,701,699,
		  698,697,695,694,693,692,690,689,688,686,685,684,682,681,680,678,677,676,675,673,672,671,669,668,
		  667,665,664,663,661,660,659,657,656,655,653,652,651,650,648,647,646,644,643,642,640,639,638,636,
		  635,634,633,631,630,629,627,626,625,623,622,621,620,618,617,616,614,613,612,611,609,608,607,606,
		  604,603,602,601,599,598,597,595,594,593,592,591,589,588,587,586,584,583,582,581,579,578,577,576,
		  575,573,572,571,570,568,567,566,565,564,562,561,560,559,558,556,555,554,553,551,550,549,548,547,
		  545,544,543,542,541,539,538,537,536,534,533,532,531,530,528,527,526,525,523,522,521,519,518,517,
		  516,514,513,512,510,509,508,507,505,504,503,501,500,499,497,496,494,493,492,490,489,487,486,485,
		  483,482,480,479,477,476,474,473,471,470,468,467,465,464,462,460,459,457,456,454,452,451,449,447,
		  446,444,442,441,439,437,436,434,432,430,428,427,425,423,421,419,417,416,414,412,410,408,406,404,
		  402,400,398,396,394,392,390,388,386,384,382,380,378,376,374,372,370,368,366,364,361,359,357,355,
		  353,351,348,346,344,342,340,337,335,333,331,329,326,324,322,320,317,315,313,311,308,306,304,301,
		  299,297,295,292,290,288,285,283,281,278,276,274,271,269,267,264,262,260,257,255,252,250,248,245,
		  243,240,238,235,233,230,228,225,223,220,218,215,213,210,207,205,202,199,197,194,191,188,185,183,
		  180,177,174,171,167,164,161,158,154,151,148,144,140,137,133,129,125,121,117,113,108,104,99,95,
		  90,85,80,75,69,64,58,52,46,40,33,27,20,13,6,-2,-10,-18,-26,-34,-43,-52,-62,-71,-81,-92,-102,
		  -113,-125,-137,-149,-162,-175,-188,-202,-216,-231,-247,-263,-279,-296,-314,-332,-351,-370,};
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
  MX_USART2_UART_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc); //Calibration
  HAL_ADC_Start(&hadc); //ADC Start
  OWInit(); //init 1-wire
  sct_init(); //init display
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //Enum defined and init state
	  static enum { SHOW_DS18B20, SHOW_NTC } state = SHOW_DS18B20;

	  //Pressed button S1 then show DS18B20
	  if (HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin) == 0) {
		  state = SHOW_DS18B20;
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(LED1_GPIO_Port	, LED1_Pin, GPIO_PIN_RESET);
	  }
	  //Pressed button S2 then show NTC
	  else if (HAL_GPIO_ReadPin(S2_GPIO_Port, S2_Pin) == 0) {
		  state = SHOW_NTC;
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	  }

	  //DS18B20 and NTC selection and calculating
	  switch(state)
	  {
	  case SHOW_DS18B20:
		  OWConvertAll();
		  HAL_Delay(CONVERT_T_DELAY);
		  OWReadTemperature(&temp_18b20);
		  sct_value(round_closest(abs(temp_18b20), 10), 0); // round to nearest integer
		  break;

	  case SHOW_NTC:
		  adc_val = HAL_ADC_GetValue(&hadc);
		  sct_value(abs(temp_val[adc_val]) ,0);
		  break;

	  default:
		  continue;
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_10B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED2_Pin|SCT_NOE_Pin|SCT_CLK_Pin|SCT_SDI_Pin 
                          |SCT_NLA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : S2_Pin S1_Pin */
  GPIO_InitStruct.Pin = S2_Pin|S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2_Pin SCT_NOE_Pin SCT_CLK_Pin SCT_SDI_Pin 
                           SCT_NLA_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|SCT_NOE_Pin|SCT_CLK_Pin|SCT_SDI_Pin 
                          |SCT_NLA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DQ_Pin */
  GPIO_InitStruct.Pin = DQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DQ_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
