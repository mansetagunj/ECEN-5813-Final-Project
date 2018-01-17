#include "MKL25Z4.h"
#include "timer0.h"
#include "uart0.h"
#include "gpio.h"
#include "conversion.h"
#include "memory.h"

#define TPM0_CLKSRC_DISABLE			(0)
#define TPM0_CLKSRC_MCGFLL_PLLBY2	(1)
#define TPM0_CLKSRC_OSCERCLK 		(2)
#define TPM0_CLKSRC_MCGIRCCLK 		(3)

#define TPM0_CLK_GATE_EN			(1)
#define TPM0_CLK_GATE_DIS			(0)

/*
000 Divide by 1
001 Divide by 2
010 Divide by 4
011 Divide by 8
100 Divide by 16
101 Divide by 32
110 Divide by 64
111 Divide by 128
*/
#define TPM0_CLK_PRES_1				(0)
#define TPM0_CLK_PRES_2				(1)
#define TPM0_CLK_PRES_4				(2)
#define TPM0_CLK_PRES_64			(6)
#define TPM0_CLK_PRES_128			(7)




//volatile uint32_t systick1 = 0;
//volatile uint32_t systick2 = 0;
//volatile uint8_t flag = 0;

void timer0_clockInit()
{
	//selecting clk source
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(TPM0_CLKSRC_MCGFLL_PLLBY2);

	//enabling TPM0 clock gate
	SIM->SCGC6 |= SIM_SCGC6_TPM0(TPM0_CLK_GATE_EN);
}

void timer0_configure()
{
	timer0_clockInit();

	TPM0->SC |= TPM_SC_TOIE(1);

	//count up mode
	TPM0->SC |= TPM_SC_CPWMS(0);

	//Prescaler of 64
	TPM0->SC |= TPM_SC_PS(TPM0_CLK_PRES_128);

	//Clock Freq – 47939584 Hz, Timer0 Pres – 64, Time wanted = 50ms
	//Count = (47939584 / 128) * (174/1000);
	TPM0->MOD = 65535;

	//software compare
	TPM0->CONTROLS->CnSC |= TPM_CnSC_MSA(1) | TPM_CnSC_MSB(1);

	//counter inc every LPTPM counter clock
	TPM0->SC |= TPM_SC_CMOD(1);

	NVIC_EnableIRQ(TPM0_IRQn);

}

void TPM0_IRQHandler()
{
	__disable_irq();
	if((TPM0->SC & TPM_SC_TOF_MASK))
	{
		TPM0->SC |= TPM_SC_TOF(1);
		GPIO_Red_Toggle();
	}
	__enable_irq();
}
