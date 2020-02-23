# fstrm
binary mode file stream

Emulates random access to a file while sticking to the standard. Uses
fsetpos(), fgetpos() instead of fseek(), ftell(). Allows for read, write,
and read/update. Reads a single byte at a time, supports scaling the
standard library io buffer.
