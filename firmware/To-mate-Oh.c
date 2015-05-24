/* Indent: space, Tabsize: 4, Encoding: UTF-8, Language: C/Eng, Breaks: linux */
/**
 * \file To-mate-Oh.c
 *
 * Contains the firmware to flash the microcontroler on the To-mate-Oh.
 *
 * The program is for a little battery powered timer device that can be used
 * with the <a href="http://pomodorotechnique.com">pomodoro technique</a>.
 *
 * \date 25 Oct 2014
 * \author <a href="mailto:con-f-use@gmx.net">con-f-use</a>
 * \copyright
 * This file is part of To-mate-Oh. Copyright © 2014 con-f-use. Use permitted
 * under GNU General Public License v3.0. It is based on the Pomodoro timer by
 * Robin Scheibler (FakuFaku). See: http://robinscheibler.org and
 * https://github.com/fakufaku/Pomodoro for more details on the original
 * project.
 */

// INCLUDES
#include <avr/io.h>         // standard input/output for handy macros
#include <util/delay.h>     // for delay functions
#include <avr/power.h>      // handy power down commands
#include <avr/wdt.h>        // easy control of the watchdog (wakes the device)
#include <avr/sleep.h>      // sleep mode for lower power consumption
#include <avr/interrupt.h>  // for sei()
#include "To-mate-Oh.h"

int main(void) {
    unsigned i;

    // SETUP
    state = WORK;         // When not idling go to work
    cal = Get_cal();      // Calibrate the touch button

    // INDICATE READINESS (and led/buzzer function & calibration value)
    for(i=0; i<cal; ++i) {
      Shine_led(i%6, 10);   _delay_ms(10);
      if( i>5 && Get_time()>cal ) break;
    }

    // MAIN LOOP
    while( 1 ) {
      if( Get_time()>cal ) Handle_button_press();
      Sleep_now(1, WDTO_500MS);
    }

    return 0;
}

//###################################################################### Timing

void Handle_button_press() {
    int i, j;
    unsigned char k;
    uint32_t t_led;
    unsigned char tmp;
    unsigned char leds;
    uint16_t bright=T_ON;
    uint16_t on_iter;

    if(state==WORK) {
        tmp = 0; leds = 5; // Initially all five LEDs on
        // Cycle through the leds
        for( i=0; i<25; ++i) { // 25 iterations, each one minute
          j = i%5;
          if( j==0 ) {
            leds--;   // turn off one led every 5 minutes
            bright = T_ON-10;
          }
          bright>>=1; // reduce brightness of leading led every minute

          for( t_led=0; t_led<(uint32_t)6e7/T_CYC; ++t_led) {

            // Blink led to indicate time in the fifth of a pomodoro
            if( j>0 && t_led%((uint32_t)5e5/T_CYC)==0 ) {
              if(tmp == 0) { --leds; tmp=1; }
              else { ++leds; --j; tmp=0; }
            }

            // PWM the leds
            for( k=0; k<6; ++k ) {
              Set_leds(leds-k);
              if( k==0 ) {
                for(on_iter=0; on_iter<bright; ++on_iter)
                  _delay_us(1);
                Set_leds(-1);
                for(on_iter=0; on_iter<T_ON-bright-10; ++on_iter) _delay_us(1);
              } else {
                _delay_us(T_ON);
                Set_leds(-1);
              }
              _delay_us( (T_OFF-T_ON*5)/6-150 );
            }

            // Check for user abort
            if( t_led>(uint32_t)1e6/T_CYC && Get_time()>cal ) {
              state = REST; i=25; break;
            }
          }
        }
        if( state != REST ) { state = WAIT; Play_sound(); }
    }

    if(state==WAIT) {
        // Blink red led until button is pressed or go to idle if timed out
        for(t_led=0; t_led<300; t_led++) {
          Shine_led(5, 49);  Sleep_now(1, WDTO_500MS);
          if( t_led>5 && Get_time()>cal ) { state = REST; break; }
        }
        if(state != REST) { state=WORK; return; }
    }

    if(state==REST) {
        for(t_led=0; t_led<300; ++t_led) {
          Shine_led(5, 100);
          if( t_led%60==59 ) Sleep_now(1, WDTO_500MS);
          if( Get_time()>cal ) { state=WORK; return; };
        }
        state = WORK;
    }

    Play_sound();
}

//=============================================================================
// HELPERS
//=============================================================================

//###################################################################Sleep mode

EMPTY_INTERRUPT(WDT_vect);

void Sleep_now(uint8_t periods, uint8_t timeout) {
  cli();
  wdt_reset();
  wdt_disable();
  // correct WDP3 location
  timeout |= ((timeout&(1<<3))<<2);
  timeout &=~(1<<3);
  MCUSR = 0x00;
  WDTCR = (1<<WDE) | (1<<WDCE); // enable watchdog
  WDTCR = (1<<WDIE) | timeout;  // watchdog interrupt instead of reset
  //+reset, timeout can be 15,30,60,120,250,500ms or 1,2,4,8s
  sei();
  for( ; periods>0; --periods) {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // set type of sleepmode
    power_all_disable();                  // disable unneeded loads
    wdt_reset();                          // timer should start at zero
    sleep_enable();                       // approach sleep mode
    sleep_mode();                         // enter sleep mode (confirm)
    sleep_disable();                      // entrance point when woken up
    wdt_disable();
    power_all_enable();                   // re-enable the loads
  }
}

