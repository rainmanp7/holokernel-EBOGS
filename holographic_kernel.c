// holographic_kernel.c
#include <stdint.h>

// Enhanced Holographic Memory Configuration
#define HOLOGRAPHIC_DIMENSIONS 512
#define HOLOGRAPHIC_MEMORY_BASE 0xA0000
#define HOLOGRAPHIC_MEMORY_SIZE 0x10000
#define MAX_MEMORY_ENTRIES 128
#define MAX_ENTITIES 32
#define INITIAL_ENTITIES 3
#define MAX_ENTITY_DOMAINS 8

// Video Memory
#define VIDEO_MEMORY 0xb8000

// --- Modified Structures ---
typedef struct {
    uint32_t task_id;
    uint32_t data[4];
    uint8_t valid;
} Task;

typedef struct {
    float data[HOLOGRAPHIC_DIMENSIONS];
    uint32_t hash_signature;
    uint16_t active_dimensions;
    uint8_t valid;
} HolographicVector;

typedef struct {
    HolographicVector input_pattern;
    HolographicVector output_pattern;
    uint32_t timestamp;
    uint8_t valid;
} MemoryEntry;

// --- EMERGENCE: Enhanced Entity Structure for True Emergence ---
// Adds task vectors, fitness, mutation flags, and GC markers
struct Entity {
    uint32_t id;
    HolographicVector state;
    HolographicVector* genome;
    uint32_t age;
    uint32_t interaction_count;
    uint8_t is_active;

    float specialization_scores[MAX_ENTITY_DOMAINS];
    float resource_allocation;
    float confidence;
    char domain_name[32];

    // --- EMERGENCE: Task & Path Assignment ---
    HolographicVector task_vector;  // Assigned task encoded as vector
    uint32_t path_id;               // Logical path ID (e.g., 0xA1 = network path)
    float task_alignment;           // Cosine similarity between state and task

    // --- EMERGENCE: Evolution & Fitness ---
    uint32_t fitness_score;         // Accumulated performance metric
    uint32_t spawn_count;           // Number of children spawned
    uint8_t marked_for_gc;          // Garbage collection flag
    uint8_t is_mutant;              // Mutation flag for debugging
};

struct HardwareInfo {
    char cpu_vendor[13];
    uint32_t cpu_features;
    uint32_t memory_kb;
    int device_count;
};

struct HolographicSystem {
    MemoryEntry memory_pool[MAX_MEMORY_ENTRIES];
    uint32_t memory_count;
    uint32_t global_timestamp;
} holo_system;

struct Entity entity_pool[MAX_ENTITIES];
uint32_t active_entity_count = 0;

// --- EMERGENCE: Fast Inverse Square Root for Vector Math ---
// Needed for cosine similarity in task alignment
float sqrtf(float x) {
    if (x <= 0.0f) return 0.0f;
    float x_half = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - x_half * x * x);
    return 1.0f / x;
}

uint32_t check_protected_mode() {
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    return cr0 & 0x1;
}

//---Function Prototypes---
void serial_init();
void serial_write(char c);
void serial_print(const char* str);
void print_char(char c, uint8_t color);
void print(const char* str);
void print_hex(uint32_t value);
void kmain();
uint32_t hash_data(const void* input, uint32_t size);
HolographicVector create_holographic_vector(const void* input, uint32_t size);
void encode_holographic_memory(HolographicVector* input, HolographicVector* output);
HolographicVector* retrieve_holographic_memory(uint32_t hash);
void initialize_holographic_memory();
void load_initial_genome_vocabulary();
void initialize_emergent_entities();
struct Entity* spawn_entity();
void update_entities();
void render_entities_to_vga();
void probe_hardware();
void set_memory_value(uint32_t address, uint8_t value);
uint8_t get_memory_value(uint32_t address);

