#include "pcm.h"
#include "pcm_apps.h"

#define N_ITERS	1024

void *pcm_thread_func(void *data)
{
	unsigned long local_word_cnt = 0;
	struct pcm_threads *pcm_thread = (struct pcm_threads *) data;
	unsigned int num_row_buffs = pcm_thread->mem_size / PCM_ROW_BUFF_SIZE;
	unsigned int i, j;
	char *mem;

	/* Access the bank's memory as a critical section of code */
//	sem_wait(&pcm_bank_lock[pcm_thread->bank]);

	printf("Thread: %d; Bank: %d; mem_base: %p; mem_size: %ld; num_row_buffs: %u\n",
			pcm_thread->id, pcm_thread->bank, pcm_thread->mem_base, pcm_thread->mem_size, num_row_buffs);

	for (i = 0; i < num_row_buffs; i++)
		local_word_cnt += pcm_word_cnt_local(
				pcm_thread->mem_base + (i * PCM_ROW_BUFF_SIZE * PCM_N_BANKS),
				PCM_ROW_BUFF_SIZE);

//	for (i = 0; i < N_ITERS; i++) {
//		mem = pcm_thread->mem_base;
//
//		for (j = 0; j < PCM_ROW_BUFF_SIZE; j++) {
//			*mem = 'a' + *mem;
//			mem++;
//		}
//	}


//	sem_post(&pcm_bank_lock[pcm_thread->bank]);

	pcm_word_cnt_update(local_word_cnt);

	pthread_exit(0);
}

void pcm_alloc_bank(struct pcm_threads *pcm_thread, char *mem, int bank_alloc_type)
{
	int id = pcm_thread->id;

	switch (bank_alloc_type) {
	case 2:
		/* Random allocation */
		pcm_thread->bank = rand() % PCM_N_BANKS;
		pcm_thread->mem_base = mem + (pcm_thread->bank * PCM_ROW_BUFF_SIZE);
		break;

	case 1:
		/* Static forced allocation */
		pcm_thread->bank = 0;
		pcm_thread->mem_base = mem + (id * PCM_N_BANKS * PCM_ROW_BUFF_SIZE);
		break;

	case 0:
	default:
		/* Round robin allocation */
		pcm_thread->bank = pcm_thread->id % PCM_N_BANKS;
		pcm_thread->mem_base = mem + (id * PCM_ROW_BUFF_SIZE);
	}
}

struct pcm_threads *pcm_threads_spawn(unsigned int n_threads, char *mem, int bank_alloc_type)
{
	int retval;
	unsigned int i;
	unsigned long mem_per_bank;
	unsigned long mem_per_thread;
	struct pcm_threads *pcm_threads;

	srand(time(NULL));

	pcm_threads = (struct pcm_threads *) calloc(n_threads, sizeof(*pcm_threads));
	if (!pcm_threads)
		return pcm_threads;

	mem_per_bank = PCM_MEM_SIZE / PCM_N_BANKS;
	mem_per_thread = PCM_MEM_SIZE / n_threads;

	for (i = 0; i < n_threads; i++) {

		pcm_threads[i].id = i;

		pcm_alloc_bank(&pcm_threads[i], mem, bank_alloc_type);

		pcm_threads[i].mem_size = mem_per_thread;


//		printf("thread:%d:: Bank assigned: %ld\n", i,
//				((unsigned long)pcm_threads[i].mem_base / PCM_ROW_BUFF_SIZE) % PCM_N_BANKS);

//		pcm_threads[i].mem_base = mem +
//				pcm_threads[i].bank * mem_per_bank +
//				mem_per_thread * (i / PCM_N_BANKS);

		retval = pthread_create(&pcm_threads[i].pthread, NULL, pcm_thread_func, &pcm_threads[i]);
		if (retval)
			return NULL;
	}

	return pcm_threads;
}

void pcm_threads_join(struct pcm_threads *pcm_threads, unsigned int n_threads)
{
	unsigned int i;

	for (i = 0; i < n_threads; i++) {
		pthread_join(pcm_threads[i].pthread, NULL);
	}

	free(pcm_threads);
}
