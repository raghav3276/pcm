#include "pcm.h"

/*
 * As only one access per bank is possible at a given time, locks
 * are taken to emulate the scenario.
 */
sem_t pcm_bank_lock[PCM_N_BANKS];

char *pcm_mem_alloc()
{
	int i;
	char *pcm_mem;

	pcm_mem = (char *) calloc(PCM_MEM_SIZE, sizeof(char));
	if (!pcm_mem) {
		return pcm_mem;
	}

	for (i = 0; i < PCM_N_BANKS; i++)
		sem_init(&pcm_bank_lock[i], 0, 1);

	return pcm_mem;
}

unsigned long pcm_mem_init(char *pcm_mem, char *file)
{
	int fd;
	unsigned long rd_cnt = 0, temp_rd_cnt;

	fd = open (file, O_RDONLY);
	if (fd < 0) {
		return fd;
	}

	while (1) {

		temp_rd_cnt = read(fd, pcm_mem + rd_cnt, PCM_INIT_CHUNK_SIZE);

		if (temp_rd_cnt < 0) {
			return temp_rd_cnt;
		} else {
			rd_cnt += temp_rd_cnt;

			if (temp_rd_cnt < PCM_INIT_CHUNK_SIZE || rd_cnt >= PCM_MEM_SIZE)
				break;
		}
	}

	return rd_cnt;
}
