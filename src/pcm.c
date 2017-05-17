#include "pcm.h"

struct pcm_threads *pcm_threads;

unsigned long pcm_word_cnt(char *pcm_mem)
{
	unsigned long word_cnt = 1;

	if (!pcm_mem)
		return 0;

	while (*pcm_mem) {
		if (*pcm_mem == ' ' || *pcm_mem == '\n' || *pcm_mem == '\t')
			word_cnt++;

		pcm_mem++;
	}

	return word_cnt;
}

int main(int argc, char *argv[])
{
	char *pcm_mem;
	unsigned long pcm_data_size;
	unsigned int pcm_n_threads;

	if (argc != 3) {
		printf("Usage: pcm <input_text_file> <num_threads>\n");
		printf("\nThe number of threads should be in the multiples of the number of banks(%u)\n", PCM_N_BANKS);
		return -1;
	}

	sscanf(argv[2], "%u", &pcm_n_threads);
	if (pcm_n_threads % PCM_N_BANKS != 0) {
		printf("The number of threads should be in the multiples of the number of banks(%u)\n", PCM_N_BANKS);
		return -1;
	}

	pcm_mem = pcm_mem_alloc();
	if (!pcm_mem) {
		perror("Failed to allocate PCM");
		return errno;
	}

	pcm_data_size = pcm_mem_init(pcm_mem, argv[1]);
	if (pcm_data_size < 0) {
		perror("Failed to initialize PCM");
		return errno;
	}

	pcm_threads = pcm_threads_spawn(pcm_n_threads, pcm_mem);
	if (!pcm_threads) {
		perror("Failed to spawn the threads");
		return errno;
	}

	pcm_threads_join(pcm_threads, pcm_n_threads);

	printf("Word count: %ld\n", pcm_word_cnt(pcm_mem));

	pcm_mem_dealloc(pcm_mem);

	return 0;
}
