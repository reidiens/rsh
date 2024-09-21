#include "rsh_core.h"

#include "rsh_bufs.h"

#include <alloca.h>
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

    rsh_errbuf = (rsh_buf_t *)malloc(sizeof(rsh_buf_t));
    if (!rsh_errbuf) {
        rsh_err("Couldn't allocate error buffer structure memory");
        free_rsh_buf(rsh_inbuf);
        free_rsh_buf(rsh_outbuf);
        exit(1);
    }

    rsh_inbuf->buf = malloc(INPUT_BUFSIZE * sizeof(char));
    if (!rsh_inbuf->buf) {
        rsh_err("Couldn't allocate input buffer memory");
        free_io_bufs();
        exit(1);
    }
    rsh_inbuf->max = INPUT_BUFSIZE;

    rsh_outbuf->buf = malloc(OUTPUT_BUFSIZE * sizeof(char));
    if (!rsh_inbuf->buf) {
        rsh_err("Couldn't allocate output buffer memory");
        free_io_bufs();
        exit(1);
    }
    rsh_outbuf->max = OUTPUT_BUFSIZE;
    
    rsh_errbuf->buf = malloc(OUTPUT_BUFSIZE * sizeof(char));
    if (!rsh_inbuf->buf) {
        rsh_err("Couldn't allocate error buffer memory");
        free_io_bufs();
        exit(1);
    }
    rsh_errbuf->max = OUTPUT_BUFSIZE;

    flush_io_bufs();

    #ifdef _IOFBF
        setvbuf(stdout, rsh_outbuf->buf, _IOFBF, rsh_outbuf->max);
        setvbuf(stderr, rsh_errbuf->buf, _IOFBF, rsh_errbuf->max);
    #else
        setbuf(stdout, rsh_outbuf->buf, rsh_outbuf->max);
        setbuf(stderr, rsh_errbuf->err, rsh_errbuf->max);
    #endif 

    return EXIT_SUCCESS;
}

void free_io_bufs() {
    free_rsh_buf(rsh_inbuf);
    free_rsh_buf(rsh_outbuf);
    free_rsh_buf(rsh_errbuf);
}

void reset_io_bufs() {
    free_io_bufs();
    init_io_bufs();
}

void flush_io_bufs() {
    flush_rsh_buf(rsh_inbuf);
    flush_rsh_buf(rsh_outbuf);
    flush_rsh_buf(rsh_errbuf);
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
    if (buffer) {
        if (buffer->buf)
            free(buffer->buf);    
        free(buffer);
    }
}

void flush_rsh_buf(rsh_buf_t *buffer) {
    memset(buffer->buf, 0, buffer->max);
    buffer->size = 0;
}