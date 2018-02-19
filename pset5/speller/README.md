# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

An artificial long word said to mean a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?

Gets resource usage.

## Per that same man page, how many members are in a variable of type `struct rusage`?

16.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Because the function expects a pointer? ;)

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

The for loop begins by reading the first character of the file. The loop checks that the
read character is not the end of the file. If the read character is not the end of the file,
then the loop executes. On completion of one loop iteration, the next character in the file
is read (where typically the loop might increment a counter, e.g. i++), and the process is
repeated until the end of the file condition is met.

Inside the loop as each character is a read a "word" buffer is populated with letters.
If the string is too long or contains a digit, the word buffer is reset and the remainder
of the string is read (like seeking in a file), effectively skipping the string.

Finally the word in the buffer is completed with a null terminator "\0".

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

Based on the currently implementation using fgetc, I would think the issue with fscanf is
that you couldn't detect words with digits?

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

So that the check and load functions don't modify the word or dictionary, respectively.
