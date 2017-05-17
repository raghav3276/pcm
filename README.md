# pcm

### Compiling:

$ cd pcm/Debug/
$ make


### Usage:
$ ./pcm <input_text_file> <num_threads> <bank_alloc_type>

The number of threads should be in the multiples of the number of banks(4)


Bank allocation types:

0: Round robin

1: Forced allocation to bank 0

2: Random allocation
