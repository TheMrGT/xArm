/* One line kernal for multitasking
*  https://www.embedded.com/a-multitasking-kernel-in-one-line-of-code-almost/
*  Tasks are individually setup using a struct
*  Fundamental premise is an on/off signal, whether LED, or speaker
*/

#include <avr/io.h>
#include "delay.h"
#include "pinMode.h"
#include "sysclock.h"
#include "digitalWrite.h"

/* Defaults:
*  NTASKS: Number of tasks to be defined
*  DEFAULT_ON/DEFAULT_OFF: Times (msec) for task to be on/off
*  With On/Off set to 435, three tasks run at 18kHz (fastest freq)
*/
#define NTASKS 3
#define DEFAULT_ON 435
#define DEFAULT_OFF 435

typedef struct task {
   volatile uint8_t pin;    // Uno pin 
   uint8_t state;           // Is pin HIGH or LOW
   uint16_t on;             // Time on
   uint16_t off;            // Time off
   uint16_t elapsed;        // Time elapsed sinced last in loop
} task;

task tasks[NTASKS];

// Uno pin numbers
enum {LED0, LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11};

void init(uint8_t index, uint8_t pin, uint8_t state, uint16_t on, uint16_t off,\
    uint16_t elapsed)
{
    tasks[index].pin = pin;
    tasks[index].state = state;
    tasks[index].on = on;
    tasks[index].off = off;
    tasks[index].elapsed = elapsed;
    pinMode(pin, OUTPUT);
}

void update (uint8_t taskID) {
    // Based on adafruit lesson on classes
    // Changed digitalWrite() to bit action, due to overhead of dW()
    uint16_t now = ticks();

    if((tasks[taskID].state == HIGH) && (now - tasks[taskID].elapsed >= tasks[taskID].on))
    {
        tasks[taskID].state = LOW;  // Turn it off
        tasks[taskID].elapsed = now;  // Remember the time
        digitalWrite(tasks[taskID].pin, LOW);
    }
    else if ((tasks[taskID].state == LOW) && (now - tasks[taskID].elapsed >= tasks[taskID].off))
    {
        tasks[taskID].state = HIGH;  // turn it on
        tasks[taskID].elapsed = now;   // Remember the time
        digitalWrite(tasks[taskID].pin, HIGH);
    }
    return;
} 

int main(void)
{
    init_sysclock_1 ();
    init(0, 3, 0, DEFAULT_ON, DEFAULT_OFF, 0);
    init(1, 5, 0, DEFAULT_ON, DEFAULT_OFF, 0);
    init(2, 6, 0, DEFAULT_ON, DEFAULT_OFF, 0);

    // init structs: {index, pin, state, on, off, elapsed}

    while (1)
    {
    for (uint8_t task_cntr=0; task_cntr < NTASKS; ++task_cntr)
        {
            update(task_cntr);
        }
    }
    return 0; 
}
