#ifndef PCM_APPS_H_
#define PCM_APPS_H_

#include "pcm.h"

void pcm_word_cnt_init();
unsigned long pcm_word_cnt_local(char *pcm_mem, unsigned long mem_size);
void pcm_word_cnt_update(unsigned long local_word_cnt);
unsigned long pcm_word_cnt_get();

#endif /* PCM_APPS_H_ */
