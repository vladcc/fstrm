#include "fstrm.h"
#include <stdio.h>
#include <string.h>

fstrm_open_code fst_open_bscale(
    fstrm * fst, const char * fname, fstrm_mode mode, unsigned int bscale
)
{
    static char mode_rb[] = "rb";
    static char mode_wb[] = "wb";
    static char mode_up[] = "r+b";

    if (!fname
        || (mode != FSTRM_MODE_READ
            && mode != FSTRM_MODE_UPDATE
            && mode != FSTRM_MODE_WRITE)
        || !bscale)
        return FSTRM_OPEN_ARG_ERR;

    memset(fst, 0, sizeof(*fst));

    FILE * pfile = NULL;
    char * mode_str = NULL;
    switch (mode)
    {
        case FSTRM_MODE_READ: mode_str = mode_rb; break;
        case FSTRM_MODE_WRITE: mode_str = mode_wb; break;
        case FSTRM_MODE_UPDATE: mode_str = mode_up; break;
        default: break;
    }
    pfile = fopen(fname, mode_str);

    if (!pfile)
        return FSTRM_OPEN_FOPEN_ERR;

    fpos_t start;
    if ((setvbuf(pfile, NULL, _IOFBF, BUFSIZ*bscale) != 0)
        || (fgetpos(pfile, &start) != 0))
    {
        fclose(pfile);
        return FSTRM_OPEN_OP_ERR;
    }

    fst->pfile = pfile;
    fst->start = start;

    return FSTRM_OPEN_OK;
}
//------------------------------------------------------------------------------

void fst_close(fstrm * fst)
{
    fclose(fst->pfile);
    memset(fst, 0, sizeof(*fst));
}
//------------------------------------------------------------------------------

fstrm_stat fst_rewind(fstrm * fst)
{
    return (fsetpos(fst->pfile, &(fst->start)) == 0) ?
        FSTRM_STAT_OK : FSTRM_STAT_ERR;
}
//------------------------------------------------------------------------------

fstrm_stat fst_wind_to(fstrm * fst, size_t ofs)
{
    if (fst_rewind(fst) != FSTRM_STAT_OK)
        return FSTRM_STAT_ERR;

    unsigned char b;
    for (size_t i = 0; i < ofs; ++i)
    {
        if (fread(&b, 1, 1, fst->pfile) == 1)
            continue;

        return (feof(fst->pfile)) ? FSTRM_STAT_EOF : FSTRM_STAT_ERR;
    }

    return FSTRM_STAT_OK;
}
//------------------------------------------------------------------------------

fstrm_stat fst_get_pos(fstrm * fst, fstrm_pos * pos)
{
    fpos_t this_pos;
    if (fgetpos(fst->pfile, &this_pos) == 0)
    {
        pos->pos = this_pos;
        return FSTRM_STAT_OK;
    }

    return FSTRM_STAT_ERR;
}
//------------------------------------------------------------------------------

fstrm_stat fst_set_pos(fstrm * fst, fstrm_pos * pos)
{
    return (fsetpos(fst->pfile, &(pos->pos)) == 0) ?
            FSTRM_STAT_OK : FSTRM_STAT_ERR;
}
//------------------------------------------------------------------------------

fstrm_stat fst_read_byte(fstrm * fst, unsigned char * out_val)
{
    if (fread(out_val, 1, 1, fst->pfile) == 1)
        return FSTRM_STAT_OK;

    if (feof(fst->pfile))
        return FSTRM_STAT_EOF;

    return FSTRM_STAT_ERR;
}
//------------------------------------------------------------------------------

fstrm_stat fst_write_byte(fstrm * fst, unsigned char val)
{
    return (fwrite(&val, 1, 1, fst->pfile) == 1) ?
            FSTRM_STAT_OK : FSTRM_STAT_ERR;
}
//------------------------------------------------------------------------------

size_t fst_def_bufsiz(void)
{
    return BUFSIZ;
}
//------------------------------------------------------------------------------
