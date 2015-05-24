/* Indent: space, Tabsize: 4, Encoding: UTF-8, Language: C/Eng, Breaks: linux */
/**
 * \file To-mate-Oh.h
 *
 * Contains the prototypes, constants and global definitions as well as the
 * documentation of To-mate-Oh, a little timer device for use with the
 * <a href="http://pomodorotechnique.com">pomodoro technique</a>.
 *
 * User input is sensed via a time integration to sense capacity in the
 * "button". An average charging time is computed before hand. This time in turn
 * then serves as a calibration value to detect the button press.
 *
 * The charging time changes with the capacity of the button, which in turn is
 * changed when a human comes near the button. A daramtic increase in charging
 * time means an increase in capacity and thus something touching the button.
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

#ifndef ___TomateOh_h___
#define ___TomateOh_h___

/**
 * \mainpage To-mate-Oh - A small Pomodoro timer with a capacitive button
 *
 * \image latex To-mate-Oh-final.jpg "Home made To-mate-Oh (not an actual tomato)" width=8cm
 *
 * \copyright This file is part of the To-mate-Oh project. Copyright © 2014
 * con-f-use. Use permitted under GNU General Public License (see LICENSE.txt).
\n
 * To-mate-Oh is based on the Pomodoro timer by Robin Scheibler (FakuFaku). See
 * <http://robinscheibler.org>
 * and
 * <https://github.com/fakufaku/Pomodoro>
 * for more details on the original project.
\n
 *  To-mate-Oh is a free electronics project: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
\n
 *  To-mate-Oh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
\n
 *  You should have received a copy of the GNU General Public License
 *  along with To-mate-Oh. If not, see <http://www.gnu.org/licenses/>
 *
 * The following files comprise To-mate-Oh: ./docu/docu.pdf, ./docu/doxygen.cfg,
 * ./docu/doxygen.sh, ./firmware/Makefile, ./firmware/To-mate-Oh.c,
 * ./firmware/To-mate-Oh.h, ./hardware/To-mate-Oh.brd,
 * ./hardware/To-mate-Oh.sch, ./hardware/To-mate-Oh-board.png,
 * ./hardware/To-mate-Oh-etch.pdf, ./hardware/To-mate-Oh-schematic.pdf,
 * ./hardware/To-mate-Oh-prototype.jpg, ./hardware/To-mate-Oh-final.jpg,
 * ./hardware/To-mate-Oh-kit.jpg, ./hardware/To-mate-Oh-panel.gerber.zip,
 * ./LICENSE.txt, ./README.md
 *
 * Their use is permitted under the terms of GNU General Public License as well.
 * If you want to use any part under different terms, please feel free to ask
 * the author for his permission.
 *
 * \section sec_intro Introduction
 *
 * I saw the <a href="https://github.com/fakufaku/Pomodoro">
 * POMODORO project
 * </a> by <a href="http://robinscheibler.org">
 * Robin Schreibler
 * </a> and decided to build it. Some may know Robin by the name FakuFaky. His'
 * is a very nice layout with beautifully clean code.
 *
 * That can't stand, I thought, so first thing I did was soil the code and
 * complicate the layout. And here is why: I was measuring the power consumption
 * of the original POMODORO. With a fresh coin cell at 3.3 V, the POMODORO
 * drained 1.6mA, even when in power down. Your average coin cell has about 200
 * mAh to give, ignoring the voltage decay. That means after 5 days at most, the
 * battery is gone.
\n
 * \image latex To-mate-Oh-schematic.pdf "New To-mate-Oh schematic. It has reverse polarity protection, four less resistors and replaced the button by a capacitive touch pad." width=12cm
\n
 * I thought I can do better, and that kind of exploded in my face. Now the
 * board is a quarter of Robin's footprint, has proper reverse polarity
 * protection, drains only 7 uA when inactive and uses less components. Most
 * notably I could do away with four resistors and the switch. The design uses
 * capacitive sensing now. The code still works with the old hardware though.
 * On the down side, the code is much uglier (but uses only a third of the
 * space on the micro controller). I think it's an improvement, but judge for
 * yourself.
 *
 * Anyways, many thanks to Robin for making his project open source and being
 * awesome!
 *
 * \section sec_making Making the Board
 *
 * You can order the circuit board at a board house of your choice. e.g.
 * <a href="http://dirtypcbs.com/view.php?share=3243&accesskey=5920ddd4c2010ad6628f47974e820113">Dirty PCBs</a>.
 * The board is still one sided, but you have to solder the components, break it
 * in two, and sandwich-connect the two halves by soldering them together with
 * solid leads of metal (pin headers work fine).
 *
 * \image latex To-mate-Oh-prototype.jpg "First prototype being programmed" width=8cm
 *
 * If you choose to make the board yourself, there are tons of good
 * introductions on
 * <a href="http://hackaday.com/2008/07/28/how-to-etch-a-single-sided-pcb/">
 * how to etch circuit boards</a> at home and
 * <a href="http://store.curiousinventor.com/guides/How_To_Solder">
 * how to solder</a> components to them. With that in mind, the eagle files,
 * pictures and schematics in the ./hardware directory should be all you need,
 * except for the components of course. The coin cell holder is sandwiched
 * between the two halves of the board. You might need to fill the middle
 * trough hole pad under the buzzer with something to make proper contact with
 * the negative side of the battery. The out two through hole pads can be used
 * to solder the battery holder (if needed with the aid of some wire). I listed
 * the components and tools needed
 * below. The components are available e.g. on
 * <a href="https://mouser.com">Mouser</a>. Exact values and part numbers are
 * only recommendations. I listed what I used in brackets:
 * - Atmel AtTiny25/45/85 microcontroller
 * - 5x LED all one color, SMD 0805 (VLMB1300-GS08)*
 * - 1x LED any other color, SMD 0805 (VLMS1300-GS08)*
 * - 2x 3.3 k Ohm Resistor, SMD 0805
 * \footnote{The 3k3 resistor on the base of the NPN shouldn't be lower or it
 * might interfere with programming.}
 * - 1x NPN BJT transistor, SOT-23 (BC857)
 * \footnote{The NPN can be pretty generic but should be able to stomach the
 * spikes created by the inductor.}
 * - 1x 220uH Inductor, SMD 1007
 * - 1x Buzzer, SMD (Kobitone 254-PB119-ROX)
 * - 1x CR2032 coin cell and holder
 * - Optional:\n
 *     1x MOSFET P-Channel, SOT-23 (SI2371)
 *     \footnote{The P-MOS should have a threshold voltage well below the
 *     battery voltage to work as reverse polarity protection. A low on
 *     resistance is nice, too. If you don't use it, short source and drain
 *     together.},\n
 *     1x 1-10 uF Capacitor, SMD 1210
 *     \footnote{The 10uF cap is a good thing to enhance battery life by
 *     smoothing out peak drains/spikes. It is however not necessary.}
 *
 * \warning __If you use LEDs other than the Vishay 1300 series, you might get
 * differences in brightness and even damage your micro controller. The LEDs are
 * PWM-dimmed with no current limiting resistor. Not all LEDs have the same
 * efficiency and forward voltage, meaning current through them may vary. And
 * even at same current, the brightness may vary. The ATTiny must only supply up
 * to 40mA max per output pin and only sink 10mA per input. Additionally all
 * inputs together must not sink more than 60mA (150 mA source per output).
 * At the low duty cycles in the program and 3.3V supply voltage from the
 * battery there should be no problem no matter the LEDs. But if you use
 * non-standard LEDs or supply the circuit with more than 3.3V, e.g. when
 * programming, give it a thought or two and consult the
 * <a href="http://www.atmel.com/images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf">
 * ATTiny's datasheet
 * </a>.__
 *
 * \image latex To-mate-Oh-board.png "To-mate-Oh Board" width=8cm
 *
 * Tools for soldering:
 * - Good soldering ion with small tip for SMD soldering
 * - Third hand tool
 * - Clothing ion
 * - Programmer for the micro controller
 * - <a href="http://www.tag-connect.com/">Tag connect adapter</a>
 *   \footnote{The tag connect adapter is only needed if you don't want to
 *   solder temporary wires for programming. It has a much smaller footprint and
 *   some other advantages. Some people have build clones.You could also program
 *   the chip off board.}
 *
 * If you are making the board yourself (if you really don't want to order it):
 * - Laser printer
 * - Dremel or similar tool for drilling small holes
 * - Tool to cut your copper clad board (for thin material -> scissors)
 * - Cloth iron or laminator
 * - Acid resistant Gloves
 * - Acid resistant container to hold your etching solution
 * - Safety glasses
 *
 * \warning __Etching circuit boards involves corrosive chemicals with the usual
 * dangers (eye damage, chemical burns, damage to the respiratory system,
 * etc.).__
 *
 * \section sec_firm Compiling and flashing the firmware
 *
 * The firmware for this project requires avr-gcc and avr-libc (a C-library for
 * the AVR controller). Please read the instructions at
 * http://www.nongnu.org/avr-libc/user-manual/install_tools.html for how to
 * install the GNU toolchain (avr-gcc, assembler, linker etc.) and avr-libc.
 *
 * <a href="http://www.ladyada.net/learn/avr/index.html">Lady Ada's avr
 * tutorial</a> covers the basics of flashing firmware to a microcontroller
 * nicely.
 *
 * However, running "make program" in the \e firmware direcotry should be
 * sufficient. Maybe you will have change the value of "DEVICE" or other
 * settings the \e Makefile, especially if you are using a programmer other
 * than
 * <a href="http://www.ladyada.net/make/usbtinyisp/">USBtinyISP</a>
 * (btw.: you can build USBtinyISP at home).
 *
 * To flash the firmware to the microcontroller run "make flashfuse" after you
 * connected the ISP header on the board to your programmer. The board does not
 * connect the Tag Connect's plus pin to the micro controller. The power pin of
 * your programmer will have to be connected manually to the battery holder's
 * positive contact. This is to avoid exposing an inserted battery to the USB
 * voltage, so **before programming, take the battery out.**
 *
 * If you change the firmware or fuse bits, be aware that the cap sensing and
 * led timing is very sensitive. In the ./firmware/To-mate-Oh.h are a few
 * settings related to LED PWM and timeouts (scroll way down in the .h file
 * until after this documentation).
 *
 * \note There is much room for optimization. For instance, one could get rid
 * of the delays by using a timer for the short led pulses and putting the
 * controller to sleep for the long ones (the watchdog timer can go down to 15
 * ms). The watchdog timer is not that accurate and temperature stable though.
 * Speaking of accuracy, the delay lengths could be adjusted for more accurate
 * timing.
 *
 * \section sec_usage Using the Board
 *
 * From Robins original documentation:
 *
 * 1. One click starts a 25 minutes _work_ counter. Five yellow LED indicates
 * how many slices of five minutes are left. There is a discrete beep when the
 * timer expires. The LED then flashes to remind you to go into _rest_ mode.
 * 2. A second click starts a 5 minutes _rest_ counter. A **red** LED is turned
 * on. There is a discrete beep when it expires. Then the timer turns off.
 * 3. If you wish to turn the timer off during _rest_ mode, click the button a
 * third time.
 *
 * When you insert a fresh battery, a calibration for the cap sense button is
 * performed. After a successful calibration the To-mate-Oh will circle through
 * its LEDs and beep. If you count the beeps or how often a different LED is
 * lit, it will give you the calibration constant. If the calibration fails, the
 * To-mate-Oh will blink LED2 for five seconds. Most likely the sense pad is
 * connected to a large capacity or ground then.
 *
 */


