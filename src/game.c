#include <game.h>
#include <champion.h>

/**
 *  Phase 4: we want each champion to ONLY call one instruction per turn. EG:
 *  Define the number of instructions that each champion should run
 * const numberOfInstructions = core.champions[0].instructions.length;

 Game loop 1
 champion 1 executes inst 1,
 champion 2 executes inst 1,
 champion 3 executes inst 1,
 Game loop 2
 champion 1 executes inst 2,
 champion 2 executes inst 2,
 champion 3 executes inst 2,
 Game loop 3
 champion 1 executes inst 3,
 champion 2 executes inst 3,
 champion 3 executes inst 3,
 looping through each instruction
 for (let gameloopNumber = 0; gameloopNumber < numberOfLoops; gameloopNumber++){
    looping through each champion
   for (let championNumber = 0; championNumber < core.champions.length; championNumber++){
      get the current champion
     const champion = core.champions[championNumber];

      get the instruction for the current champion
     const instruction = champion.instructions[gameloopNumber] 
     console.log(`champion ${champion.name} is executing: ` )
     instruction();
   }
 }
 * 
*/

void run_game(core_t *core_vm) {
    printf("\n\n");
    printf("====================START GAME=====================\n");
    // Have to create a game loop just like the one in the snippet above that will loop through each champion and execute one instruction at a time
    // Phase 5: we want each champion to call ONLY ONE instruction per turn.
    int game_loop_number = 0;
    int max_game_loops = 10;

    while (game_loop_number < max_game_loops) {
        printf("\n");
        printf("====================GAME LOOP %d=====================\n\n", game_loop_number);
        run_champions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
    }
}