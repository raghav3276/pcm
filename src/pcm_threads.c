#include "pcm.h"

void pcm_alloc_bank(struct pcm_threads *pcm_thread)
{
	/* Round robin allocation */
	pcm_thread->bank = pcm_thread->id % PCM_N_BANKS;
}

void *pcm_thread_func(void *data)
{
	struct pcm_threads *pcm_thread = (struct pcm_threads *) data;

	pcm_alloc_bank(pcm_thread);

	/* Access the bank's memory as a critical section of code */
	sem_wait(&pcm_bank_lock[pcm_thread->bank]);

	printf("Thread: %d; Bank: %d\n", pcm_thread->id, pcm_thread->bank);

	sem_post(&pcm_bank_lock[pcm_thread->bank]);

	pthread_exit(0);
}

struct pcm_threads *pcm_threads_spawn(unsigned int n_threads)
{
	int retval;
	unsigned int i;
	struct pcm_threads *pcm_threads;

	pcm_threads = (struct pcm_threads *) calloc(n_threads, sizeof(*pcm_threads));
	if (!pcm_threads)
		return pcm_threads;

	for (i = 0; i < n_threads; i++) {

		pcm_threads[i].id = i;

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
}
