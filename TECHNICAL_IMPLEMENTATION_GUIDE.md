# Core War Rust Implementation Technical Guide

## Core Data Structures

### Virtual Machine Core

```rust
// Core virtual machine state
pub struct CoreWar {
    memory: Vec<u8>,                    // 6KB circular memory
    processes: Vec<Process>,            // Active processes
    champions: Vec<Champion>,           // Loaded champions
    cycle_count: u32,                   // Current cycle
    cycle_to_die: u32,                  // Cycles until death check
    lives_in_period: u32,              // Lives declared in current period
    winner: Option<usize>,              // Winner champion ID
    config: GameConfig,                 // Game configuration
}

// Process representation
pub struct Process {
    id: usize,                          // Unique process ID
    player_id: usize,                   // Owner champion ID
    pc: usize,                          // Program counter
    registers: [i32; 16],               // r1-r16 registers
    carry_flag: bool,                   // Carry flag
    cycles_to_wait: u32,                // Cycles until next execution
    last_live_cycle: u32,               // Last cycle when 'live' was called
    alive: bool,                        // Process status
}

// Champion representation
pub struct Champion {
    id: usize,                          // Unique champion ID
    name: String,                       // Champion name
    comment: String,                    // Champion comment
    code: Vec<u8>,                      // Compiled bytecode
    size: usize,                        // Code size
    load_address: usize,                // Memory load address
    color: Color,                       // Terminal display color
    process_count: usize,               // Number of active processes
    last_live_cycle: u32,               // Last cycle when any process called 'live'
}

// Instruction representation
pub struct Instruction {
    opcode: u8,                         // Instruction opcode
    param_types: u8,                    // Parameter type descriptor
    params: Vec<i32>,                   // Parameter values
    cycles: u32,                        // Execution cycles required
}
```

### Memory Management

```rust
// Memory operations with bounds checking
impl CoreWar {
    pub fn read_memory(&self, address: usize) -> u8 {
        self.memory[address % MEM_SIZE]
    }
    
    pub fn write_memory(&mut self, address: usize, value: u8) {
        let addr = address % MEM_SIZE;
        self.memory[addr] = value;
    }
    
    pub fn read_int(&self, address: usize) -> i32 {
        let addr = address % MEM_SIZE;
        i32::from_le_bytes([
            self.memory[addr],
            self.memory[(addr + 1) % MEM_SIZE],
            self.memory[(addr + 2) % MEM_SIZE],
            self.memory[(addr + 3) % MEM_SIZE],
        ])
    }
    
    pub fn write_int(&mut self, address: usize, value: i32) {
        let bytes = value.to_le_bytes();
        for (i, &byte) in bytes.iter().enumerate() {
            self.memory[(address + i) % MEM_SIZE] = byte;
        }
    }
}
```

### Instruction Set Implementation

```rust
// Instruction execution trait
pub trait Instruction {
    fn execute(&self, process: &mut Process, core: &mut CoreWar) -> Result<(), VMError>;
    fn cycles(&self) -> u32;
    fn validate_params(&self, params: &[Parameter]) -> Result<(), VMError>;
}

// Parameter types
#[derive(Debug, Clone)]
pub enum Parameter {
    Register(u8),                       // Register r1-r16
    Direct(i32),                        // Direct value
    Indirect(i32),                      // Indirect address
}

// Instruction implementations
pub struct LiveInstruction;
impl Instruction for LiveInstruction {
    fn execute(&self, process: &mut Process, core: &mut CoreWar) -> Result<(), VMError> {
        // Mark process as alive
        process.last_live_cycle = core.cycle_count;
        core.lives_in_period += 1;
        
        // Display live message
        println!("Player {} ({}) is alive", process.player_id, 
                 core.champions[process.player_id].name);
        
        Ok(())
    }
    
    fn cycles(&self) -> u32 { 10 }
}

pub struct LoadInstruction;
impl Instruction for LoadInstruction {
    fn execute(&self, process: &mut Process, core: &mut CoreWar) -> Result<(), VMError> {
        // Implementation for LD instruction
        // Load value from memory/direct into register
        // Set carry flag based on loaded value
        Ok(())
    }
    
    fn cycles(&self) -> u32 { 5 }
}

// Fork instruction creates new process
pub struct ForkInstruction;
impl Instruction for ForkInstruction {
    fn execute(&self, process: &mut Process, core: &mut CoreWar) -> Result<(), VMError> {
        let new_process = Process {
            id: core.next_process_id(),
            player_id: process.player_id,
            pc: (process.pc + self.get_jump_address()) % MEM_SIZE,
            registers: process.registers.clone(),
            carry_flag: process.carry_flag,
            cycles_to_wait: 0,
            last_live_cycle: process.last_live_cycle,
            alive: true,
        };
        
        core.processes.push(new_process);
        core.champions[process.player_id].process_count += 1;
        
        Ok(())
    }
    
    fn cycles(&self) -> u32 { 800 }
}
```

