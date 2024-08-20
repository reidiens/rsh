#include "tsh_bufs.h"

uint8_t init_io_bufs() {
    inbuf->buf = malloc(INPUT_BUFSIZE * sizeof(char));
    if (!inbuf->buf) return EXIT_UNRECOVERABLE;

    inbuf->max = INPUT_BUFSIZE;

    outbuf->buf = malloc(OUTPUT_BUFSIZE * sizeof(char));
    if (!outbuf->buf) return EXIT_UNRECOVERABLE;

    outbuf->max = OUTPUT_BUFSIZE;

    memset(inbuf->buf, 0, INPUT_BUFSIZE);
    memset(outbuf->buf, 0, OUTPUT_BUFSIZE);

    inbuf->size = 0;
    outbuf->size = 0;

    return EXIT_SUCCESS;
}

void free_io_bufs() {
    free(inbuf->buf);
    free(outbuf->buf);
    memset(inbuf, 0, sizeof(*inbuf));
    memset(outbuf, 0, sizeof(*outbuf));
}

uint8_t flush_io_bufs() {
    memset(inbuf->buf, 0, inbuf->max);
    memset(outbuf->buf, 0, outbuf->max);

    inbuf->size = 0;
    outbuf->size = 0;

    if (inbuf->max >= INPUT_BUFSIZE * 4) {
        inbuf->buf = realloc(inbuf->buf, INPUT_BUFSIZE * sizeof(char));
        if (!inbuf->buf) {
            inbuf = alloc_tsh_buf(INPUT_BUFSIZE);
            if (!inbuf)
                return EXIT_UNRECOVERABLE;
            return EXIT_RECOVERABLE;
        }
        inbuf->max = INPUT_BUFSIZE;
    }

    if (outbuf->max >= OUTPUT_BUFSIZE * 2) {
        outbuf->buf = realloc(outbuf->buf, OUTPUT_BUFSIZE * sizeof(char));
        if (!outbuf->buf) {
            outbuf = alloc_tsh_buf(OUTPUT_BUFSIZE);
            if (!outbuf)
                return EXIT_UNRECOVERABLE;
            return EXIT_RECOVERABLE;
        }
        outbuf->max = OUTPUT_BUFSIZE;
    }

    return EXIT_SUCCESS;
}

tsh_buf_t* alloc_tsh_buf(size_t bufsize) {
    tsh_buf_t *buffer = {};
    
    buffer->buf = malloc(bufsize * sizeof(char));
    if (!buffer->buf) return NULL;

    buffer->max = bufsize;

    memset(buffer->buf, 0, bufsize);

    buffer->size = 0;

    return buffer;
}

void free_tsh_buf(tsh_buf_t *buffer) {
    free(buffer->buf);
    memset(buffer, 0, sizeof(*buffer));
}

void flush_tsh_buf(tsh_buf_t *buffer) {
    memset(buffer->buf, 0, buffer->max);
    buffer->size = 0;
}