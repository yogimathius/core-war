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

int parse_header(champion_t *champion, int bytes_read, char *hex_buffer) {
  int i;
  unsigned int magic_value = 0;
  for (i = 0; i < bytes_read && i < 4; i++) {
      magic_value = (magic_value << 8) | (unsigned char)hex_buffer[i];
  }

  if (magic_value == 0xea83f3) {
      champion->header.magic = magic_value;
      printf("Magic value equals 0xea83f3!\n");
  } else {
      printf("Magic value does not equal 0xea.\n");
      return 1;
  }

  for (i = 4; i < bytes_read && i < 132; i++) {
      champion->header.prog_name[i - 4] = hex_buffer[i];
  }

  for (i = 136; i < bytes_read && i < 140; i++) {
      champion->header.prog_size = (champion->header.prog_size << 8) | (unsigned char)hex_buffer[i];
  }

  for (i = 140; i < 140 + COMMENT_LENGTH; i++) {
      champion->header.comment[i - 140] = hex_buffer[i];
  }

  printf("Program name: %s\n", champion->header.prog_name);
  printf("Program size: %d\n", champion->header.prog_size);
  printf("Comment: %s\n", champion->header.comment);
  return 0;
}

char bin_to_hex(uint8_t byte) {
    return byte < 10 ? byte + '0' : byte - 10 + 'a';
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

    if (parse_header(champion, bytes_read, hex_buffer) == 1) {
      printf("Error: could not parse header\n");
      exit(1);
    }

    int j = 0; // Index for inst array
    for (int i = 140 + COMMENT_LENGTH; i < bytes_read && j < MEM_SIZE; i++) {
        if (hex_buffer[i] != 0) {
            champion->inst[j] = hex_buffer[i];
            j++;
        }
    }

    champion->inst[j] = '\0';

    char *hex_string = (char *)malloc(st.st_size * 2 + 1);

    j = 0;
    for (int i = 140 + COMMENT_LENGTH; i < bytes_read && j < MEM_SIZE; i++) {
      if (hex_buffer[i] != 0) {
        hex_string[j * 3]     = bin_to_hex(hex_buffer[i] >> 4);
        hex_string[j * 3 + 1] = bin_to_hex(hex_buffer[i] & 0x0F);
        hex_string[j * 3 + 2] = ' '; 
        j++;
      }
    }
    hex_string[st.st_size * 3] = '\0';
    champion->instruction_size = j;
    printf("Hexadecimal representation: %s\n", hex_string);
  
    champion->instructions = hex_string;
    
  } else {
      perror("Error getting file size");
  }
  
  return champion;
}

void add_champion(core_t *core_t, champion_t *champion) {
  core_t->champion_count++;
  champion->id = core_t->champion_count;
  core_t->champions[core_t->champion_count - 1] = *champion;
  champion->counter = 0;
  champion->carry_flag = 0;
  champion->instructions = NULL;
}
