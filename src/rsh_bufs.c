#include "rsh_core.h"

#include "rsh_bufs.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t init_io_bufs() {
    rsh_inbuf = (rsh_buf_t *)malloc(sizeof(rsh_buf_t));
    if (!rsh_inbuf) {
        rsh_err("Couldn't allocate input buffer structure memory");
        exit(1);
    }

    rsh_outbuf = (rsh_buf_t *)malloc(sizeof(rsh_buf_t));
    if (!rsh_outbuf) {
        rsh_err("Couldn't allocate output buffer structure memory");
        free_rsh_buf(rsh_inbuf);
        exit(1);
    }

    rsh_inbuf->buf = malloc(INPUT_BUFSIZE * sizeof(char));
    if (!rsh_inbuf->buf) return EXIT_UNRECOVERABLE;

    rsh_inbuf->max = INPUT_BUFSIZE;

    rsh_outbuf->buf = malloc(OUTPUT_BUFSIZE * sizeof(char));
    if (!rsh_outbuf->buf) return EXIT_UNRECOVERABLE;

    rsh_outbuf->max = OUTPUT_BUFSIZE;

    memset(rsh_inbuf->buf, 0, INPUT_BUFSIZE);
    memset(rsh_outbuf->buf, 0, OUTPUT_BUFSIZE);

    rsh_inbuf->size = 0;
    rsh_outbuf->size = 0;

    return EXIT_SUCCESS;
}

void free_io_bufs() {
    free(rsh_inbuf->buf);
    free(rsh_outbuf->buf);
    free(rsh_inbuf);
    free(rsh_outbuf);
}

uint8_t flush_io_bufs() {
    memset(rsh_inbuf->buf, 0, rsh_inbuf->max);
    memset(rsh_outbuf->buf, 0, rsh_outbuf->max);

    rsh_inbuf->size = 0;
    rsh_outbuf->size = 0;

    if (rsh_inbuf->max >= INPUT_BUFSIZE * 4) {
        rsh_inbuf->buf = realloc(rsh_inbuf->buf, INPUT_BUFSIZE * sizeof(char));
        if (!rsh_inbuf->buf) {
            rsh_inbuf = alloc_rsh_buf(INPUT_BUFSIZE);
            if (!rsh_inbuf)
                return EXIT_UNRECOVERABLE;
            return EXIT_RECOVERABLE;
        }
        rsh_inbuf->max = INPUT_BUFSIZE;
    }

    if (rsh_outbuf->max >= OUTPUT_BUFSIZE * 2) {
        rsh_outbuf->buf = realloc(rsh_outbuf->buf, OUTPUT_BUFSIZE * sizeof(char));
        if (!rsh_outbuf->buf) {
            rsh_outbuf = alloc_rsh_buf(OUTPUT_BUFSIZE);
            if (!rsh_outbuf)
                return EXIT_UNRECOVERABLE;
            return EXIT_RECOVERABLE;
        }
        rsh_outbuf->max = OUTPUT_BUFSIZE;
    }

    return EXIT_SUCCESS;
}

rsh_buf_t* alloc_rsh_buf(size_t bufsize) {
    rsh_buf_t *buffer = {};
    
    buffer->buf = malloc(bufsize * sizeof(char));
    if (!buffer->buf) return NULL;

    buffer->max = bufsize;

    memset(buffer->buf, 0, bufsize);

    buffer->size = 0;

    return buffer;
}

void free_rsh_buf(rsh_buf_t *buffer) {
    free(buffer->buf);
    memset(buffer, 0, sizeof(*buffer));
}

void flush_rsh_buf(rsh_buf_t *buffer) {
    memset(buffer->buf, 0, buffer->max);
    buffer->size = 0;
}