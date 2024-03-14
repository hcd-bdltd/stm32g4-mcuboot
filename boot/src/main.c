/**
 * @file main.c
 * @author B Duncan LTD (info@bduncanltd.com)
 * @brief Entry point for the bootloader
 * @version 0.1
 * @date 2024-03-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "main.h"

#include "bootutil/bootutil.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/fault_injection_hardening.h"
#include "bootutil/image.h"

static void Init(void);
static void SystemClock_Config(void);
static void DeInit(void);
static void boot_jump(struct boot_rsp *rsp);

int main(void)
{
	// Initialize the system (clocks, peripherals, etc.)
	Init();

	// Blink 10 times to indicate bootloader is running
	for (int i = 0; i < 10; i++) {
		BSP_LED_On(LED2);
		HAL_Delay(50);
		BSP_LED_Off(LED2);
		HAL_Delay(50);
	}

	/* MCUboot's boot_go validates and checks all images for update and returns
	 * the load information for booting the main image
	 */
	BOOT_LOG_INF("Starting bootloader");
	struct boot_rsp rsp;

	FIH_DECLARE(fih_rc, FIH_FAILURE);
	FIH_CALL(boot_go, fih_rc, &rsp);
	if (FIH_NOT_EQ(fih_rc, FIH_SUCCESS)) {
		BOOT_LOG_ERR("Unable to find bootable image");
		Error_Handler();
	}

	BOOT_LOG_INF("Jumping to the first image slot");
	BOOT_LOG_INF("Bootloader chainload address offset: 0x%x", (int)rsp.br_image_off);

	// De-initialize the system (clocks, peripherals, etc.)
	// No more logging or blinking LEDs after this point
	DeInit();

	// Jump to the application
	boot_jump(&rsp);

	// Should never get here
	FIH_PANIC;
	return 0;
}

static void Init(void)
{
	HAL_Init();
	SystemClock_Config();

	COM_InitTypeDef COM_Init = {
		.BaudRate = 115200,
		.WordLength = COM_WORDLENGTH_8B,
		.StopBits = COM_STOPBITS_1,
		.Parity = COM_PARITY_NONE,
		.HwFlowCtl = COM_HWCONTROL_NONE,
	};
	BSP_COM_Init(COM1, &COM_Init);
	BSP_LED_Init(LED2);
}

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};

	/* Enable voltage range 1 boost mode for frequency above 150 Mhz */
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
	__HAL_RCC_PWR_CLK_DISABLE();

	/* Activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	   clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
				       RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_8) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}
}

static void DeInit(void)
{
	BSP_LED_DeInit(LED2);
	BSP_COM_DeInit(COM1);

	HAL_RCC_DeInit();
	HAL_DeInit();
}

static void boot_jump(struct boot_rsp *rsp)
{
	uintptr_t flash_base = FLASH_BASE;
	void *start = (void *)(flash_base + rsp->br_image_off + rsp->br_hdr->ih_hdr_size);

	/* Lock interrupts and dive into the entry point */
	__disable_irq();
	((void (*)(void))start)();
}

void Error_Handler(void)
{
	const char *SOS_PATTERN = "...---...";
	const int DOT_DURATION = 100;
	const int DASH_DURATION = 3 * DOT_DURATION;
	const int PAUSE_DURATION = 3 * DOT_DURATION;
	const int WORD_PAUSE_DURATION = 7 * DOT_DURATION;

	while (1) {
		const char *c = SOS_PATTERN;
		BSP_LED_Off(LED2);
		while (*c) {
			if (*c == '.') {
				BSP_LED_On(LED2);
				HAL_Delay(DOT_DURATION);
			} else if (*c == '-') {
				BSP_LED_On(LED2);
				HAL_Delay(DASH_DURATION);
			}
			BSP_LED_Off(LED2);
			HAL_Delay(PAUSE_DURATION);
			c++;
		}
		HAL_Delay(WORD_PAUSE_DURATION);
	}
}
