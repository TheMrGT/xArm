// xArm Commander
// Change baud rate in env.make to 9600 to match xArm
// UNO 0/1 (uart) - connect to xARM, RX/TX
// UNO 2/3 (soft_serial) - connect to FTDI/USB converter to RPi, RX/TX
// Uno sends commands to xARM via 0/1
// UNO accepts commands/prints responses from xArm via 2/3
// use tio -l to determine which /dev is connected to FTDI
// use tio /dev/{name} to connect to FTDI i.e; /dev/ttyUSB0
 
#include <stdio.h>
#include <string.h>
#include "xArm.h"
#include "uart.h"
#include "soft_serial.h"

// tokenLine - break arg:input into tokens
// on entry arg:input has tokens each separated by a delim, defined below
// on return: tokens contains pointers to a legal string for each token
// ex: token[0] is a pointer to the first string
char* tokenLine(char *input)
{
    char delim[MAX_DELIMS + 1] = {" "};

    uint8_t index = 0;
    tokens[index] = strtok(input, delim);
    while ( (tokens[index] != NULL) && (index < MAX_TOKENS - 1) ) 
    {
        index++;
        tokens[index] = strtok(NULL, delim);
    }
    return *tokens;
}

// define the available commands for controlling the xArm
// command is converted into an int then used in execute_cmd()
#define NUM_COMMANDS 6
#define MAX_CMD_LENGTH 6 // # of characters + null terminator
const char commands[NUM_COMMANDS][MAX_CMD_LENGTH] = 
{
    "move",
    "pos",
    "off",
    "reset",
    "volt",
    "beep"
};

int command_to_int(const char *command) 
{
    for (uint8_t i = 0; i < NUM_COMMANDS; ++i) {
        if (strcmp(command, commands[i]) == 0) {
            return i + 1; // Return 1-based index
        }
    }
    return -1; // Invalid command
}

uint8_t execute_cmd(uint8_t c_id)
{
    int8_t r = 0;
    switch (c_id) 
    {
        // move joint position
        case 1:
            echo_command(pos);
            r = valid_move(tokens[joint], tokens[pos]);
            break;
        
        // pos joint
        case 2:
            echo_command(joint);
            r = print_position(tokens[joint]);
            break;
        
        // off
        case 3:
            echo_command(joint);
            r = notimplemented;
            break;
        
        // reset
        case 4:
            echo_command(joint);
            r = notimplemented;
            break;
        
        // volt - get the battery voltage
        case 5:
            echo_command(cmd);
            r = print_Voltage();
            break;
        
        // beep - make arm beep
        case 6:
            echo_command(cmd);
            xArm_beep();
            r = 0;
            break;
        
        // command not found
        default:
            echo_command(pos);
            r = 1;
            break;
    }

    if (r == -1)
    {
        r = badparms;
    }
    return r;
}

int main(void) 
{    
    // UART is used to communicate with xArm
    // soft_serial is used to communicate with user
    init_serial();
    init_soft_serial();
    init_xArm();

    // input is the buffer for the soft_serial port
    char input[MAX_BUFFER + 1] = {};

    while (1) 
    {
        // get input, use first token as command
        soft_readLine(input, MAX_BUFFER);
        tokenLine(input);
        uint8_t command_id = command_to_int(tokens[cmd]);

        uint8_t result = execute_cmd(command_id);
        print_result(result);
        soft_char_NL();

        // clear input buffer for next command
        for (uint8_t i=0; i<MAX_BUFFER; i++)
        {
            input[i] = ' ';
        }
    }
    return 0;
}
