#include <champion.h>

void load_champions(core_t *core_vm, int ac, char **av) {
  printf("====================LOADING CHAMPIONS=====================\n");
  while (ac > 1) {
    printf("Loading champion: %s\n", av[ac - 1]);
    champion_t *champ = init_champion();
    create_champion(champ, av[ac - 1]);
    add_champion(core_vm, champ);
    ac--;
  }
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

char bin_to_hex(unsigned int byte) {
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

    char **parsed_instructions = parse_instructions(hex_buffer, bytes_read);
    champion->instruction_list = NULL;
    int instruction_size = build_instructions(parsed_instructions, &champion->instruction_list, champion);
    champion->instruction_size = instruction_size;
    free(parsed_instructions);
  } else {
      perror("Error getting file size");
  }
  
  return champion;
}


void add_champion(core_t *core_t, champion_t *champion) {
  // List of champ colors (RED, GREEN, BLUE, CYAN)
  const int champ_color[4] = {31, 32, 35, 36};

  core_t->champion_count+=1;
  champion->id = core_t->champion_count;
  champion->counter = 0;
  champion->carry_flag = 0;
  champion->registers[0] = champion->id;
  champion->color = champ_color[champion->id - 1];
  core_t->champions[core_t->champion_count - 1] = *champion;
}

void run_champion(core_t *vm, champion_t champion) {
    int instruction_size = champion.instruction_size;
    int instruction_pointer = vm->instruction_pointer;

    int instruction_index = instruction_pointer >= instruction_size ? (instruction_pointer % instruction_size) : instruction_pointer;

    instruction_t *found_inst = &champion.inst[instruction_index];

    if (found_inst->opcode < 0 || found_inst->opcode > 16) {
        printf("Invalid opcode for operands: %d\n", found_inst->opcode);
        return;
    }


    execute_instruction(vm, &champion, found_inst->opcode, found_inst->operands);
}

void run_champions(core_t *vm) {
    for (int i = 0; i < vm->champion_count; i++) {
        run_champion(vm, vm->champions[i]);
    }
}