# Basic C Functions

## `fprintf`

`int fprintf (FILE *stream, const char *format, ...);`

writes the C string pointed by _format_ to the _stream_. if _format_ includes _format specifiers_, the additional argument following _format_ are formatted and isnerted in the resulting string replacing their respective specifiers.

## `sprintf`

`int sprintf (char *str, const char *format, ...)`

composes a string with the same text that would be printed if _format_ was used on `printf`, but instead of being printed, the content is stored as a C _string_ in the buffer pointed by _str_.

## `fopen`

`FILE *fopen (const char *filename, const char *mode);`

opens the file whose name is specified in the parameter _filename_ and associates it with a stream that can be indetified in the future operations by the FILE pointer returned.

### mode

* __r__ (read): open file for input operations. the file must exist.
* __w__ (write): create an empty file for output operations. if a file with the same name already exists it is overwritten.
* __a__ (append): open file for output at the end of a file. the file is created if it does not exists.
* __r+__ (read/update): open a file for input and output. the file must exist.
* __w+__ (write/update): create an empty file for input and output. if a file with the same name exists it is overwritten.
* __a+__ (append/update): open a file for input and output with all outputs being added to the end of the file. file is created if it does not exist.

## `fclose`

`int fclose (FILE *stream);`

closes the file associated with the _stream_ and disassociates it.

## `fwrite`

`size_t fwrite (const void *ptr, size_t size, size_t count, FILE *stream);`

writes an array of _count_ elements, each one with a size of _size_ bytes, from the block of memory pointed by _ptr_ to the current position in stream.

## `strcmp`

`int strcmp (const char *str1, const char *str2);`

compares the C string _str1_ to the C string _str2_.

## `strlen`

`size_t strlen (const char *str);`

returns the length of the C string _str_.

## `strtok`

`char *strtok (char *str, const char *delimiters);`

a sequence of calls to this function split _str_ into tokens, which are sequences of contiguous characters separated by any of the characters that are part of delimiters.

## `fgets`

`char *fgets (char *str, int num, FILE *stream);`

reads characters from _stream_ and stores them as a C string into _str_ until (_num_-1) characters have been read or either a newline of the end-of-file is reached. 
