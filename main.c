#include <stdint.h>

#define RCC_AHB1ENR   (*(unsigned long*) (0x40023800+0x30))
#define RCC_RC        (*(unsigned long*) (0x40023800))
#define GPIOB_ODR     (*(unsigned long*) (0x40020400+0x14))
#define GPIOB_MODER   (*(unsigned long*) (0x40020400))

#define IWDG_BASE   0x40003000

// IWDG registers
#define IWDG_KR_OFFSET      0x00
#define IWDG_PR_OFFSET      0x04
#define IWDG_RLR_OFFSET     0x08

#define IWDG_KEY_ENABLE     0x5555
#define IWDG_KEY_REFRESH    0xAAAA
#define IWDG_KEY_START      0xCCCC

void IWDG_Init(void);
void IWDG_Refresh(void);

int main(void)
{
  
  RCC_RC |= (1<<0);
  RCC_AHB1ENR |= (1 << 1);
  
  IWDG_Init();
  
  GPIOB_MODER |= (1 << (5 * 2));
  GPIOB_ODR |= (1 << 5);
  for(int i =0; i <1000000; i++);
  GPIOB_ODR &= ~(1 << 5);
  

  while (1)
  {
    // Feed the watchdog timer to prevent reset
    //IWDG_Refresh();
  }
}


void IWDG_Init(void)
{
  // Enable write access to IWDG_PR, IWDG_RLR registers
  *((volatile uint32_t *)(IWDG_BASE + IWDG_KR_OFFSET)) = IWDG_KEY_ENABLE;

  // Set the prescaler and reload values for a 2-second timeout
  *((volatile uint32_t *)(IWDG_BASE + IWDG_PR_OFFSET)) = 4;    // Prescaler: 64
  *((volatile uint32_t *)(IWDG_BASE + IWDG_RLR_OFFSET)) = 1000; // Reload value for 2-second timeout

  // Start the watchdog timer
  *((volatile uint32_t *)(IWDG_BASE + IWDG_KR_OFFSET)) = IWDG_KEY_START;
}

void IWDG_Refresh(void)
{
  // Feed the watchdog timer
  *((volatile uint32_t *)(IWDG_BASE + IWDG_KR_OFFSET)) = IWDG_KEY_REFRESH;
}


