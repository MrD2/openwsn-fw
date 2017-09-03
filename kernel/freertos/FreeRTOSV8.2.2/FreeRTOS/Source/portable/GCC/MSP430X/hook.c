/* ---FreeRTOS includes--- */
#include "FreeRTOS.h"
#include "task.h"

#include "leds.h"

/* ---FUNTION PROTOTYPES--- */
void vApplicationTickHook( void );
void vApplicationSetupTimerInterrupt( void );
void vApplicationIdleHook( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, signed char *pcTaskName );
void vApplicationSetupTimerInterrupt( void );


/**********************************************************************//**
 *	@brief Called everytime that FreeRTOS fails to get memory from the
 *			microcontroller
 *	
 *	@return none
 *
 *	Applocation Malloc Failed Hook is responsible for dealing with the
 *	problem when FreeRTOS can't get the memory requested by the execution
 *	thread. Instead, here the orange led is blinked fast so that the user
 *	may know that memory is unsufficient.
 ************************************************************************/
void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	   free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	   internally by FreeRTOS API functions that create tasks, queues or
	   semaphores. */
	taskDISABLE_INTERRUPTS();
	while(1){
		leds_sync_toggle();
		__delay_cycles(2500000);
	}
}

/**********************************************************************//**
 *	@brief Called everytime that FreeRTOS has no more tasks to run
 *	
 *	@return none
 *
 *	When Application Idle function is called the FreeRTOS places the
 *	microcontroler in a Lowish power mode. This function has the lowest
 *	priority in the FreeRTOS schedule.
 ************************************************************************/
void vApplicationIdleHook( void )
{
	/* Called on each iteration of the idle task.  In this case the idle task
	   just enters a low(ish) power mode. */
	__bis_SR_register( LPM0_bits + GIE );
}

/**********************************************************************//**
 *	@brief Called everytime a FreeRTOS tick occurs
 *	
 *	@return none
 *
 *	Application Tick Hook function is called everytime a FreeRTOS tick Occurs.
 *	Here we blink the green led to signal that the FreeRTOS kernel is running
 *	under normal circunstances.
 ************************************************************************/
void vApplicationTickHook( void )
{
	static unsigned long ulCounter = 0;

	/* Is it time to toggle the LED again? */
	ulCounter++;

	/* Just periodically toggle an LED to show that the tick interrupt is
	   running.  Note that this access LED_PORT_OUT in a non-atomic way, so tasks
	   that access the same port must do so from a critical section. */
	if( ( ulCounter == 0x3E8) )//0xff== 0 ) //0x3E8 is 1sec if 1000 tick rate
	{
		ulCounter = 0;
		leds_radio_toggle();
	}
}

/**********************************************************************//**
 *	@brief Detects if a Stack Overflow occurred
 *	
 *	@param[in]	pxTask		Task Handle
 *	@param[in]	pcTaskName	Name of Task
 *	@return none
 *
 *	Application Stack Overflow function is responsible for dealing with problems
 *	when FreeRTOS detects that an overflow occured. Here we just blink leds
 *	to notify user that an event triggered a Stack Overflow.
 ************************************************************************/
void vApplicationStackOverflowHook( TaskHandle_t pxTask, signed char *pcTaskName ){
	( void ) pxTask;
	( void ) pcTaskName;

	/* Run time stack overflow checking is performed if
	   configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	   function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	while(1){
		leds_error_toggle();
		__delay_cycles(2500000);
	}
}

// The MSP430X port uses this callback function to configure its tick interrupt.
// This allows the application to choose the tick interrupt source.
// configTICK_VECTOR must also be set in FreeRTOSConfig.h to the correct
// interrupt vector for the chosen tick interrupt source.  This implementation of
// vApplicationSetupTimerInterrupt() generates the tick from timer A0, so in this
// case configTICK_VECTOR is set to TIMER0_A0_VECTOR.
void vApplicationSetupTimerInterrupt( void ){
    TA1CTL = 0;
	__delay_cycles(50);

	/* Count continuously using ACLK clearing the initial counter. */
	TA1CTL = TASSEL_1 | MC_1 | TACLR;

	/* Set the compare match value according to the tick rate we want. */
	TA1CCR0 = 32768 / configTICK_RATE_HZ;

	/* Enable the interrupts. */
	TA1CCTL0 = CCIE;
}