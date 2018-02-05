# Questions

## What's `stdint.h`?

A header file that provides "a set of typedefs that specify exact-width integer types,
together with the defined minimum and maximum allowable values for each type". Sounds
like a way maintain types consistently across different systems. So we can alias what
Microsoft wants to call a `BYTE` to what C calls an `uint8_t`.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

These clarify exactly how much memory an interger needs and how it will be represented,
which could otherwise vary across different systems. For example a particular system could
represent and integer with 8 bits or 16 bits, and the C language needs to know which.

Apparently it also documents your intent, and is a code style consideration. [1](https://stackoverflow.com/questions/1725855/uint8-t-vs-unsigned-char)

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE - 8 (uint8_t)
DWORD - 32 (uint16_t)
LONG - 32 (int32_t)
WORD - 16 (uint16_t)

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

According to [Microsoft documentation](https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx),
"BM" / 66 77 / (0x424d) / 01000010 01001011

## What's the difference between `bfSize` and `biSize`?

bfSize is the size of the file, biSize is the size of the BITMAPINFOHEADER structure

## What does it mean if `biHeight` is negative?

That the bitmap map isn't vertically flipped - the bits representing the top of the image
are at the top of the bitmap.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

There isn't enough memory for the file or you don't have read/write access.

## Why is the third argument to `fread` always `1` in our code?

Thats how many "blocks" you want to read. So if you're reading blocks the size of
a BM header, or the size of a BM triple, you want to only read one at a time.
If you specified something other than 1, you would read multiple blocks at a time.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3 (bytes)

## What does `fseek` do?

Skips reading ahead a certain amount (in this case).

## What is `SEEK_CUR`?

The current reading position.
