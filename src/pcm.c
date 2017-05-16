#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define PCM_N_BANKS				4
#define PCM_SEGMENTS_PER_BANK	1024
#define	PCM_SEGMENT_SIZE		2048
#define PCM_MEM_SIZE	\
	(PCM_N_BANKS * PCM_SEGMENTS_PER_BANK * PCM_SEGMENT_SIZE)

#define PCM_INIT_CHUNK_SIZE	512

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

char *pcm_mem_alloc()
{
	char *pcm_mem;

	pcm_mem = (char *) calloc(PCM_MEM_SIZE, sizeof(char));
	if (!pcm_mem) {
		return pcm_mem;
	}

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

int main(int argc, char *argv[])
{
	char *pcm_mem;
	unsigned long pcm_data_size;

	if (argc != 2) {
		printf("Usage: pcm <input_text_file>\n");
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

	printf("%s\n\n", pcm_mem);

	printf("Word count: %ld\n", pcm_word_cnt(pcm_mem));

	return 0;
}