// CONFIGURATION CONSTANTS
#define BTN_THRESHOLD 2 ///< Threshold for the button to detect user input
#define ON_TIME 5       ///< LED on time per PWN cycle in units of 100us
#define DUTY    5       ///< Duty cycle of the LED


// GLOBALS
unsigned cal;           ///< Storage for the calibration values of the button
unsigned char state;    ///< Current state the timer is in


// VALUES FOR THE STAT VARIABLE
#define WORK 1          ///< Timer is counting down a Pomodro (25min chunk)
#define WAIT 3          ///< Waiting for user to start rest period
#define REST 4          ///< 5 min rest


// DERIVED VALUES
#if ON_TIME<1 || ON_TIME>10
  #error "T_ON must be an integer between 1 and 10"
#endif
#if DUTY>100 || DUTY<1
  #error "DUTY must be an integer between 1 and 100"
#endif
#define T_ON  ((uint32_t)(ON_TIME)*100)
#define T_CYC (T_ON*100/(uint32_t)(DUTY))
#define T_OFF (T_CYC-T_ON)

// FUNCTION PROTOTYPES

/**Intializes the watchdog timer and sets the microcontroller's pins properly.*/
void Config_wdt(uint8_t);

/**
 * Put microcontroller into sleep mode to conserve power.
 *
 * Although the ATTiny25 supports it on paper, using timeouts over two seconds
 * leads to strange behavior in test cases. So use "WDTO_2S" as absolute maximum
 * timeout!
 */
void Sleep_now(uint8_t, uint8_t);

/** Access individual LEDs through charlieplexing. */
void Set_leds(int led);

/** Poor PWM for dimming an led for a duration given in multiples of 10ms. */
void Shine_led(int led, uint32_t duration);

/** Start the piezo buzzer. */
void Start_buzz();

/** Stop the piezo buzzer. */
void Stop_buzz();

/** Plays the indicator melody. */
void Play_sound();

/**
 * Measure the time to charge the button. Takes the sum of several charge
 * cycles to be more sensitive.
 *
 * \return Time it took to charge the button
 */
unsigned Get_time();

/**
 * Calibrate charge time of the button.
 *
 * This time is later used to detect a button press.
 */
unsigned Get_cal();

/** Invokes the respective actions, when the button was pressed. */
void Handle_button_press();

#endif