//---Kernel starting point---
void kmain() {
    volatile char* video = (volatile char*)0xb8000;
    video[0] = 'K';
    video[1] = 0x0F;
    video[2] = 'E';
    video[3] = 0x0F;
    video[4] = 'R';
    video[5] = 0x0F;
    video[6] = 'N';
    video[7] = 0x0F;
    video[8] = '2';
    video[9] = 0x0F;

    serial_init();
    serial_print("DEBUG: Serial initialized, kernel reached!\n");
    serial_print("Enhanced Holographic Kernel (Emergent Entities) Starting...\n");
    serial_print("Initializing high-dimensional memory system...\n");
    print("Enhanced Holographic Kernel (Emergent Entities) Starting...\n");
    print("Initializing high-dimensional memory system...\n");

    initialize_holographic_memory();
    load_initial_genome_vocabulary();
    initialize_emergent_entities();

    // --- EMERGENCE: Assign Initial Task Vectors ---
    // Proof-of-concept: assign "network_io_path" to first entities
    HolographicVector path_vector = create_holographic_vector("network_io_path", strlen("network_io_path") + 1);
    for (int i = 0; i < active_entity_count && i < 2; i++) {
        entity_pool[i].task_vector = path_vector;
        entity_pool[i].path_id = 0xA1;
        serial_print("[TASK] Assigned path 0xA1 to entity ");
        print_hex(entity_pool[i].id);
        serial_print("\n");
    }

    print("Holographic Kernel with Emergent Entities Initialized!\n");
    print("System entering emergent entity loop...\n");
    serial_print("[BOOT] Kernel fully initialized. Emergence engine online.\n");

    uint32_t last_update = 0;
    uint32_t update_interval = 500000;

    while (1) {
        if (holo_system.global_timestamp - last_update > update_interval) {
             update_entities();
             render_entities_to_vga();
             last_update = holo_system.global_timestamp;
        }
        holo_system.global_timestamp++;
        __asm__ volatile("hlt");
    }
}

//---Hash function (FNV-1a) ---
uint32_t hash_data(const void* input, uint32_t size) {
    const uint8_t* data = (const uint8_t*)input;
    uint32_t hash = 2166136261U;
    for (uint32_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 16777619U;
    }
    return hash;
}

//---Holographic Memory Functions ---
HolographicVector create_holographic_vector(const void* input, uint32_t size) {
    HolographicVector vector = {0};
    vector.hash_signature = hash_data(input, size);
    vector.valid = 1;
    vector.active_dimensions = 0;
    uint32_t seed = vector.hash_signature;
    for (int i = 0; i < HOLOGRAPHIC_DIMENSIONS; i++) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        if ((seed % 10) == 0) {
            vector.data[i] = ((float)((seed % 2000) - 1000)) / 1000.0f;
            vector.active_dimensions++;
        } else {
            vector.data[i] = 0.0f;
        }
    }
    return vector;
}

void encode_holographic_memory(HolographicVector* input, HolographicVector* output) {
    if (holo_system.memory_count >= MAX_MEMORY_ENTRIES) {
        for (int i = 0; i < MAX_MEMORY_ENTRIES - 1; i++) {
            holo_system.memory_pool[i] = holo_system.memory_pool[i + 1];
        }
        holo_system.memory_count = MAX_MEMORY_ENTRIES - 1;
        serial_print("Warning: Holographic memory full, evicted oldest entry.\n");
    }
    MemoryEntry* entry = &holo_system.memory_pool[holo_system.memory_count];
    entry->input_pattern = *input;
    entry->output_pattern = *output;
    entry->timestamp = holo_system.global_timestamp++;
    entry->valid = 1;
    holo_system.memory_count++;
}

HolographicVector* retrieve_holographic_memory(uint32_t hash) {
    for (int i = holo_system.memory_count - 1; i >= 0; i--) {
        if (holo_system.memory_pool[i].valid &&
            holo_system.memory_pool[i].input_pattern.hash_signature == hash) {
            return &holo_system.memory_pool[i].output_pattern;
        }
    }
    return 0;
}

void initialize_holographic_memory() {
    print("Setting up holographic memory pool...\n");
    holo_system.memory_count = 0;
    holo_system.global_timestamp = 0;
    for (int i = 0; i < MAX_MEMORY_ENTRIES; i++) {
        holo_system.memory_pool[i].valid = 0;
    }
    print("Holographic memory system online - ");
    print_hex(HOLOGRAPHIC_DIMENSIONS);
    print(" dimensions available\n");
}

