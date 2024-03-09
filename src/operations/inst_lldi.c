#include <op.h>
#include <instructions.h>

/* 
Same as ldi without the %IDX_MOD. Modifies the carry
*/

int calculate_address_for_lldi(int arg1, int arg2) {
    return arg1 + arg2; // Removed % IDX_MOD
}

int inst_lldi(champion_t *champion, core_t *core, code_t code, int *inst) {
    UNUSED(core);
    UNUSED(code);
    int arg1 = champion->registers[inst[0]];
    int arg2 = champion->registers[inst[1]];
    int address = calculate_address_for_lldi(arg1, arg2);

    int value_to_load = load_value_from_address(core, address);
    champion->registers[inst[2]] = value_to_load; // Update the target register

    // Update carry flag based on the loaded value
    champion->carry_flag = (value_to_load == 0) ? 1 : 0;

    return 0;
}