### Assembler Implementation

```rust
// Token types for lexical analysis
#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    Label(String),                      // label:
    Instruction(String),                // opcode
    Register(u8),                       // r1-r16
    Direct(i32),                        // %123
    Indirect(i32),                      // 123
    DirectLabel(String),                // %:label
    IndirectLabel(String),              // :label
    Name(String),                       // .name "..."
    Comment(String),                    // .comment "..."
    LineComment(String),                // # comment
    Separator,                          // ,
    Newline,
    EOF,
}

// Assembler state
pub struct Assembler {
    tokens: Vec<Token>,                 // Tokenized input
    current: usize,                     // Current token index
    labels: HashMap<String, usize>,     // Label -> address mapping
    instructions: Vec<CompiledInstruction>, // Compiled instructions
    current_address: usize,             // Current assembly address
    errors: Vec<AssemblerError>,        // Compilation errors
}

// Compiled instruction
pub struct CompiledInstruction {
    opcode: u8,                         // Instruction opcode
    param_types: u8,                    // Parameter type encoding
    params: Vec<u8>,                    // Compiled parameters
    size: usize,                        // Total instruction size
    source_line: usize,                 // Source line number
}

// Assembly process
impl Assembler {
    pub fn assemble(&mut self, source: &str) -> Result<Vec<u8>, Vec<AssemblerError>> {
        self.tokenize(source)?;
        self.first_pass()?;             // Collect labels
        self.second_pass()?;            // Generate code
        self.resolve_labels()?;         // Fix label references
        
        if self.errors.is_empty() {
            Ok(self.generate_bytecode())
        } else {
            Err(self.errors.clone())
        }
    }
    
    fn encode_parameters(&self, params: &[Parameter]) -> (u8, Vec<u8>) {
        let mut type_byte = 0u8;
        let mut param_bytes = Vec::new();
        
        for (i, param) in params.iter().enumerate() {
            let param_type = match param {
                Parameter::Register(r) => {
                    param_bytes.push(*r);
                    0b01
                }
                Parameter::Direct(val) => {
                    param_bytes.extend_from_slice(&val.to_le_bytes()[..2]);
                    0b10
                }
                Parameter::Indirect(val) => {
                    param_bytes.extend_from_slice(&val.to_le_bytes());
                    0b11
                }
            };
            
            type_byte |= param_type << (6 - i * 2);
        }
        
        (type_byte, param_bytes)
    }
}
```

### Terminal Visualization