void load_initial_genome_vocabulary() {
    const char* vocab[] = {
        "ACTION_PRODUCE", "ACTION_CONSUME", "ACTION_SHARE",
        "ACTION_ACTIVATE", "ACTION_DEACTIVATE",
        "TRAIT_GENERIC", "TRAIT_ACTIVE", "TRAIT_DORMANT",
        "SENSOR_NEIGHBOR_ACTIVE", "SENSOR_MEMORY_MATCH",
        "GENOME_SIMPLE_RULE_1"
    };
    int num_vocab = sizeof(vocab) / sizeof(vocab[0]);
    serial_print("Loading initial genome vocabulary...\n");
    for (int i = 0; i < num_vocab; i++) {
        HolographicVector pattern = create_holographic_vector(vocab[i], strlen(vocab[i]) + 1);
        encode_holographic_memory(&pattern, &pattern);
        serial_print("  Loaded: ");
        serial_print(vocab[i]);
        serial_print("\n");
    }
    serial_print("Initial genome vocabulary loaded.\n");
}

void initialize_emergent_entities() {
    serial_print("Initializing emergent entity pool...\n");
    HolographicVector simple_genome_rule = create_holographic_vector("GENOME_SIMPLE_RULE_1", strlen("GENOME_SIMPLE_RULE_1") + 1);
    HolographicVector* genome_ptr = retrieve_holographic_memory(simple_genome_rule.hash_signature);
    if (!genome_ptr) {
         serial_print("Error: Initial genome rule not found in memory!\n");
         encode_holographic_memory(&simple_genome_rule, &simple_genome_rule);
         genome_ptr = &simple_genome_rule;
    }
    HolographicVector trait_dormant = create_holographic_vector("TRAIT_DORMANT", strlen("TRAIT_DORMANT") + 1);

    for (int i = 0; i < INITIAL_ENTITIES; i++) {
        if (active_entity_count >= MAX_ENTITIES) {
            serial_print("Error: Cannot initialize more entities, pool full.\n");
            break;
        }
        struct Entity* entity = &entity_pool[active_entity_count];
        entity->id = active_entity_count;
        entity->age = 0;
        entity->interaction_count = 0;
        entity->is_active = 1;
        entity->state = trait_dormant;
        entity->genome = genome_ptr;
        for (int j = 0; j < MAX_ENTITY_DOMAINS; j++) {
            entity->specialization_scores[j] = 0.1f;
        }
        entity->resource_allocation = 1.0f;
        entity->confidence = 0.5f;
        entity->fitness_score = 0;
        entity->spawn_count = 0;
        entity->marked_for_gc = 0;
        entity->is_mutant = 0;
        entity->task_alignment = 0.0f;
        strncpy(entity->domain_name, "generic", 31);
        entity->domain_name[31] = '\0';
        active_entity_count++;
        serial_print("  Initialized entity ID: ");
        print_hex(entity->id);
        serial_print("\n");
    }
    serial_print("Initialized ");
    print_hex(active_entity_count);
    serial_print(" emergent entities.\n");
}

struct Entity* spawn_entity() {
    if (active_entity_count >= MAX_ENTITIES) {
        serial_print("Cannot spawn: Entity pool full.\n");
        return NULL;
    }

    struct Entity* new_entity = &entity_pool[active_entity_count];
    new_entity->id = active_entity_count;
    new_entity->age = 0;
    new_entity->interaction_count = 0;
    new_entity->is_active = 1;
    new_entity->fitness_score = 0;
    new_entity->spawn_count = 0;
    new_entity->marked_for_gc = 0;
    new_entity->is_mutant = 0;

    HolographicVector simple_genome_rule = create_holographic_vector("GENOME_SIMPLE_RULE_1", strlen("GENOME_SIMPLE_RULE_1") + 1);
    HolographicVector* genome_ptr = retrieve_holographic_memory(simple_genome_rule.hash_signature);
    if (!genome_ptr) {
         encode_holographic_memory(&simple_genome_rule, &simple_genome_rule);
         genome_ptr = &simple_genome_rule;
    }

    new_entity->state = create_holographic_vector("TRAIT_DORMANT", strlen("TRAIT_DORMANT") + 1);
    new_entity->genome = genome_ptr;

