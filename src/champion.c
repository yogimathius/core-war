#include <champion.h>

champion_t *init_champion() {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);

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
      printf("Magic value equals 0xea83f3!\n");
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
      if (i < 132) {
          champion->header.prog_name[i - 4] = hex_buffer[i];
      } else if (i > 136 && i < 140) {
          // printf("hex_buffer[i]: %d\n", hex_buffer[i]);

          champion->header.prog_size = (champion->header.prog_size << 8) | (unsigned char)hex_buffer[i];
      } else if (i >= 140 && i < 140 + COMMENT_LENGTH) {
          champion->header.comment[i - 140] = hex_buffer[i];
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
    int bytes_read = read(fd, hex_buffer, sizeof(hex_buffer) - 1);

    if (bytes_read == -1) {
      printf("Error: could not read file\n");
      exit(1);
    }

    if (parse_header(champion, bytes_read, hex_buffer) == 1) {
      printf("Error: could not parse header\n");
      exit(1);
    }

    char **parsed_instructions = parse_instructions(hex_buffer, bytes_read, champion);
    champion->instruction_list = NULL;
    build_instructions(champion, parsed_instructions, &champion->instruction_list);
    free(parsed_instructions);

  } else {
      perror("Error getting file size");
  }
  
  return champion;
}

void add_champion(core_t *core_t, champion_t *champion) {
  core_t->champion_count+=1;
  champion->id = core_t->champion_count;
  core_t->champions[core_t->champion_count - 1] = *champion;
  champion->counter = 0;
  champion->carry_flag = 0;
  champion->registers[0] = champion->id;
}

void run_champion(core_t *vm, champion_t champion) {
    instruction_t *inst = champion.instruction_list;
    while (inst != NULL && inst->opcode != -1) {
        if (inst->opcode < 0 || inst->opcode >= 16) {
            printf("Invalid opcode: %d\n", inst->opcode);
            break;
        }
        printf("instruction from run_program: %s\n", op_tab[inst->opcode].mnemonique);

        if (inst->operands != NULL) {
          int i = 0;
          printf("operands in run champion: ");
          while (i < op_tab[inst->opcode].nbr_args) {
              if (inst->operands[i] < 0 || inst->operands[i] >= MEM_SIZE) {
                printf("Invalid operand: %d\n", inst->operands[i]);
                return;
              }
              printf("%d ", inst->operands[i]);
              i++;
          }
        }
        printf("\n");
        execute_instruction(vm, &champion, inst->opcode, inst->operands);
        inst = inst->next;
    }
}