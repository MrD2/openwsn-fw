// #define CONFIG_DEBUG
// #define CONFIG_DATA_MODEL_SMALL
#define CONFIG_DATA_MODEL_LARGE
// #define CONFIG_LOGGING

// CPU fequency hardcoded limit
#define CONFIG_CPU_CLOCK_LIMIT_KHZ      25000
// Desired CPU frequency
#define CONFIG_CPU_CLOCK_HZ             25000000
// XT1 oscillator frequency
#define CONFIG_XT1_CLOCK_HZ             32768
#define CONFIG_XT2_CLOCK_HZ				( 25000000UL )
// Number of desired FreeRTOS ticks per second
#define CONFIG_FREERTOS_TICK_RATE_HZ    1024
// Number of TimerA0 ticks for one FreeRTOS tick
#define CONFIG_FREERTOS_TICK_COUNT      1
