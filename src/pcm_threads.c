#include "pcm.h"
#include "pcm_apps.h"

void pcm_alloc_bank(struct pcm_threads *pcm_thread, int bank_alloc_type)
{
	switch (bank_alloc_type) {
	case 2:
		/* Random allocation */
		pcm_thread->bank = rand() % PCM_N_BANKS;
		break;

	case 1:
		/* Static forced allocation */
		pcm_thread->bank = 0;
		break;

	case 0:
	default:
		/* Round robin allocation */
		pcm_thread->bank = pcm_thread->id % PCM_N_BANKS;
	}
}

void *pcm_thread_func(void *data)
{
	struct pcm_threads *pcm_thread = (struct pcm_threads *) data;

	/* Access the bank's memory as a critical section of code */
	sem_wait(&pcm_bank_lock[pcm_thread->bank]);

	printf("Thread: %d; Bank: %d; mem_base: %p; mem_size: %ld\n",
			pcm_thread->id, pcm_thread->bank, pcm_thread->mem_base, pcm_thread->mem_size);

	pcm_word_cnt_local(pcm_thread->mem_base, pcm_thread->mem_size);

	sem_post(&pcm_bank_lock[pcm_thread->bank]);

	pthread_exit(0);
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

		pcm_alloc_bank(&pcm_threads[i], bank_alloc_type);

		pcm_threads[i].mem_size = mem_per_thread;
		pcm_threads[i].mem_base = mem +
				pcm_threads[i].bank * mem_per_bank +
				mem_per_thread * (i / PCM_N_BANKS);

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
