#include <champion.h>

void load_champions(core_t *core_vm, int ac, char **av) {
  printf("====================LOADING CHAMPIONS=====================\n");
  int i = 2;
  while (i <= ac) {
    printf("Loading champion: %s\n", av[i - 1]);
    champion_t *champ = init_champion();
    create_champion(champ, av[i - 1]);
    add_champion(core_vm, champ);
    i++;
  }
  load_instructions(core_vm);
  load_instructionsv2(core_vm);
  build_processes(core_vm);
}

champion_t *init_champion() {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);
  // initialize all registers to 0
  for (int i = 0; i < REG_NUMBER; i++) {
    champ->registers[i] = 0;
  }
  for (int i = 0; i < MEM_SIZE; i++) {
    const instruction_t *inst = malloc(sizeof(instruction_t));
    champ->inst[i] = *inst;
  }
  champ->id = 0;
	return champ;
}

int get_magic_value(champion_t *champion, char *hex_buffer) {
  int i;
  unsigned int magic_value = 0;
  for (i = 0; i < 4; i++) {
      magic_value = (magic_value << 8) | (unsigned char)hex_buffer[i];
  }
  champion->header.magic = magic_value;

  if (magic_value == COREWAR_EXEC_MAGIC) {
      champion->header.magic = magic_value;
      return 0;
  } else {
      printf("Magic value does not equal 0xea.\n");
      return 1;
  }
}

int parse_header(champion_t *champion, int bytes_read, char *hex_buffer) {
  if (get_magic_value(champion, hex_buffer) == 1) {
    return 1;
  }

  for (int i = 4; i < bytes_read && i < 140 + COMMENT_LENGTH; i++) {
      if (i < PROG_NAME_LENGTH + 4) {
          champion->header.prog_name[i - 4] = hex_buffer[i];
      } else if (i > PROG_NAME_LENGTH && i < PROG_NAME_LENGTH + 8) {
          while (hex_buffer[i] == 00) { 
              i++;
          }
          char *hex_value = (char*)malloc(3 * sizeof(char));
          snprintf(hex_value, 3, "%.2x", (unsigned int)hex_buffer[i]);
          champion->header.prog_size = strtol(hex_value, NULL, 16);
      } else if (i >= PROG_NAME_LENGTH + 12 && i < PROG_NAME_LENGTH + 12 + COMMENT_LENGTH) {
          champion->header.comment[i - (PROG_NAME_LENGTH + 12)] = hex_buffer[i];
      }
  }
  printf("Program name: %s\n", champion->header.prog_name);
  printf("Program size: %d\n", champion->header.prog_size);
  printf("Comment: %s\n", champion->header.comment);

  return 0;
}

champion_t *create_champion(champion_t *champion, char *filename) {
  int fd = open(filename, O_RDONLY);

  if (fd == -1) {
    printf("Error: could not open file\n");
    exit(1);
  }

  struct stat st;

  if (stat(filename, &st) == 0) {
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

    parse_instructions(hex_buffer, bytes_read, champion);
  } else {
      perror("Error getting file size");
  }
  
  return champion;
}

champion_t* clone_champion(const champion_t* original) {
    champion_t* clone = (champion_t*)malloc(sizeof(champion_t));
    if (!clone) {
        perror("Failed to allocate memory for clone champion");
        return NULL;
    }

    clone->id = original->id;
    clone->header = original->header;
    clone->counter = original->counter;
    clone->carry_flag = original->carry_flag;
    clone->instruction_size = original->instruction_size;
    clone->color = original->color;
    clone->lives = original->lives;
    clone->dead = original->dead;

    memcpy(clone->registers, original->registers, sizeof(original->registers));
    memcpy(clone->inst, original->inst, sizeof(original->inst));
    return clone;
}