#include <stdint.h>
#include "SysTick.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#define L_GoN   36   // North Green, East Red, Walk Red
#define L_SlowN 20   // North Yellow, East Red, Walk Red
#define L_SlowNW 70   // North Yellow, East Red, Walk Red
#define L_GoE   9    // East Green, North Red, Walk Red
#define L_SlowE 10   // East Yellow, North Red, Walk Red
#define L_SlowEW 69   // North Yellow, East Red, Walk Red
#define L_WalkE  12   // East Red, North Red, Walk On, go back to E
#define L_WalkN   99  // East Red, North Red, Walk On, go back to N
#define GPIO_PORTF_PIN1_EN 0x02
#define GPIO_PORTF_PIN2_EN 0x04
#define GPIO_PORTF_PIN3_EN 0x08
#define DEFAULT_STATE L_GoN
unsigned state = DEFAULT_STATE;
unsigned delay = 0;

// GPIO_PCTL_PA3_SSI0FSS
// PB1 = SRCLK
// PB2 = RCLK

PortB_Init(){
    SYSCTL_RCGCGPIO_R |= 0x00000002; // initialize clock for port B
    GPIO_PORTB_LOCK_R |= 0x4C4F434B; // unlock gpio port B
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}; // wait until Port B is ready
    GPIO_PORTB_AMSEL_R = 0;
    GPIO_PORTB_PCTL_R = 0;
    GPIO_PORTB_DIR_R &= ~0x58;        // Set PB4, PB3, PB6 as inputs
    GPIO_PORTB_DIR_R |= 0x07;         // Set PB0, PB1, PB2 as outputs
    GPIO_PORTB_DEN_R |= 0x5F;
    GPIO_PORTB_LOCK_R = 0;
    GPIO_PORTB_AFSEL_R = 0;
}

PortF_Init() {
    SYSCTL_RCGCGPIO_R |= 0x20;     //enable clock for PORTF
    GPIO_PORTF_DEN_R  |= GPIO_PORTF_PIN1_EN;    //enable pins 1 on PORTF
    GPIO_PORTF_DIR_R  |= GPIO_PORTF_PIN1_EN;    //make pins 1 as output pins
    GPIO_PORTF_DEN_R  |= GPIO_PORTF_PIN2_EN;    //enable pins 2 on PORTF
    GPIO_PORTF_DIR_R  |= GPIO_PORTF_PIN2_EN;    //make pins 2 as output pins
    GPIO_PORTF_DEN_R  |= GPIO_PORTF_PIN3_EN;    //enable pins 3 on PORTF
    GPIO_PORTF_DIR_R  |= GPIO_PORTF_PIN3_EN;    //make pins 3 as output pins
}


void send_bit(int bit) {
    GPIO_PORTB_DATA_R = bit; // SER port on or off
    GPIO_PORTB_DATA_R |= 0x2; // turn on clock
    GPIO_PORTB_DATA_R &= ~0x2; // turn off clock
    GPIO_PORTB_DATA_R |= 0x4; // RCLK high
    GPIO_PORTB_DATA_R &= ~0x4; // RCLK low
}

void change_state(int a, int b, int c, int d, int e, int f) {
    send_bit(a);
    send_bit(b);
    send_bit(c);
    send_bit(d);
    send_bit(e);
    send_bit(f);

}


void set_binary_light(int num) {
    int a = (num & (1 << 0)) > 0;
    int b = (num & (1 << 1)) > 0;
    int c = (num & (1 << 2)) > 0;
    int d = (num & (1 << 3)) > 0;
    int e = (num & (1 << 4)) > 0;
    int f = (num & (1 << 5)) > 0;

    change_state(a, b, c, d, e, f);
}
 // testing
void run() {
    set_binary_light(L_GoE);
    SysTick_Wait10ms(100);
    set_binary_light(L_SlowE);
    SysTick_Wait10ms(50);
    set_binary_light(L_GoN);
    SysTick_Wait10ms(100);
    set_binary_light(L_SlowN);
    SysTick_Wait10ms(50);
}

struct styp {
    unsigned s;
    unsigned delay_ms;
    void (*transition_functions[8])(unsigned /*delay*/);
};

