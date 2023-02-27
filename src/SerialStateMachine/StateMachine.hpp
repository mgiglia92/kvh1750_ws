#ifndef SERIAL_STATE_MACHINE_H
#define SERIAL_STATE_MACHINE_H
#include <string>
#include <iostream>
#include <CppLinuxSerial/Exception.hpp>
#include <Ring-Buffer/ringbuffer.h>

using namespace std;
namespace SerialStateMachine{
    struct StateMachine
    {
        char bom;// = '$';
        char eom;// = '%';
        char prev[100];
        char current[100];
        char buf_arr[1024];
        ring_buffer_t* ring_buffer;

    };
    void smInit(StateMachine *sm, char bom = '$', char eom = '%')
    {   
        char buf_arr[100];
        sm->bom = bom;
        sm->eom=eom;
        ring_buffer_init(sm->ring_buffer, buf_arr, (size_t)sizeof(buf_arr));
    }



    int16_t findBOM(std::string *s, std::string *bom)
    {
        return s->find(*bom, 0);
    }

    int16_t findEOM(std::string *s, std::string *eom)
    {
        return s->find(*eom, 0);

    }

    void update(StateMachine* sm)
    {

    }

    void append(struct StateMachine *sm, std::string data)
    {

    }
}
#endif 