#include "pcm.h"
#include "pcm_apps.h"

#include <sys/time.h>

#define TO_USEC	((unsigned long) 1000000)

int main(int argc, char *argv[])
{
	char *pcm_mem;
	unsigned long pcm_data_size;
	unsigned int pcm_n_threads;
	struct pcm_threads *pcm_threads;

	struct timeval t1, t2;
	float execution_time;

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

	pcm_word_cnt_init();

	pcm_mem = pcm_mem_alloc();
	if (!pcm_mem) {
		perror("Failed to allocate PCM");
		return errno;
	}

	pcm_data_size = pcm_mem_init(pcm_mem, argv[1]);
	if (pcm_data_size < 0) {
		perror("Failed to initialize PCM");
		goto out1;
	}

	pcm_threads = pcm_threads_spawn(pcm_n_threads, pcm_mem);
	if (!pcm_threads) {
		perror("Failed to spawn the threads");
		return errno;
	}

	gettimeofday(&t1, NULL);

	pcm_threads_join(pcm_threads, pcm_n_threads);

	gettimeofday(&t2, NULL);

	printf("Word count: %ld\n", pcm_word_cnt_get());

	execution_time = ((t2.tv_sec * TO_USEC + t2.tv_usec) - (t1.tv_sec * TO_USEC + t1.tv_usec)) / (float) 1000;
	printf("Time taken: %.2f ms\n", execution_time);

out1:
	pcm_mem_dealloc(pcm_mem);

	return errno;
}