    for (int i = 0; i < MAX_ENTITY_DOMAINS; i++) {
        new_entity->specialization_scores[i] = 0.1f;
    }
    new_entity->resource_allocation = 1.0f;
    new_entity->confidence = 0.5f;
    new_entity->task_alignment = 0.0f;

    strncpy(new_entity->domain_name, "emergent", 31);
    new_entity->domain_name[31] = '\0';

    active_entity_count++;

    serial_print("[SPAWN] SUCCESS: New entity ID ");
    print_hex(new_entity->id);
    serial_print(" initialized.\n");

    return new_entity;
}

// --- EMERGENCE: Core Update Loop with CA Rules, Task Alignment, Mutation, GC ---
void update_entities() {
    uint8_t next_active[MAX_ENTITIES] = {0};
    HolographicVector next_state[MAX_ENTITIES];
    char next_domain[MAX_ENTITIES][32];
    HolographicVector next_task_vector[MAX_ENTITIES];
    uint32_t next_path_id[MAX_ENTITIES];
    float next_task_alignment[MAX_ENTITIES];

    uint32_t hash_trait_active = create_holographic_vector("TRAIT_ACTIVE", strlen("TRAIT_ACTIVE") + 1).hash_signature;
    uint32_t hash_trait_dormant = create_holographic_vector("TRAIT_DORMANT", strlen("TRAIT_DORMANT") + 1).hash_signature;
    uint32_t hash_action_spawn = create_holographic_vector("ACTION_SPAWN", strlen("ACTION_SPAWN") + 1).hash_signature;

    serial_print("[GC] Starting entity update cycle...\n");

    for (int i = 0; i < active_entity_count; i++) {
        struct Entity* entity = &entity_pool[i];

        next_active[i] = entity->is_active;
        next_state[i] = entity->state;
        strncpy(next_domain[i], entity->domain_name, 31);
        next_domain[i][31] = '\0';
        next_task_vector[i] = entity->task_vector;
        next_path_id[i] = entity->path_id;
        next_task_alignment[i] = entity->task_alignment;

        entity->age++;

        int neighbor_active = 0;
        int prev_idx = (i == 0) ? (active_entity_count - 1) : (i - 1);
        int next_idx = (i == active_entity_count - 1) ? 0 : (i + 1);
        if (entity_pool[prev_idx].is_active) neighbor_active++;
        if (entity_pool[next_idx].is_active) neighbor_active++;

        // --- EMERGENCE: Cellular Automata Rule 1 - Activate if neighbor active ---
        if (!entity->is_active && neighbor_active > 0) {
            next_active[i] = 1;
            next_state[i] = create_holographic_vector("TRAIT_ACTIVE", strlen("TRAIT_ACTIVE") + 1);
            strncpy(next_domain[i], "reactor", 31);
            next_domain[i][31] = '\0';
            entity->interaction_count++;
            serial_print("[SPAWN] Entity ");
            print_hex(entity->id);
            serial_print(" activated by neighbor.\n");
        }
        // --- EMERGENCE: Cellular Automata Rule 2 - Sleep if no neighbors ---
        else if (entity->is_active && neighbor_active == 0) {
            next_active[i] = 0;
            next_state[i] = create_holographic_vector("TRAIT_DORMANT", strlen("TRAIT_DORMANT") + 1);
            strncpy(next_domain[i], "sleeper", 31);
            next_domain[i][31] = '\0';
            entity->interaction_count++;
            serial_print("[SLEEP] Entity ");
            print_hex(entity->id);
            serial_print(" going dormant (no neighbors).\n");
        }
        // --- EMERGENCE: Cellular Automata Rule 3 - Spawn if 2+ neighbors ---
        else if (entity->is_active && neighbor_active >= 2 && active_entity_count < MAX_ENTITIES - 1) {
            struct Entity* child = spawn_entity();
            if (child) {
                child->genome = entity->genome;
                child->is_mutant = 1;

                // --- EMERGENCE: Simple Mutation - Flip one random dimension ---
                child->state = entity->state;
                if (HOLOGRAPHIC_DIMENSIONS > 0) {
                    int rand_dim = holo_system.global_timestamp % HOLOGRAPHIC_DIMENSIONS;
                    child->state.data[rand_dim] = -child->state.data[rand_dim];
                }

                child->task_vector = entity->task_vector;
                child->path_id = entity->path_id;
                child->task_alignment = entity->task_alignment;

                serial_print("[MUTATE] Spawned mutant child ID: ");
                print_hex(child->id);
                serial_print(" from parent ");
                print_hex(entity->id);
                serial_print("\n");

                entity->spawn_count++;
                entity->fitness_score += 10;
            }
        }

        // --- EMERGENCE: Task Alignment via Cosine Similarity ---
        if (entity->task_vector.valid) {
            float dot = 0.0f;
            float mag1 = 0.0f, mag2 = 0.0f;
            for (int d = 0; d < HOLOGRAPHIC_DIMENSIONS; d++) {
                dot += entity->state.data[d] * entity->task_vector.data[d];
                mag1 += entity->state.data[d] * entity->state.data[d];
                mag2 += entity->task_vector.data[d] * entity->task_vector.data[d];
            }
            mag1 = (mag1 > 0) ? sqrtf(mag1) : 1.0f;
            mag2 = (mag2 > 0) ? sqrtf(mag2) : 1.0f;
            next_task_alignment[i] = (mag1 * mag2 > 0) ? (dot / (mag1 * mag2)) : 0.0f;

            if (next_task_alignment[i] > 0.7f) {
                entity->fitness_score += 5;
                serial_print("[FIT] Entity ");
                print_hex(entity->id);
                serial_print(" alignment high. Fitness +5.\n");
            }
        }

        // --- EMERGENCE: Mark Low-Fitness/Old Entities for GC ---
        if (entity->age > 1000 && entity->fitness_score < 50) {
            entity->marked_for_gc = 1;
            serial_print("[GC] Entity ");
            print_hex(entity->id);
            serial_print(" marked for garbage collection (low fitness).\n");
        }
    }

    // --- EMERGENCE: Apply State Changes ---
    for (int i = 0; i < active_entity_count; i++) {
        entity_pool[i].is_active = next_active[i];
        entity_pool[i].state = next_state[i];
        strncpy(entity_pool[i].domain_name, next_domain[i], 31);
        entity_pool[i].domain_name[31] = '\0';
        entity_pool[i].task_vector = next_task_vector[i];
        entity_pool[i].path_id = next_path_id[i];
        entity_pool[i].task_alignment = next_task_alignment[i];
    }

    // --- EMERGENCE: Garbage Collection Phase ---
    int write_index = 0;
    for (int i = 0; i < active_entity_count; i++) {
        if (!entity_pool[i].marked_for_gc) {
            if (write_index != i) {
                entity_pool[write_index] = entity_pool[i];
            }
            write_index++;
        } else {
            serial_print("[GC] Entity ");
            print_hex(entity_pool[i].id);
            serial_print(" collected.\n");
        }
    }
    active_entity_count = write_index;

    serial_print("[GC] Update cycle completed. Active entities: ");
    print_hex(active_entity_count);
    serial_print("\n");
}

