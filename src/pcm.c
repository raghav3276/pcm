#include "pcm.h"
#include "pcm_apps.h"

#include <sys/time.h>

#define TO_USEC	((unsigned long) 1000000)

//char pcm_mem[PCM_MEM_SIZE];

void print_usage()
{
	printf("Usage: pcm <input_text_file> <num_threads> <bank_alloc_type>\n");

	printf("\nThe number of threads should be in the multiples of the number of banks(%u)\n", PCM_N_BANKS);

	printf("\nBank allocation types:\n");
	printf("0: Round robin\n");
	printf("1: Forced allocation to bank 0\n");
	printf("2: Random allocation\n");
}

int main(int argc, char *argv[])
{
	char *pcm_mem;
	unsigned long pcm_data_size;
	unsigned int pcm_n_threads;
	int bank_alloc_type;
	struct pcm_threads *pcm_threads;

	unsigned int i;
	char *temp_pcm;
	unsigned int granularity;
	char ch;

	struct timeval t1, t2;
	float execution_time;

	if (argc != 4) {
		print_usage();
		return -1;
	}

	sscanf(argv[2], "%u", &pcm_n_threads);
	if (pcm_n_threads % PCM_N_BANKS != 0) {
		printf("The number of threads should be in the multiples of the number of banks(%u)\n", PCM_N_BANKS);
		return -1;
	}

	pcm_word_cnt_init();

	printf("Before mem alloc..\n");

	pcm_mem = pcm_mem_alloc();
	if (!pcm_mem) {
		perror("Failed to allocate PCM");
		return errno;
	}

	printf("Mem allocated.. Now initializing..\n");

//	temp_pcm = pcm_mem;
//	granularity = 1024 * PCM_N_BANKS;
//	granularity = 1;

//	ariel_enable();

//	for (i = 0; i < PCM_MEM_SIZE / granularity ;i++) {
//		*temp_pcm = 'a';
//		temp_pcm += granularity;
//	}
//
//	printf("Done init.. Now reading\n");
//	temp_pcm = pcm_mem;
//	for (i = 0; i < PCM_MEM_SIZE / granularity; i++) {
//		ch = *temp_pcm;
////		printf("i: %d; ch = %c\n", i, ch);
//	}



//	printf("%p: %d\n", pcm_mem, *pcm_mem);


	pcm_data_size = pcm_mem_init(pcm_mem, argv[1]);
	if (pcm_data_size < 0) {
		perror("Failed to initialize PCM");
		goto out1;
	}

	printf("Done mem init..\n");

	sscanf(argv[3], "%d", &bank_alloc_type);

	gettimeofday(&t1, NULL);

	ariel_enable();

	pcm_threads = pcm_threads_spawn(pcm_n_threads, pcm_mem, bank_alloc_type);
	if (!pcm_threads) {
		perror("Failed to spawn the threads");
		return errno;
	}

	pcm_threads_join(pcm_threads, pcm_n_threads);

	gettimeofday(&t2, NULL);

	printf("Word count: %ld\n", pcm_word_cnt_get());

	execution_time = ((t2.tv_sec * TO_USEC + t2.tv_usec) - (t1.tv_sec * TO_USEC + t1.tv_usec)) / (float) 1000;
	printf("Time taken: %.2f ms\n", execution_time);

out1:
	pcm_mem_dealloc(pcm_mem);

	return errno;
}
