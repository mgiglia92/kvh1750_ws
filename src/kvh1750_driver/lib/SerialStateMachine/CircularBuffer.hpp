#include <stdint.h>
#include <assert.h>

typedef struct circular_buff_s{
    bool full;
    uint8_t * buffer;
    size_t head;
    size_t tail;
    size_t bufferLength;

}circular_buff_t;



void cbuf_reset(circular_buff_s* buff)
{

    buff->head = 0;
    buff->tail = 0;
    buff->full = false;
}
circular_buff_s *cbuf_init(uint8_t* buffer, size_t size)
{
    circular_buff_s* cbuf = (circular_buff_s *)malloc(sizeof(circular_buff_t));
    cbuf->buffer = buffer;
    cbuf->bufferLength = size;
    cbuf_reset(cbuf);
    return cbuf;
}
bool cbuf_full(circular_buff_s* buff)
{
    return buff->full;
}

bool cbuf_empty(circular_buff_s* buff)
{
    return(!buff->full && (buff->head == buff->tail));
}

size_t cbuf_capacity(circular_buff_s* buff)
{
    return buff->bufferLength;
}

size_t cbuf_size(circular_buff_s* buff)
{
    size_t size = buff->bufferLength;

    if(!buff->full)
    {
        if(buff->head >= buff->tail)
        {
            size = (buff->head - buff->tail);
        }
        else{
            size = (buff->bufferLength + buff->head - buff->tail);
        }
    }
    return size;
}

static void advance_pointer(circular_buff_s* buff)
{
    if(buff->full)
    {
        if(++(buff->tail) = buff->bufferLength)
        {
            buff->tail=0;
        }
    }
    buff->head = (buff->head  + 1) % buff->bufferLength;
    buff->full = (buff->head == buff->tail);
}