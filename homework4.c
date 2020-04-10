#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";
    int charState = 0;
    int responseIndex = 0;

    // TODO: Declare the variables that main uses to interact with your state machine.

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 myConfig;
    myConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    myConfig.clockPrescalar = 19;
    myConfig.firstModReg = 8;
    myConfig.secondModReg = 0x55;
    myConfig.parity = EUSCI_A_UART_NO_PARITY;
    myConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    myConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    myConfig.uartMode = EUSCI_A_UART_MODE;
    myConfig.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    myConfig.dataLength = EUSCI_A_UART_8_BIT_LEN;

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &myConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)){
            rChar = UART_receiveData(EUSCI_A0_BASE);
        }
        else{
            rChar = 0xff;
        }

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != 0xff){
            if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)){
                UART_transmitData(EUSCI_A0_BASE, rChar);
            }

            // TODO: If the FSM indicates a successful string entry, transmit the response string.
            //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
            //       Make sure to reset the success variable after transmission.
            if(rChar == '2'){
                charState = 1;
            }
            else if(charState == 1 && rChar == '5'){
                charState = 2;
            }
            else if(charState == 2 && rChar == '3'){
                charState = 3;
            }
            else if(charState == 3 && rChar == '4'){
                while(responseIndex < 48){
                    if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)){
                        UART_transmitData(EUSCI_A0_BASE, response[responseIndex]);
                        responseIndex++;
                    }
                }
                charState = 0;
            }
            else{
                charState = 0;
            }
        }
    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    return false;
}
