/*
 * pingingit.c
 *
 *  Created on: 03.09.2018
 *      Author: Daniel.Cesarini
 */

#include "nsconfig.h"
#include "Thread.h"

extern "C"
{
#include "protocol.h"
//#include "icmpv6.h"
}

protocol_interface_info_entry_t *cur;

extern "C"
{
extern protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_id(int8_t nwk_id);
extern void icmpv6_build_echo_req(struct protocol_interface_info_entry *cur, const uint8_t target_addr[16]);
}

void getInterface(int8_t interface)
{
    cur = protocol_stack_interface_info_get_by_id(interface);
}

void pingIt()
{

    // You will need to provide a target IP here
    // I used one of the ULA fd00:...:xx ones that seemed to stay stable across reboots
    // from the BR Thread interfaces
    uint8_t target_addr[16] = {
            0x2a, 0x03,
            0x39, 0xa0,
            0x00, 0x1f,
            0x00, 0x01,
            0x0c, 0x9c,
            0x7b, 0x96,
            0x6d, 0x9b,
            0x5b, 0xd6, };
            
    printf("#\n");
    icmpv6_build_echo_req(cur, target_addr);
}

void cyclicPinging()
{
    rtos::Thread::wait(30000);
    while (1)
    {
        pingIt();
        rtos::Thread::wait(50);
    }
}

rtos::Thread* pingingThread = NULL;

void initPingingThread ()
{
    pingingThread = new rtos::Thread(osPriorityHigh, 1024, NULL, "pingingThread");
    pingingThread->start(cyclicPinging);
}