void render_entities_to_vga() {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    int start_line = 5;
    int start_col = 0;
    int max_lines = 15;

    for (int y = 0; y < max_lines && (start_line + y) < 25; y++) {
        for (int x = 0; x < 80; x++) {
            int screen_pos = ((start_line + y) * 80 + x) * 2;
            video[screen_pos] = ' ';
            video[screen_pos + 1] = 0x07;
        }
    }

    for (int i = 0; i < active_entity_count && i < max_lines; i++) {
        struct Entity* entity = &entity_pool[i];
        int screen_y = start_line + i;
        if (screen_y >= 25) break;
        int col = start_col;
        int screen_pos = (screen_y * 80 + col) * 2;

        video[screen_pos] = 'E'; screen_pos += 2;
        video[screen_pos] = ':'; screen_pos += 2;

        char hex_char = (entity->id < 10) ? ('0' + entity->id) : ('A' + entity->id - 10);
        video[screen_pos] = hex_char; screen_pos += 2;
        video[screen_pos] = ' '; screen_pos += 2;

        video[screen_pos] = entity->is_active ? 'A' : 'D'; screen_pos += 2;
        video[screen_pos] = ' '; screen_pos += 2;

        int domain_len = strlen(entity->domain_name);
        for (int j = 0; j < 6 && j < domain_len; j++) {
            video[screen_pos] = entity->domain_name[j]; screen_pos += 2;
        }
        for (int j = strlen(entity->domain_name); j < 6; j++) {
             video[screen_pos] = ' '; screen_pos += 2;
        }
        video[screen_pos] = ' '; screen_pos += 2;

        video[screen_pos] = 'I'; screen_pos += 2;
        video[screen_pos] = ':'; screen_pos += 2;
        int ic_mod = entity->interaction_count % 100;
        video[screen_pos] = '0' + (ic_mod / 10); screen_pos += 2;
        video[screen_pos] = '0' + (ic_mod % 10); screen_pos += 2;
        video[screen_pos] = ' '; screen_pos += 2;

        int conf_int = (int)(entity->confidence * 10);
        if (conf_int > 9) conf_int = 9;
        video[screen_pos] = 'C'; screen_pos += 2;
        video[screen_pos] = ':'; screen_pos += 2;
        video[screen_pos] = '0' + conf_int; screen_pos += 2;
        video[screen_pos] = ' '; screen_pos += 2;

        // --- EMERGENCE: Render Fitness Score (F:0-9) ---
        int fit_int = (int)(entity->fitness_score / 10) % 10;
        video[screen_pos] = 'F'; screen_pos += 2;
        video[screen_pos] = ':'; screen_pos += 2;
        video[screen_pos] = '0' + fit_int; screen_pos += 2;
        video[screen_pos] = ' '; screen_pos += 2;
    }
}

