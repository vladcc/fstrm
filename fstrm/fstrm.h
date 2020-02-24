/*  fstrm.h -- binary mode file stream
    v1.01

    Emulates random access to a file while sticking to the standard. Uses
    fsetpos(), fgetpos() instead of fseek(), ftell(). Allows for read, write,
    and read/update. Reads a single byte at a time, supports scaling the
    standard library io buffer.

    Author: Vladimir Dinev
    vld.dinev@gmail.com
    2020-02-24
*/

#ifndef FSTRM_H
#define FSTRM_H

#include <stdio.h>

// Do not use directly
typedef struct fstrm_pos {
    fpos_t pos;
} fstrm_pos;

// Do not use directly
typedef struct fstrm {
    FILE * pfile;
} fstrm;

typedef enum fstrm_mode {
    FSTRM_MODE_READ,    // rb
    FSTRM_MODE_WRITE,   // wb
    FSTRM_MODE_UPDATE,  // r+b
} fstrm_mode;

typedef enum fstrm_stat {
    FSTRM_STAT_OK,  // all went well
    FSTRM_STAT_EOF, // end of file reached
    FSTRM_STAT_ERR, // ferror
} fstrm_stat;

typedef enum fstrm_open_code {
    FSTRM_OPEN_OK,          // stream opened successfully
    FSTRM_OPEN_ARG_ERR,     // bad argument to fst_open*()
    FSTRM_OPEN_FOPEN_ERR,   // fopen() failed
    FSTRM_OPEN_BUFF_ERR,    // setvbuf() failed
} fstrm_open_code;

fstrm_open_code fst_open_bscale(
    fstrm * fst, const char * fname, fstrm_mode mode, unsigned int bscale
);
#define fst_open(fst, fname, mode) fst_open_bscale((fst), (fname), (mode), 1)
/*
Returns: One of fstrm_open_code.

Description: Attempts to open the file specified by fname in the mode specified
by mode as a fully buffered stream. bscale is a scale value for the standard
library buffer and cannot be 0, since a buffer of size BUFSIZ*bscale is always
requested.
*/

void fst_close(fstrm * fst);
/*
Returns: Nothing.
Description: Closes the file associated with fst and zeroes out *fst.
*/

void fst_rewind(fstrm * fst);
/*
Returns: Nothing.
Description: Rewinds to the start of the file.
*/

fstrm_stat fst_wind_to(fstrm * fst, size_t ofs);
/*
Returns: One of fstrm_stat.
Description: Does fst_rewind() and reads bytes until ofs or EOF is reached.
*/

fstrm_stat fst_get_pos(fstrm * fst, fstrm_pos * pos);
/*
Returns: One of fstrm_stat.
Description: Saves the current file position in pos.
*/

fstrm_stat fst_set_pos(fstrm * fst, fstrm_pos * pos);
/*
Returns: One of fstrm_stat.
Description: Restores the stream to the file position pos, previously retrieved
by fst_get_pos().
*/

fstrm_stat fst_read_byte(fstrm * fst, unsigned char * out_val);
/*
Returns: One of fstrm_stat.
Description: Reads a byte from the file associated with fst.
*/

fstrm_stat fst_write_byte(fstrm * fst, unsigned char val);
/*
Returns: One of fstrm_stat.
Description: Writes a byte to the file associated with fst.
*/

size_t fst_def_bufsiz(void);
/*
Returns: BUFSIZ
Description: Returns the BUFSIZ value at the time of compilation.
*/
#endif
