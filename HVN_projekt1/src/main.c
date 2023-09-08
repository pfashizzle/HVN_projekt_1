/********************************************************************************
 * @brief  This program controls four LEDs using two buttons through GPIO lines.
 *
 *         The program sets up two pairs of LEDs (led1, led2) and (led3, led4) along with
 *         two buttons (button1 and button2) connected to GPIO pins. Two threads are created
 *         to continuously toggle the LEDs based on button presses. When a button is pressed,
 *         the corresponding LED pair toggles its state (on/off), and the program prints a
 *         message indicating whether the LEDs are enabled or disabled.
 *
 *         GPIO line configuration and thread management are handled in this program to
 *         achieve the LED control functionality.
 *
 * @return 0 on successful execution.
 ********************************************************************************/

#include <gpiod_utils.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>

#define SEMAPHORE_ID_PRINT 0


struct thread_args {
    int id_a;
    int id_b;
    struct gpiod_line* led_a;
    struct gpiod_line* led_b;
    struct gpiod_line* button;
    uint16_t blink_speed_ms;
    bool blink_enabled;
    uint8_t last_value;
};

/********************************************************************************
 * @brief Runs thread continuously and prints which LEDs are enabled/disabled
 *        when the button is pressed.
 * 
 * @param arg A pointer to a struct thread_args containing thread-specific data.
 ********************************************************************************/

void* run_thread(void* arg) {
    struct thread_args* self = (struct thread_args*)(arg);

    while(1) {
        if (gpiod_line_event_detected(self->button, GPIOD_LINE_EDGE_RISING, &self->last_value)) {
            self->blink_enabled = !self->blink_enabled;
            semaphore_reserve(SEMAPHORE_ID_PRINT);
            if (self->blink_enabled) {
                printf("LEDs connected to pin %d and %d now enabled!\n\n", self->id_a, self->id_b);
            } else {
                printf("LEDs connected to pin %d and %d now disabled!\n\n", self->id_a, self->id_b);
            }
            delay_ms(10);
            semaphore_release(SEMAPHORE_ID_PRINT);
        }

        if (self->blink_enabled) {
            gpiod_line_set_value(self->led_a, 1);
            gpiod_line_set_value(self->led_b, 1);
            delay_ms(self->blink_speed_ms);

            gpiod_line_set_value(self->led_a, 0);
            gpiod_line_set_value(self->led_b, 0);
            delay_ms(self->blink_speed_ms);
        } else {
            gpiod_line_set_value(self->led_a, 0);
            gpiod_line_set_value(self->led_b, 0);
        }
    }
    return 0;
}
/********************************************************************************
 * @brief The main function initializes GPIO lines for LEDs and buttons, creates
 *        two threads to control LEDs, and waits for the threads to complete.
 *
 *   This function does the following:
 *   Initializes GPIO lines for two pairs of LEDs (led1, led2) and (led3, led4)
 *   and two buttons (button1 and button2).
 *   Defines thread arguments (args1 and args2) for controlling the LEDs and buttons.
 *   Creates two threads (t1 and t2) to run the run_thread function concurrently
 *   for each LED pair and button pair.
 *   Waits for both threads to finish execution using pthread_join.
 *
 * @return 0 on successful execution.
 ********************************************************************************/
int main(void) {
    struct gpiod_line* led1 = gpiod_line_new(17, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led2 = gpiod_line_new(22, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led3 = gpiod_line_new(23, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* led4 = gpiod_line_new(24, GPIOD_LINE_DIRECTION_OUT);
    struct gpiod_line* button1 = gpiod_line_new(27, GPIOD_LINE_DIRECTION_IN);
    struct gpiod_line* button2 = gpiod_line_new(25, GPIOD_LINE_DIRECTION_IN);

    struct thread_args args1 = { 17, 22, led1, led2, button1, 100, false, 0};
    struct thread_args args2 = { 23, 24, led3, led4, button2, 500, false, 0};

    pthread_t t1, t2; 
    pthread_create(&t1, 0, run_thread, &args1);
    pthread_create(&t2, 0, run_thread, &args2);
    pthread_join(t1, 0);
    pthread_join(t2, 0);

    return 0;
}
