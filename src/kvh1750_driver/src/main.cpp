#include <iostream>
#include <string>
// #include <SerialStateMachine/CircularBuffer.hpp>
#include <SerialStateMachine/StateMachine.hpp>
#include <CppLinuxSerial/SerialPort.hpp>
#include <thread>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include "Ring-Buffer/ringbuffer.h"
#include <assert.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstddef>

using namespace SerialStateMachine;
using namespace mn::CppLinuxSerial;

uint16_t x = 1024;
// uint8_t * buffer = (uint8_t *)malloc(x * sizeof(uint8_t));
// circular_buff_s * cbuff = cbuf_init(buffer, x);
std::string devname = "/dev/ttyUSB0";
StateMachine sm;

int main()
{
    ring_buffer_t ring_buffer;
    char buf_arr[1024];
    ring_buffer_init(&ring_buffer, buf_arr, (size_t)sizeof(buf_arr));
    auto serialPort = std::make_shared<SerialPort>(devname, 921600);

    // SerialPort serialPort
    serialPort->SetTimeout(0);
    serialPort->Open();

    std::stringstream ss;
    std::vector<uint8_t> readData;    
    char tmp[100] = "";
    char str[100];
    char str2[100];
    while(true)
    {
        readData.clear();
        auto numAvail = serialPort->Available();
        if(numAvail) 
        {
            serialPort->ReadBinary(readData);
            for(int i=0; i<numAvail;i++)
            {
                ring_buffer_queue(&ring_buffer, readData[i]);
            }
            if(ring_buffer_is_full(&ring_buffer))
            {
                for(int i=0; i<100; i++)
                {
                    ring_buffer_dequeue(&ring_buffer, &tmp[i]);
                }
            }
            for(auto item : readData)
            {
                sprintf(str2, "%x", item);
                std::cout << str2;
            }
            
            // sprintf(str, "Value is: %li", ring_buffer_num_items(&ring_buffer));

            // sprintf(str2, "BLAH: %u", readData[1]);
            
        }
    }

}