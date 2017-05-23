#ifndef PCM_H_
#define PCM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include <arielapi.h>

#define PCM_N_BANKS		4
//#define PCM_BANK_SIZE	((unsigned long) 256 * 1024 * 1024)
#define PCM_BANK_SIZE	((unsigned long) 10 * 256 * 1024)
#define PCM_MEM_SIZE	PCM_N_BANKS * PCM_BANK_SIZE
#define PCM_ROW_BUFF_SIZE	1024	// 8 Kbits

//#define PCM_SEGMENTS_PER_BANK	1024
//#define	PCM_SEGMENT_SIZE		2048
//#define PCM_MEM_SIZE
//	(PCM_N_BANKS * PCM_SEGMENTS_PER_BANK * PCM_SEGMENT_SIZE)

#define PCM_INIT_CHUNK_SIZE	512

extern sem_t pcm_bank_lock[PCM_N_BANKS];

char *pcm_mem_alloc();
void pcm_mem_dealloc(char *pcm_mem);
unsigned long pcm_mem_init(char *pcm_mem, char *file);

struct pcm_threads {
	int id;
	int bank;

	char *mem_base;
	unsigned long mem_size;

	pthread_t pthread;
};

struct pcm_threads *pcm_threads_spawn(unsigned int n_threads, char *mem, int bank_alloc_type);
void pcm_threads_join(struct pcm_threads *pcm_threads, unsigned int n_threads);

#endif /* PCM_H_ */