//###################################################################Indicators

void Set_leds(int led) { // Same as Robin's version except for white spaces.
  switch (led) {
    case 2:         // LED1
      DDRB |= (1 << DDB3);  DDRB |= (1 << DDB1);  DDRB &= ~(1 << DDB4);
      PORTB &= ~(1 << 3);   PORTB &= ~(1 << 4);   PORTB |= (1 << 1);
    break; case 3:  // LED2
      DDRB |= (1 << DDB3);  DDRB |= (1 << DDB1);  DDRB &= ~(1 << DDB4);
      PORTB &= ~(1 << 1);   PORTB &= ~(1 << 4);   PORTB |= (1 << 3);
    break; case 4:  // LED3
      DDRB |= (1 << DDB3);  DDRB |= (1 << DDB4);  DDRB &= ~(1 << DDB1);
      PORTB &= ~(1 << 1);   PORTB &= ~(1 << 4);   PORTB |= (1 << 3);
    break; case 5:  // LED4 (button side)
      DDRB |= (1 << DDB3);  DDRB |= (1 << DDB4);  DDRB &= ~(1 << DDB1);
      PORTB &= ~(1 << 1);   PORTB &= ~(1 << 3);   PORTB |= (1 << 4);
    break; case 0:  // LED5 (other color)
      DDRB |= (1 << DDB1);  DDRB |= (1 << DDB4);  DDRB &= ~(1 << DDB3);
      PORTB &= ~(1 << 3);   PORTB &= ~(1 << 1);   PORTB |= (1 << 4);
    break; case 1:  // LED6
      DDRB |= (1 << DDB1);  DDRB |= (1 << DDB4);  DDRB &= ~(1 << DDB3);
      PORTB &= ~(1 << 3);   PORTB &= ~(1 << 4);   PORTB |= (1 << 1);
    break; default: // turn all LEDs off
      DDRB &= ~(1 << DDB1) & ~(1 << DDB3) & ~(1 << DDB4);
      PORTB &= ~(1 << 1) & ~(1 << 3) & ~(1 << 4);;
    break;
  }
}

void Shine_led(int led, uint32_t duration) {
  duration = (duration*10000)/T_CYC;
  for( ; duration>0; --duration ) {
        Set_leds(led);    _delay_us(T_ON);
        Set_leds( -1);    _delay_us(T_OFF);
  }
}

void Start_buzz() {
  DDRB |= (1 << DDB0); // set PB0 output

  // Fast PWM, OC0A, non-inverting, divide clock by 8 (~4kHz @ clk 8MHz)
  TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS01);
  OCR0A = 128;  // half duty cycle
}

void Stop_buzz() {
  TCCR0A = 0x0;         // stop timer
  DDRB &= ~(1 << DDB0); // set PB0 as input
}

void Play_sound() {
    unsigned i;
    unsigned char j;
    unsigned char duration[] = {2, 1, 1};

    for(i=0; i<11; ++i) {
        Start_buzz();
        for(j=0; j<duration[i%3];++j) Shine_led(0, 35);
        Stop_buzz();
        _delay_ms(50);
    }
}

//################################################################# Cap sensing

unsigned Get_time() {
    unsigned i = 0;
    const uint8_t mask = (1 << PB2);

    // MEASURE THE TIME FOR THE BUTTON TO CHARGE
    PORTB &= ~mask;         // pull-up off
    DDRB  |=  mask;         // discharge
    while( PINB & mask );   // wait till discharge complete
    _delay_us(10);          // wait some more for good measure
    DDRB  &= ~mask;         // set pin to input
    TCCR1 = (1 << CS10);    // enable timer at CPU frequency

    PORTB |=  mask;         // internal pull-up on
    TCNT1 = 0;              // reset timer
    while((PINB & mask)<1); // wait for pin to charge
    i=TCNT1;                // get time
    TCCR1 = 0;              // disable timer
    if( TIFR&(1<<TOV1) ) {  // If there was a timer overflow
      i=255;
      TIFR |= (1<<TOV1);    // Clear timeout flag
    }

    PORTB &= ~mask;         // pull-up off
    return i;
}

unsigned Get_cal() {
    int i;
    uint16_t stime = 0;

    _delay_ms(10);  // wait till controller has settled

    // Measure a couple discharge times, then calculate the average
    for(i=1<<3; i>0; --i) stime += Get_time();
    stime >>= 3;    // division to get the mean
    // Indicate Cal Error
    if( stime >= 255 || stime < 2 ) { // constant too large or small
      for(i=0; i<5; ++i) { Shine_led(3, 50); Sleep_now(1, WDTO_500MS); }
      return stime;
    }
    return stime + BTN_THRESHOLD;
}
