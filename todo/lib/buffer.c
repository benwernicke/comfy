#include "buffer.h"

struct buffer_t {
    uint64_t allocated;
    uint64_t used;
    uint64_t member_size;
    uint8_t* buf;
};

buffer_t* buffer_create(uint64_t initial_size, uint64_t member_size)
{
    buffer_t* b = malloc(sizeof(*b));
    if (b == NULL) {
        return NULL;
    }
    b->allocated = initial_size;
    b->used = 0;
    b->member_size = member_size;
    b->buf = malloc(initial_size * member_size);
    if (b->buf == NULL) {
        free(b);
        return NULL;
    }
    return b;
}

buffer_t* buffer_create_from_range(uint64_t initial_size, uint64_t member_size, void* range, uint64_t range_size)
{
    buffer_t* b = buffer_create(initial_size, member_size);
    if (b == NULL) {
        return NULL;
    }
    memcpy(b->buf, range, range_size);
    b->used = range_size / member_size;
    return b;
}

void* buffer_get(buffer_t* b, uint64_t index)
{
    return b->buf + index * b->member_size;
}

void buffer_free(buffer_t* b)
{
    if (b == NULL) {
        return;
    }
    free(b->buf);
    free(b);
}

void* buffer_more(buffer_t* b)
{
    if (b->used >= b->allocated) {
        uint64_t new_allocated = b->allocated << 1;
        uint8_t* new_buf = realloc(b->buf, new_allocated * b->member_size);
        if (new_buf == NULL) {
            return NULL;
        }
        b->buf = new_buf;
        b->allocated = new_allocated;
    }
    return b->buf + b->used++ * b->member_size;
}

uint64_t buffer_index_from_ptr(buffer_t* b, void* ptr)
{
    return (uint64_t)((uint8_t*)ptr - b->buf) / b->member_size;
}

void* buffer_begin(buffer_t* b)
{
    return b->buf;
}

void* buffer_end(buffer_t* b)
{
    return b->buf + b->used * b->member_size;
}

uint64_t buffer_size(buffer_t* b)
{
    return b->used;
}

void buffer_remove(buffer_t* b, uint64_t index)
{
    if (index == b->used - 1) {
        b->used--;
        return;
    }
    memmove(b->buf + index * b->member_size, b->buf + (index + 1) * b->member_size, (b->used - index) * b->member_size);
    b->used--;
}