```rust
// Terminal UI state
pub struct TerminalUI {
    memory_view: MemoryView,            // Memory visualization
    dashboard: Dashboard,               // Game state dashboard
    input_handler: InputHandler,        // Keyboard/mouse input
    animator: Animator,                 // Animation system
    paused: bool,                       // Pause state
    speed: f32,                         // Execution speed multiplier
    selected_process: Option<usize>,    // Selected process for inspection
}

// Memory visualization
pub struct MemoryView {
    grid_width: usize,                  // Memory grid width
    grid_height: usize,                 // Memory grid height
    cell_colors: Vec<Color>,            // Color for each memory cell
    process_markers: Vec<ProcessMarker>, // Process position markers
    recent_writes: Vec<WriteAnimation>, // Recent memory write animations
}

// Process marker for visualization
pub struct ProcessMarker {
    process_id: usize,                  // Process ID
    position: usize,                    // Memory position
    symbol: char,                       // Display symbol
    color: Color,                       // Display color
    trail: Vec<usize>,                  // Recent positions for trail effect
}

// Animation system
pub struct Animator {
    animations: Vec<Box<dyn Animation>>, // Active animations
    frame_count: u64,                   // Animation frame counter
}

pub trait Animation {
    fn update(&mut self, dt: f32) -> bool; // Returns true if animation continues
    fn render(&self, renderer: &mut Renderer);
}

// Memory write animation
pub struct WriteAnimation {
    address: usize,                     // Memory address
    value: u8,                          // Written value
    start_time: std::time::Instant,     // Animation start time
    duration: std::time::Duration,      // Animation duration
    color: Color,                       // Animation color
}

impl Animation for WriteAnimation {
    fn update(&mut self, dt: f32) -> bool {
        self.start_time.elapsed() < self.duration
    }
    
    fn render(&self, renderer: &mut Renderer) {
        let progress = self.start_time.elapsed().as_secs_f32() / self.duration.as_secs_f32();
        let alpha = 1.0 - progress;
        
        renderer.render_cell_highlight(self.address, self.color, alpha);
    }
}
```

### Game Loop Implementation

```rust
// Main game loop
impl CoreWar {
    pub fn run(&mut self) -> Result<usize, VMError> {
        while !self.is_game_over() {
            self.execute_cycle()?;
            self.check_death_conditions()?;
            self.update_display()?;
            
            if self.should_dump_memory() {
                self.dump_memory();
            }
        }
        
        Ok(self.winner.unwrap_or(0))
    }
    
    fn execute_cycle(&mut self) -> Result<(), VMError> {
        self.cycle_count += 1;
        
        // Execute processes that are ready
        let mut i = 0;
        while i < self.processes.len() {
            if self.processes[i].cycles_to_wait == 0 {
                self.execute_process(i)?;
                i += 1;
            } else {
                self.processes[i].cycles_to_wait -= 1;
                i += 1;
            }
        }
        
        // Remove dead processes
        self.processes.retain(|p| p.alive);
        
        Ok(())
    }
    
    fn execute_process(&mut self, process_index: usize) -> Result<(), VMError> {
        let process = &self.processes[process_index];
        let instruction = self.decode_instruction(process.pc)?;
        
        // Execute instruction
        match instruction.opcode {
            0x01 => self.execute_live(process_index, &instruction),
            0x02 => self.execute_load(process_index, &instruction),
            0x03 => self.execute_store(process_index, &instruction),
            0x04 => self.execute_add(process_index, &instruction),
            0x05 => self.execute_sub(process_index, &instruction),
            0x06 => self.execute_and(process_index, &instruction),
            0x07 => self.execute_or(process_index, &instruction),
            0x08 => self.execute_xor(process_index, &instruction),
            0x09 => self.execute_zjmp(process_index, &instruction),
            0x0A => self.execute_ldi(process_index, &instruction),
            0x0B => self.execute_sti(process_index, &instruction),
            0x0C => self.execute_fork(process_index, &instruction),
            0x0D => self.execute_lld(process_index, &instruction),
            0x0E => self.execute_lldi(process_index, &instruction),
            0x0F => self.execute_lfork(process_index, &instruction),
            0x10 => self.execute_aff(process_index, &instruction),
            _ => return Err(VMError::InvalidInstruction(instruction.opcode)),
        }?;
        
        // Update process state
        self.processes[process_index].cycles_to_wait = instruction.cycles;
        self.processes[process_index].pc = 
            (self.processes[process_index].pc + instruction.size()) % MEM_SIZE;
        
        Ok(())
    }
    
    fn check_death_conditions(&mut self) -> Result<(), VMError> {
        // Check if cycle_to_die cycles have passed
        if self.cycle_count % self.cycle_to_die == 0 {
            self.perform_death_check()?;
        }
        
        // Check if enough lives have been declared
        if self.lives_in_period >= NBR_LIVE {
            self.cycle_to_die = self.cycle_to_die.saturating_sub(CYCLE_DELTA);
            self.lives_in_period = 0;
        }
        
        Ok(())
    }
    
    fn perform_death_check(&mut self) -> Result<(), VMError> {
        let current_cycle = self.cycle_count;
        
        // Kill processes that haven't called live recently
        self.processes.retain(|p| {
            current_cycle - p.last_live_cycle < self.cycle_to_die
        });
        
        // Update champion process counts
        for champion in &mut self.champions {
            champion.process_count = self.processes.iter()
                .filter(|p| p.player_id == champion.id)
                .count();
        }
        
        // Check for winner
        let alive_champions: Vec<_> = self.champions.iter()
            .filter(|c| c.process_count > 0)
            .collect();
            
        if alive_champions.len() == 1 {
            self.winner = Some(alive_champions[0].id);
        } else if alive_champions.is_empty() {
            self.winner = Some(0); // Draw
        }
        
        Ok(())
    }
}
```

