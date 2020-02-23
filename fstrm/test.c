#include "test.h"
#include "fstrm.h"
#include <stdio.h>
#include <string.h>

static bool test_open(void);
static bool test_open_read(void);
static bool test_open_update(void);
static bool test_open_write(void);

static char ftext[] = "The quick brown fox jumps over the lazy dog\n";
static char tfname[] = "test_file.txt";

static ftest tests[] = {
    test_open,
    test_open_read,
    test_open_update,
    test_open_write,
};
//------------------------------------------------------------------------------

static bool test_open_write(void)
{
    fstrm fst_, * fst = &fst_;

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_WRITE));

    unsigned char b;
    check(FSTRM_STAT_ERR == fst_read_byte(fst, &b));
    fst_close(fst);

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_WRITE));

    unsigned char write_what = 'x';
    check(FSTRM_STAT_OK == fst_write_byte(fst, write_what));
    fst_close(fst);

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_READ));

    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check(write_what == b);
    check(FSTRM_STAT_EOF == fst_read_byte(fst, &b));
    fst_close(fst);

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_WRITE));

    int sl = strlen(ftext);
    for (int i = 0; i < sl; ++i)
        check(FSTRM_STAT_OK == fst_write_byte(fst, ftext[i]));
    fst_close(fst);

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_READ));
    check(FSTRM_STAT_OK == fst_wind_to(fst, 4));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('q' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('u' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('i' == b);

    fst_close(fst);
    return true;
}
//------------------------------------------------------------------------------

static bool test_open_update(void)
{
    fstrm fst_, * fst = &fst_;

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_UPDATE));

    unsigned char b;
    check(FSTRM_STAT_OK == fst_wind_to(fst, 4));

    fstrm_pos quick_pos;
    check(FSTRM_STAT_OK == fst_get_pos(fst, &quick_pos));

    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('q' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('u' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('i' == b);

    fstrm_pos change_pos;
    check(FSTRM_STAT_OK == fst_get_pos(fst, &change_pos));

    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('c' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('k' == b);

    check(FSTRM_STAT_OK == fst_set_pos(fst, &change_pos));
    check(FSTRM_STAT_OK == fst_write_byte(fst, 'e'));
    check(FSTRM_STAT_OK == fst_write_byte(fst, 't'));

    check(FSTRM_STAT_OK == fst_set_pos(fst, &quick_pos));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('q' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('u' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('i' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('e' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('t' == b);

    check(FSTRM_STAT_OK == fst_set_pos(fst, &change_pos));
    check(FSTRM_STAT_OK == fst_write_byte(fst, 'c'));
    check(FSTRM_STAT_OK == fst_write_byte(fst, 'k'));

    check(FSTRM_STAT_OK == fst_set_pos(fst, &quick_pos));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('q' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('u' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('i' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('c' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('k' == b);

    fst_close(fst);
    return true;
}
//------------------------------------------------------------------------------

static bool test_open_read(void)
{
    fstrm fst_, * fst = &fst_;

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_READ));
    check(FSTRM_STAT_ERR == fst_write_byte(fst, 0x01));
    fst_close(fst);

    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_READ));

    unsigned char b;
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('T' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('h' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('e' == b);

    check(FSTRM_STAT_OK == fst_rewind(fst));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('T' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('h' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('e' == b);

    check(FSTRM_STAT_OK == fst_wind_to(fst, 4));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('q' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('u' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('i' == b);

    fstrm_pos fpos;
    check(FSTRM_STAT_OK == fst_get_pos(fst, &fpos));

    const int past_eof = 1000;
    check(FSTRM_STAT_EOF == fst_wind_to(fst, past_eof));
    check(FSTRM_STAT_EOF == fst_read_byte(fst, &b));

    check(FSTRM_STAT_OK == fst_set_pos(fst, &fpos));
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('c' == b);
    check(FSTRM_STAT_OK == fst_read_byte(fst, &b));
    check('k' == b);

    fst_close(fst);
    return true;
}
//------------------------------------------------------------------------------

static bool test_open(void)
{
    fstrm fst_, * fst = &fst_;

    check(FSTRM_OPEN_ARG_ERR == fst_open(fst, NULL, FSTRM_MODE_READ));

    const int not_proper_mode = 1234;
    check(FSTRM_OPEN_ARG_ERR == fst_open(fst, tfname, not_proper_mode));
    check(FSTRM_OPEN_ARG_ERR
        == fst_open_bscale(fst, tfname, FSTRM_MODE_READ, 0)
    );

    char bad_name[] = "abzsdberafdakhl.vcx";
    check(FSTRM_OPEN_FOPEN_ERR == fst_open(fst, bad_name, FSTRM_MODE_READ));
    /* can't force error
    check(FSTRM_OPEN_FILE_OP_ERR
        == fst_open_bscale(fst, tfname, FSTRM_MODE_READ, ~0)
    );
    */
    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_READ));
    fst_close(fst);


    check(FSTRM_OPEN_OK == fst_open(fst, tfname, FSTRM_MODE_UPDATE));
    fst_close(fst);
    return true;
}
//------------------------------------------------------------------------------

void run_tests(void)
{
    int i, end = sizeof(tests)/sizeof(*tests);

    int passed = 0;
    for (i = 0; i < end; ++i)
        if (tests[i]())
            ++passed;

    if (passed != end)
        putchar('\n');

    int failed = end - passed;
    report(passed, failed);
    return;
}
//------------------------------------------------------------------------------

int main(void)
{
	run_tests();
	return 0;
}
//------------------------------------------------------------------------------
