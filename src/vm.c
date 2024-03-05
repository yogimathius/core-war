#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vm.h>

core_t *init_vm() {
    core_t *core = malloc(sizeof(core_t));
    // memset(core->memory, 0, MEM_SIZE);
    // memset(core->registers, 0, REG_NUMBER);
    // TODO: Use memset instead of for loop
    for (int i = 0; i < MEM_SIZE; i++) {
        core->memory[i] = 0;
    }

    // TODO: Use memset instead of for loop
    // TODO: free extra champions that don't get added eventually
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        const champion_t *champion = malloc(sizeof(champion_t));
        core->champions[i] = *champion;
    }
    core->champion_count = 0;
    core->instruction_pointer = 0;
    core->nbr_cycles = 0;
    core->cycle_to_die = CYCLE_TO_DIE;
    core->lives = 0;
    core->winner = 0;
    
    return core;
}

int read_vm(core_t *core_t, int address) {
    if (address < MEM_SIZE) {
        return core_t->memory[address];
    } else {
        printf("Error: Invalid memory address\n");
        return 1;
    }
}

void write_vm(core_t *core_t, int address, int value) {
    if (address < MEM_SIZE) {
        core_t->memory[address] = value;
    } else {
        printf("Error: Invalid memory address\n");
        // return 1;
    }
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

void load_instructions(core_t *core) {
    for (int i = 0; i < core->champion_count; i++) {
        champion_t *champion = &core->champions[i];
        for (int j = 0; j < champion->instruction_size; j++) {
            core->memory[j + (MEM_SIZE / core->champion_count) * i] = champion->inst[j].opcode;
        }
    }
}

void load_instructionsv2(core_t *core) {
    for (int i = 0; i < core->champion_count; i++) {
        champion_t *champion = &core->champions[i];
        for (int j = 0; j < champion->parsed_instructions_size; j++) {
            core->hex_memory[j + (MEM_SIZE / core->champion_count) * i] = champion->parsed_instructions[j];
        }
    }
}

int add_champion_to_core(core_t* core, champion_t* champion) {
    if (core->champion_count >= MAX_CHAMPIONS) {
        fprintf(stderr, "Maximum number of champions reached.\n");
        free(champion); // Ensure to free the champion if not added
        return -1;
    }
    core->champions[core->champion_count++] = *champion;
    return 0;
}