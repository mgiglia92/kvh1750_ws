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

//Struct to house converted kvh1750 data
struct Kvh1750Data
{
    float wx;
    float wy;
    float wz;
    float ax;
    float ay;
    float az;
    uint8_t status;
    uint8_t sequence;
    int16_t temp;
};

//Intermediate data structure to validate incoming serial packets
struct SerialParser
{
    char current[36];
    std::vector<char> prev; //The current and previous valid message
    ring_buffer_t ringBuffer; //A ring buffer for the incoming bytes
    ring_buffer_size_t firstValidIndex;
    const char BOM[4] = {(char)0xFE, (char)0x81, (char)0xFF, (char)0x55}; 
};

bool check_bom(char *data, const char *bom, size_t len)
{
    for(auto i=0;i<len;i++)
    {
        if(data[i] != bom[i]) {return false; }
    }
    return true;
}

bool search_parser(SerialParser*);

//Extract valid message if tehre is one, else return false
bool extract_valid_message(SerialParser *parser)
{
    if(search_parser(parser))
    {
        ring_buffer_dequeue_arr(&parser->ringBuffer, &(parser->current[0]), 36);

        std::cout << ring_buffer_num_items(&parser->ringBuffer ) << std::endl;
        std::cout << "ELEMENTS: ";
        for(int i=0; i<36; i++)
        {
            std:: cout << +(int8_t)parser->current[i] << "|";
        }
        std::cout << std::endl;
        // exit(0);
        return true;
    }

    return false;
}

//Search ringbuffer for pattern 0xFE81FF55
bool search_parser(SerialParser *parser)
{
    ring_buffer_size_t FEindex;
    ring_buffer_size_t sIndex;
    size_t numItems = ring_buffer_num_items(&parser->ringBuffer);
    if(numItems > size_t(36))
    {
        //Find 0xFE
        //TODO: Search from end for valid message (BOM and length) and use that cdinstead
        for(ring_buffer_size_t i=0; i<numItems; i++)
        {

            char temp[4];
            ring_buffer_peek(&parser->ringBuffer, temp, 0); //peek value at search index

            if(temp[0] == parser->BOM[0]) //If its == 0xFE
            {            

                //Put next 3 bytes into temp
                ring_buffer_peek(&parser->ringBuffer, &temp[1], 1);
                ring_buffer_peek(&parser->ringBuffer, &temp[2], 2);
                ring_buffer_peek(&parser->ringBuffer, &temp[3], 3);
                char str[100];

                if(check_bom(&temp[0], &parser->BOM[0], 4))
                {
                    parser->firstValidIndex = 0;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {

                //remove all garbage data from start until FEindex
                char throwaway;
                ring_buffer_dequeue(&parser->ringBuffer, &throwaway);
                
            }
        }   
    }
    return false;
}




int main()
{
    //Instantiate structs
    Kvh1750Data imuData;
    SerialParser parser;
    // parser.current.reserve(36);
    // parser.prev.reserve(36);
    //Initialize ring buffer with size 1024
    char buf_arr[128];
    ring_buffer_init(&parser.ringBuffer, buf_arr, (size_t)sizeof(buf_arr));
    
    //Initialize serial port, hardcoded devname for now
    //TODO: Add dev string check to make sure proper device
    std::string devname = "/dev/ttyUSB0";
    auto serialPort = std::make_shared<SerialPort>(devname, 921600);
    serialPort->SetTimeout(0);
    serialPort->Open();

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
                ring_buffer_queue(&parser.ringBuffer, readData[i]);
            }

            std::cout << parser.ringBuffer.buffer[parser.ringBuffer.tail_index] << std::endl;
            if(extract_valid_message(&parser))
            {
                std::cout << str;
            }
        }
    }

}