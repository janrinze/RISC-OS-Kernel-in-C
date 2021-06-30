/* Copyright 2021 Simon Willcocks
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

inline uint32_t __attribute__(( always_inline )) get_core_number()
{
  uint32_t result;
  asm ( "mrc p15, 0, %[result], c0, c0, 5" : [result] "=r" (result) );
  return ((result & 0xc0000000) != 0x80000000) ? 0 : (result & 15);
}

inline uint32_t __attribute__(( always_inline )) number_of_cores()
{
  uint32_t result;
  // L2CTLR, ARM DDI 0500G Cortex-A53, generally usable?
  asm ( "MRC p15, 1, %[result], c9, c0, 2" : [result] "=r" (result) );
  return ((result >> 24) & 3) + 1;
}

typedef struct {
  uint32_t base;
  uint32_t size;
} ram_block;

// Various values that are needed pre-mmu
typedef struct {
  uint32_t relocation_offset;
  uint32_t final_location;
  uint32_t *states;
  bool states_initialised;
  uint32_t core_workspaces;
  uint32_t shared_memory;

  ram_block ram_blocks[4];

  ram_block less_aligned;
  uint32_t core_to_enter_mmu;
  uint32_t core_entered_mmu;
} startup;

startup boot_data; // Read-only once MMU enabled
