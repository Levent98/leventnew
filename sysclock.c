#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include "sysclock.h"
#define PLL_M 8  //shift to 0
#define PLL_N 336 //shift to 6
#define PLL_P 2 //shift to 16 2 ama 0 yaz 
#define PLL_Q 4 //shift to 24

//STM32F4XX DISCOVERY BOARD

void SystemInitt(void)
{
//Set HSION bit
RCC->CR|=(uint32_t)0x00000001;

//Reset CFGR Register 
RCC->CFGR=0;

//Reset HSEON,CSSON and PLLON bits

//RCC->CR&=(uint32_t)0xFEF6FFFF;

//Reset PLLCFGR register

//RCC->PLLCFGR|=0x24003010;
	
//Reset HSEBYP bit

//RCC->CR&=(uint32_t)0xFFFBFFFF;

//Disable all interrupts

RCC->CIR|=0x00000000;

SysclockConfig();	
	
	
	
}
void SysclockConfig(void){

	
__IO uint32_t StartUpCounter=0, HSEStatus=0;

//Enable HSE//

RCC->CR |=((uint32_t)RCC_CR_HSEON);
	
//Wait till HSE is ready and if timeout is reached exit
	
do{

HSEStatus=RCC->CR&RCC_CR_HSERDY;
StartUpCounter++;
}while ((HSEStatus==0)&&(StartUpCounter!=0x5000));

if(((RCC->CR&RCC_CR_HSERDY))!=0x00)
	{
HSEStatus=(uint32_t)0x01;
	}
else{
HSEStatus=(uint32_t)0x00;

}	
if (HSEStatus==(uint32_t)0x01)
{
	//Select regulator voltage output Scale 1 mode //
//set the power enable clock and voltage regulator
	
RCC->APB1ENR|=RCC_APB1ENR_PWREN;
PWR->CR|=PWR_CR_VOS;
	

//configure main pll
RCC->PLLCFGR=0;
RCC->PLLCFGR|=PLL_M|(PLL_N<<6)|RCC_PLLCFGR_PLLSRC_HSE|(((PLL_P>>1)-1)<<16)|(PLL_Q<<24);
//ENABLE MAIN PLL
RCC->CR|=RCC_CR_PLLON;

while((RCC->CR&RCC_CR_PLLRDY)==0)
	{
	
}

//HCLK=SYSCLK1
RCC->CFGR|=RCC_CFGR_HPRE_DIV1;

//PCLK2=HCLK2
RCC->CFGR|=RCC_CFGR_PPRE2_DIV2;
//PCLK1=HCLK4
RCC->CFGR|=RCC_CFGR_PPRE1_DIV4;

//configure flash prefetch 	
FLASH->ACR|=FLASH_ACR_ICEN|FLASH_ACR_DCEN|FLASH_ACR_PRFTEN|FLASH_ACR_LATENCY_5WS;
//SELECT MAIN PLL AS SYSTEM CLOCK SOURCE 
RCC->CFGR&=(uint32_t)((uint32_t)~(RCC_CFGR_SW));
RCC->CFGR|=RCC_CFGR_SW_PLL;

//Wait till the main PLL is used as system clock source 

while((RCC->CFGR&(uint32_t)RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL)
{
}
}
else 
	{
	//if hse fails to startup the application will have wrong clock configuration . User can add here some codes to deal with this type of error 
		uint16_t a;
		a++;
		
}
SystemCoreClockUpdate();
}



uint32_t HAL_RCC_GetSysClockFreq(void)
{
  uint32_t pllm = 0U, pllvco = 0U, pllp = 0U;
  uint32_t sysclockfreq = 0U;

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
    {
      sysclockfreq = HSI_VALUE;
       break;
    }
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
    {
      sysclockfreq = HSE_VALUE;
      break;
    }
    case RCC_CFGR_SWS_PLL:  /* PLL used as system clock  source */
    {
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
      SYSCLK = PLL_VCO / PLLP */
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      if(__HAL_RCC_GET_PLL_OSCSOURCE() != RCC_PLLSOURCE_HSI)
      {
        /* HSE used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSE_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (uint32_t) ((((uint64_t) HSI_VALUE * ((uint64_t) ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos)))) / (uint64_t)pllm);
      }
      pllp = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U) *2U);

      sysclockfreq = pllvco/pllp;
      break;
    }
    default:
    {
      sysclockfreq = HSI_VALUE;
      break;
    }
  }
  return sysclockfreq;
}