### Error Handling

```rust
// Core War error types
#[derive(Debug, Clone)]
pub enum VMError {
    InvalidInstruction(u8),
    InvalidRegister(u8),
    InvalidMemoryAccess(usize),
    InvalidParameter(String),
    ProcessNotFound(usize),
    ChampionNotFound(usize),
    AssemblerError(AssemblerError),
    IOError(String),
}

#[derive(Debug, Clone)]
pub enum AssemblerError {
    SyntaxError { line: usize, message: String },
    UnknownInstruction { line: usize, instruction: String },
    InvalidParameter { line: usize, parameter: String },
    UndefinedLabel { line: usize, label: String },
    DuplicateLabel { line: usize, label: String },
    InvalidDirective { line: usize, directive: String },
    FileTooLarge { size: usize, max_size: usize },
    MissingName,
    MissingComment,
}

impl std::fmt::Display for VMError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            VMError::InvalidInstruction(opcode) => 
                write!(f, "Invalid instruction: 0x{:02x}", opcode),
            VMError::InvalidRegister(reg) => 
                write!(f, "Invalid register: r{}", reg),
            VMError::InvalidMemoryAccess(addr) => 
                write!(f, "Invalid memory access: 0x{:04x}", addr),
            VMError::InvalidParameter(param) => 
                write!(f, "Invalid parameter: {}", param),
            VMError::ProcessNotFound(id) => 
                write!(f, "Process not found: {}", id),
            VMError::ChampionNotFound(id) => 
                write!(f, "Champion not found: {}", id),
            VMError::AssemblerError(err) => 
                write!(f, "Assembler error: {:?}", err),
            VMError::IOError(msg) => 
                write!(f, "IO error: {}", msg),
        }
    }
}

impl std::error::Error for VMError {}
```

### Performance Optimizations

