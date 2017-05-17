#include "pcm.h"

/* Word Count Application */

struct pcm_word_cnt {
	unsigned long word_cnt;
	sem_t word_cnt_lock;
};

static struct pcm_word_cnt pcm_word_cnt;

void pcm_word_cnt_update(unsigned long local_word_cnt)
{
	sem_wait(&pcm_word_cnt.word_cnt_lock);
	pcm_word_cnt.word_cnt += local_word_cnt;
	sem_post(&pcm_word_cnt.word_cnt_lock);
}

unsigned long pcm_word_cnt_local(char *pcm_mem, unsigned long mem_size)
{
	unsigned long word_cnt = 1;

	if (!pcm_mem)
		return 0;

	while (*pcm_mem && mem_size) {

		if (*pcm_mem == ' ' || *pcm_mem == '\n' || *pcm_mem == '\t')
			word_cnt++;

		pcm_mem++;
		mem_size--;
	}

	return word_cnt;
}

unsigned long pcm_word_cnt_get()
{
	unsigned long word_cnt;

	sem_wait(&pcm_word_cnt.word_cnt_lock);
	word_cnt = pcm_word_cnt.word_cnt;
	sem_post(&pcm_word_cnt.word_cnt_lock);

	return word_cnt;
}

void pcm_word_cnt_init()
{
	sem_init(&pcm_word_cnt.word_cnt_lock, 0, 1);
	pcm_word_cnt.word_cnt = 0;
}
