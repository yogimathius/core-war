#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "../include/champion.h"
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

champion_t *init_champion() {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);

	return champ;
}

champion_t *create_champion(champion_t *champion, char *filename) {
  int fd = open(filename, O_RDONLY);

  if (fd == -1) {
    printf("Error: could not open file\n");
    exit(1);
  }

  struct stat st;

  if (stat(filename, &st) == 0) {
    printf("File size of %s: %lld bytes\n", filename, (long long)st.st_size);
    char hex_buffer[st.st_size];
    int bytes_read = read(fd, hex_buffer, sizeof(hex_buffer));
  
    if (bytes_read == -1) {
      printf("Error: could not read file\n");
      exit(1);
    }

    unsigned int magic_value = 0;

    int i;
    for (i = 0; i < bytes_read && i < 4; i++) {
        magic_value = (magic_value << 8) | (unsigned char)hex_buffer[i];
    }

    if (magic_value == 0xea83f3) {
        champion->header.magic = magic_value;
        printf("Magic value equals 0xea83f3!\n");
    } else {
        printf("Magic value does not equal 0xea.\n");
        return NULL;
    }

    for (i = 4; i < bytes_read && i < 132; i++) {
        champion->header.prog_name[i - 4] = hex_buffer[i];
    }

    for (i = 136; i < bytes_read && i < 140; i++) {
        champion->header.prog_size = (champion->header.prog_size << 8) | (unsigned char)hex_buffer[i];
    }

    for (i = 140; i < bytes_read && i < 2140; i++) {
        champion->header.comment[i - 140] = hex_buffer[i];
    }

    printf("Program name: %s\n", champion->header.prog_name);
    printf("Program size: %d\n", champion->header.prog_size);
    printf("Comment: %s\n", champion->header.comment);


  } else {
      perror("Error getting file size");
  }
  
  printf("\n");
  UNUSED(champion);
  UNUSED(fd);
  return champion;
}

void add_champion(core_t *core_t, champion_t *champion) {
  core_t->champion_count++;
  champion->id = core_t->champion_count;
  core_t->champions[core_t->champion_count - 1] = *champion;
  champion->counter = 0;
  champion->carry_flag = 0;
}