```rust
// Memory pool for process management
pub struct ProcessPool {
    processes: Vec<Process>,            // Pre-allocated processes
    free_list: Vec<usize>,              // Free process indices
    active_count: usize,                // Number of active processes
}

impl ProcessPool {
    pub fn new(capacity: usize) -> Self {
        let mut processes = Vec::with_capacity(capacity);
        let mut free_list = Vec::with_capacity(capacity);
        
        for i in 0..capacity {
            processes.push(Process::default());
            free_list.push(i);
        }
        
        ProcessPool {
            processes,
            free_list,
            active_count: 0,
        }
    }
    
    pub fn allocate(&mut self) -> Option<usize> {
        if let Some(index) = self.free_list.pop() {
            self.active_count += 1;
            Some(index)
        } else {
            None
        }
    }
    
    pub fn deallocate(&mut self, index: usize) {
        self.processes[index].reset();
        self.free_list.push(index);
        self.active_count -= 1;
    }
}

// Instruction cache for performance
pub struct InstructionCache {
    cache: HashMap<usize, CachedInstruction>,
    max_size: usize,
}

#[derive(Clone)]
pub struct CachedInstruction {
    instruction: Instruction,
    last_accessed: std::time::Instant,
}

impl InstructionCache {
    pub fn get(&mut self, address: usize, memory: &[u8]) -> &Instruction {
        if let Some(cached) = self.cache.get_mut(&address) {
            cached.last_accessed = std::time::Instant::now();
            return &cached.instruction;
        }
        
        // Decode instruction and cache it
        let instruction = self.decode_instruction(address, memory);
        self.cache.insert(address, CachedInstruction {
            instruction,
            last_accessed: std::time::Instant::now(),
        });
        
        // Evict old entries if cache is full
        if self.cache.len() > self.max_size {
            self.evict_oldest();
        }
        
        &self.cache[&address].instruction
    }
    
    fn evict_oldest(&mut self) {
        let oldest_key = self.cache.iter()
            .min_by_key(|(_, cached)| cached.last_accessed)
            .map(|(key, _)| *key)
            .unwrap();
            
        self.cache.remove(&oldest_key);
    }
}
```

## Testing Framework

```rust
// Test utilities
pub struct TestVM {
    core: CoreWar,
    expected_cycles: u32,
    expected_memory: Vec<(usize, u8)>,
    expected_registers: Vec<(usize, usize, i32)>, // (process, register, value)
}

impl TestVM {
    pub fn new() -> Self {
        TestVM {
            core: CoreWar::new(),
            expected_cycles: 0,
            expected_memory: Vec::new(),
            expected_registers: Vec::new(),
        }
    }
    
    pub fn load_champion(&mut self, bytecode: &[u8]) -> &mut Self {
        let champion = Champion::from_bytecode(bytecode).unwrap();
        self.core.add_champion(champion);
        self
    }
    
    pub fn expect_memory(&mut self, address: usize, value: u8) -> &mut Self {
        self.expected_memory.push((address, value));
        self
    }
    
    pub fn expect_register(&mut self, process: usize, register: usize, value: i32) -> &mut Self {
        self.expected_registers.push((process, register, value));
        self
    }
    
    pub fn run_cycles(&mut self, cycles: u32) -> Result<(), VMError> {
        for _ in 0..cycles {
            self.core.execute_cycle()?;
        }
        Ok(())
    }
    
    pub fn assert_expectations(&self) {
        // Check memory expectations
        for (address, expected_value) in &self.expected_memory {
            let actual_value = self.core.read_memory(*address);
            assert_eq!(actual_value, *expected_value, 
                      "Memory mismatch at address 0x{:04x}", address);
        }
        
        // Check register expectations
        for (process_id, register, expected_value) in &self.expected_registers {
            let actual_value = self.core.processes[*process_id].registers[*register];
            assert_eq!(actual_value, *expected_value,
                      "Register mismatch for process {} register r{}", 
                      process_id, register + 1);
        }
    }
}

// Example test
#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_live_instruction() {
        let mut vm = TestVM::new();
        
        // Load a simple champion that just calls live
        let bytecode = assemble_simple_live_program();
        vm.load_champion(&bytecode)
          .run_cycles(10)
          .expect("Failed to run cycles");
          
        // Check that the process is still alive
        assert!(vm.core.processes[0].alive);
        assert_eq!(vm.core.lives_in_period, 1);
    }
    
    #[test]
    fn test_fork_instruction() {
        let mut vm = TestVM::new();
        
        let bytecode = assemble_fork_program();
        vm.load_champion(&bytecode)
          .run_cycles(800)  // Fork takes 800 cycles
          .expect("Failed to run cycles");
          
        // Check that a new process was created
        assert_eq!(vm.core.processes.len(), 2);
        assert_eq!(vm.core.processes[0].player_id, vm.core.processes[1].player_id);
    }
}
```

This technical guide provides the detailed implementation patterns, data structures, and algorithms needed to build a robust Core War implementation in Rust. The design emphasizes memory safety, performance, and maintainability while providing comprehensive error handling and testing capabilities.