/* SPDX-License-Identifier: GPL-3.0
*/

#include "main.h"

static int look_for_prefix(char);

static int Port = -1; //value of port number parsed
static int Command = 0; //value of command number parsed
static int (*ParserNext)(char) = &look_for_prefix; //pointer to function to be called for tne next letter

static int look_for_delim(char letter)
{
    switch(letter) {
        case '\r': case '\n': //no more digit in the number
            return Command << 8;
        default:
            return -EINVAL; //
    }
    return 0;
}

static int look_for_number(char letter)
{
    switch(letter) {
        case '0'...'9':
            if(0 <= Port) // Not a first digit
                Port = Port * 10 + letter -'0';
            else
                Port =  letter -'0';

            if(MAX_GPIO < Port) {
                return -E2BIG;
            }
            break;
        case '\r': case '\n': //no more digit in the number
            return Command << 8 | Port;
        default:
            return -EINVAL;
    }
    return 0;
}

static int look_for_command_letter(char letter)
{
    switch(letter) {
        case 'H': case 'h':
            Command = COMMAND_PRINT_HELP;
            break;
        case 'D': case 'd':
            Command = COMMAND_DISABLE_ECHO;
            break;
        case 'E': case 'e':
            Command = COMMAND_ENABLE_ECHO;
            break;
        case 'G': case 'g':
            Command = COMMAND_ENABLE_CHANGE;
            break;
        case 'R': case 'r':
            Command = COMMAND_DISABLE_CHANGE;
            break;
        case 'B': case 'b':
            Command = COMMAND_ENTER_BOOTLOADER;
            break;
        default:
            return -EINVAL;
    }
    ParserNext = &look_for_delim;
    return 0;
}

static int look_for_prefix(char letter)
{
    Command = 0;

    switch(letter) {
    case '-':
        ParserNext = &look_for_command_letter;
        break;
    case '^':
    case '_':
    case '%':
    case '&':
    case '?':
    case '@':
        Port = -1;// Reset the port number
        ParserNext = &look_for_number;
        switch(letter) {
        case '^':
            Command = COMMAND_SET_HIGH;
            break;
        case '_':
            Command = COMMAND_SET_LOW;
            break;
        case '%':
            Command = COMMAND_SET_INPUT;
            break;
        case '&':
            Command = COMMAND_SET_OUTPUT;
            break;
        case '?':
            Command = COMMAND_GET_STATE;
            break;
        case '@':
            Command = COMMAND_GET_LEVEL;
            break;
        }
        break;
    case '\r': case '\n':
        break; // An empty line -- do nothing
    default:
        return -EINVAL;
    }
    return 0;
}

static void parser_reset(void)
{
    ParserNext = &look_for_prefix;
}

int parser_get(char letter)
{
    int retval;

    switch(letter){
    case ' ': case '\t':
        return 0; //Omit whitespace
    default:
        retval = (*ParserNext)(letter); 
        if(!!retval) // Reset the parser if the cycle is completed or failed
            parser_reset();

        if(0 > retval) { // Negative return means an error
            print_error(-retval);
            return 0; // no command parsed
        }
        else {
            return retval;
        }
    }
}