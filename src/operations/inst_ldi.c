#include <op.h>
#include <instructions.h>

/* 
Takes 3 parameters. First two are indexes and the third one is a register
Ex: ldi 3, %4, r1 -> reads IND_SIZ bytes from address Counter + 3 % IDX_MOD, add 4 to this value (SumResult). REG_SIZE byte are read from the Counter + SumResult % IDX_MOD and copies to r1.
*/

int calculate_address_for_ldi(int arg1, int arg2) {
    return (arg1 + arg2) % IDX_MOD; 
}

int inst_ldi(champion_t *champion, core_t *core, code_t code, int *inst) {
    log_instruction_args(champion, core, code, inst);

    int arg1 = champion->registers[inst[0]];
    int arg2 = champion->registers[inst[1]];
    int address = calculate_address_for_ldi(arg1, arg2);

    int value_to_load = load_value_from_address(core, address);
    champion->registers[inst[2]] = value_to_load; // Update the target register

    // Update carry flag based on the loaded value
    champion->carry_flag = (value_to_load == 0) ? 1 : 0;

    return 0;
}

