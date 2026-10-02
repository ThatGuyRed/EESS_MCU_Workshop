#include <avr/interrupt.h>
#include <avr/io.h>
#include <ssd1306.h>
#include <stdint.h>
#include <stdio.h>

void button_init(void) {
  // PB as input
  DDRB = 0x0;
  // Enable pullup resistors on PORTB
  PORTB = 0xFF;
  // Note: PB 6 and PB 7 will have to be modified if using external clock
  // Read with PINx & bm
}

void timer_init(void) {
  // SLOW timer
  // DIV 1024 prescaler,
  // Overflows at 0.128 ms * 2**8 = 0.032 s
  TCCR0B |= (1 << CS02) | (1 << CS00);
  // Overflow Interrupt
  TIMSK0 = (1 << TOIE0);
}

ISR(TIMER0_OVF_vect) {
  static uint8_t cnt = 0;
  cnt++;
  if (cnt >= 30) {
    PORTB ^= (1 << PB6);
    cnt = 0;
  }
}

int main(void) {

  // Set PB6 as output for testing
  DDRB |= (1 << PB6);

  // Timer init
  cli();
  timer_init();
  sei();

  // Display
  SSD1306_Init(SSD1306_ADDR);

  SSD1306_ClearScreen();
  SSD1306_SetPosition(0, 0);
  SSD1306_DrawString("Tally ho!");
  SSD1306_UpdateScreen(SSD1306_ADDR);
  // To-Do: Track cursor position
  //
  while (1)
    ;
}