void GoN(unsigned d) {
    state = L_GoN;
    set_binary_light(L_GoN);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void SlowN(unsigned d) {
    state = L_SlowN;
    set_binary_light(L_SlowN);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void GoE(unsigned d) {
    state = L_GoE;
    set_binary_light(L_GoE);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void SlowE(unsigned d) {
    state = L_SlowE;
    set_binary_light(L_SlowE);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void SlowNW(unsigned d) {
    state = L_SlowNW;
    set_binary_light(L_SlowN);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void SlowEW(unsigned d) {
    state = L_SlowEW;
    set_binary_light(L_SlowE);
    GPIO_PORTF_DATA_R = 0x02;
    SysTick_Wait10ms(d);
}
void GoWalkE(unsigned d) {
    state = L_WalkE;
    GPIO_PORTF_DATA_R = 0x08;    //Turn on GREEN LED
    set_binary_light(12);
    SysTick_Wait10ms(d);
    unsigned i = 10;
    while(i>0) {
        GPIO_PORTF_DATA_R = 0x02;    //Turn on RED LED
        SysTick_Wait10ms(30);
        GPIO_PORTF_DATA_R = 0x00;    //Turn  off LED
        SysTick_Wait10ms(30);
        i--;
    }

}
void GoWalkN(unsigned d) {
    state = L_WalkN;
    GPIO_PORTF_DATA_R = 0x08;    //Turn on GREEN LED
    set_binary_light(12);
    SysTick_Wait10ms(d);
    unsigned i = 10;
    while(i>0) {
        GPIO_PORTF_DATA_R = 0x02;    //Turn on RED LED
        SysTick_Wait10ms(30);
        GPIO_PORTF_DATA_R = 0x00;    //Turn  off LED
        SysTick_Wait10ms(30);
        i--;
    }

}

typedef struct styp styp;
styp fsm[8] = {
                 L_GoN, 100, {GoN, SlowN, GoN, SlowN, SlowNW, SlowNW, SlowNW, SlowNW},
                 L_SlowN, 300, {GoE, GoE, GoE, GoE, GoE, GoE, GoE, GoE},
                 L_GoE, 100, {GoE, GoE, SlowE, SlowE, SlowEW, SlowEW, SlowEW, SlowEW},
                 L_SlowE, 300, {GoN, GoN, GoN, GoN,  GoN, GoN, GoN, GoN},
                 L_WalkN, 300, {GoN, GoE, GoN, GoN, GoN,GoE, GoN, GoE},
                 L_WalkE, 300, {GoE, GoE, GoN, GoN, GoN,GoE, GoN, GoE},
                 L_SlowNW, 150, {GoWalkN, GoWalkN, GoWalkN, GoWalkN, GoWalkN, GoWalkN, GoWalkN, GoWalkN},
                 L_SlowEW, 150, {GoWalkE, GoWalkE, GoWalkE, GoWalkE, GoWalkE, GoWalkE}
  };

unsigned getStateByInput(unsigned currentState) {
    unsigned i = 0;
    while(i<9) {
        if (fsm[i].s == currentState) {
            return i;  // Return index to the matching state
        }
        i++;
    }
    return NULL;  // Return NULL if not found
}

unsigned int sw1_pressed() {
    SysTick_Wait10ms(100);
    unsigned sw1 = (GPIO_PORTB_DATA_R & 0x40) >> 6;
    return sw1;
}

unsigned int sw2_pressed() {
    unsigned sw2 = (GPIO_PORTB_DATA_R & 0x10) >> 4;
    return sw2;
}

unsigned int sw3_pressed() {
    return (GPIO_PORTB_DATA_R & 0x08) >> 3;
}
unsigned get_switch_input() {
  unsigned int sw1 = sw1_pressed();
  unsigned int sw2 = sw2_pressed();
  unsigned int sw3 = sw3_pressed();
  return ((sw1 << 2) | (sw2 << 1) | sw3);
}

void main(void){

  PLL_Init(Bus80MHz);  // set system clock to 80 MHz
  SysTick_Init();
  PortB_Init();
  PortF_Init();

  while(1) {

    unsigned inp = get_switch_input();
    styp current_state = fsm[getStateByInput(state)];
    current_state.transition_functions[inp](current_state.delay_ms);

  }

}