void probe_hardware() {
    print("Probing hardware (placeholder)...\n");
    holo_system.global_timestamp += 10;
}

void set_memory_value(uint32_t address, uint8_t value) {
    uint8_t *ptr = (uint8_t *)address;
    *ptr = value;
}

uint8_t get_memory_value(uint32_t address) {
    uint8_t *ptr = (uint8_t *)address;
    return *ptr;
}

void print_char(char c, uint8_t color) {
    volatile char* video = (volatile char*)VIDEO_MEMORY;
    static int position = 0;
    if (c == '\n') {
        position = ((position / 80) + 1) * 80;
    } else {
        video[position * 2] = c;
        video[position * 2 + 1] = color;
        position++;
    }
    if (position >= 80 * 25) {
        position = 0;
    }
}

void print(const char* str) {
    while (*str != 0) {
        print_char(*str, 0x0f);
        str++;
    }
}

void print_hex(uint32_t value) {
    char hex_digits[] = "0123456789ABCDEF";
    char buffer[9];
    for (int i = 7; i >= 0; i--) {
        buffer[7-i] = hex_digits[(value >> (i*4)) & 0xF];
    }
    buffer[8] = '\0';
    print("0x");
    print(buffer);
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void serial_init() {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

void serial_write(char c) {
    while (!(inb(0x3F8 + 5) & 0x20));
    outb(0x3F8, c);
}

void serial_print(const char* str) {
    while (*str != 0) {
        serial_write(*str);
        str++;
    }
}

size_t strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

int snprintf(char *str, size_t size, const char *format, ...) {
    if (size == 0) return 0;
    if (size == 1) { str[0] = '\0'; return 0; }
    const char *fmt_ptr = format;
    char *str_ptr = str;
    size_t remaining = size - 1;
    va_list args;
    va_start(args, format);
    int value = va_arg(args, int);
    va_end(args);
    while (*fmt_ptr && remaining > 0) {
        if (*fmt_ptr == '%' && *(fmt_ptr + 1) == 'd') {
            if (value == 0) {
                *str_ptr++ = '0';
                remaining--;
            } else {
                char temp[10];
                int i = 0;
                int val = value;
                while (val > 0 && i < 9) {
                    temp[i++] = '0' + (val % 10);
                    val /= 10;
                }
                for (int j = i - 1; j >= 0 && remaining > 0; j--) {
                    *str_ptr++ = temp[j];
                    remaining--;
                }
            }
            fmt_ptr += 2;
        } else {
            *str_ptr++ = *fmt_ptr++;
            remaining--;
        }
    }
    *str_ptr = '\0';
    return (str_ptr - str);
}

typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)