/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Swanson.
 *
 * Swanson is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swanson is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swanson.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SWANSON_CPU_H
#define SWANSON_CPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Describes a CPU condition
 * code. This codes are carefully
 * chosen to match the encoding of
 * the branching instructions.
 * */

enum cpu_condition {
	/** Equal to. */
	CPU_CONDITION_EQ = 0x00,
	/** Greater than or equal to. */
	CPU_CONDITION_GE = 0x06,
	/** Greater than or equal to (unsigned). */
	CPU_CONDITION_GEU = 0x08,
	/** Greater than. */
	CPU_CONDITION_GT = 0x03,
	/** Greater than (unsigned). */
	CPU_CONDITION_GTU = 0x05,
	/** Less than or equal to. */
	CPU_CONDITION_LE = 0x07,
	/** Less than or equal to (unsigned). */
	CPU_CONDITION_LEU = 0x09,
	/** Less than. */
	CPU_CONDITION_LT = 0x02,
	/** Less than (unsigned). */
	CPU_CONDITION_LTU = 0x04,
	/** Not equal to. */
	CPU_CONDITION_NE = 0x01
};

/** A Moxie CPU simulator.
 * This is used for executing application code.
 * */

struct cpu {
	/** Implementation data. */
	void *data;
	/** Reads a 32-bit word. */
	int (*read32)(void *data, uint32_t addr, uint32_t *value);
	/** Reads a 16-bit word. */
	int (*read16)(void *data, uint32_t addr, uint16_t *value);
	/** Reads an 8-bit word. */
	int (*read8)(void *data, uint32_t addr, uint8_t *value);
	/** Writes a 32-bit word. */
	int (*write32)(void *data, uint32_t addr, uint32_t value);
	/** Writes a 16-bit word. */
	int (*write16)(void *data, uint32_t addr, uint16_t value);
	/** Writes an 8-bit word. */
	int (*write8)(void *data, uint32_t addr, uint8_t value);
	/** General-purpose registers. */
	uint32_t regs[18];
	/** Special-purpose registers. */
	uint32_t sregs[256];
	/** Condition register */
	enum cpu_condition condition;
	/** Exception flag. */
	int exception;
	/** Instruction counter. */
	uintmax_t instruction_count;
};

/** Initializes a CPU.
 * @param cpu An uninitialized CPU structure.
 * */

void
cpu_init(struct cpu *cpu);

/** Get the current program counter.
 * @param cpu An initialized CPU structure.
 * */

uint32_t
cpu_get_pc(const struct cpu *cpu);

/** Reads a 16-bit value from memory.
 * @param cpu An initialized CPU structure.
 * @param addr The address of the 16-bit value to read.
 * @param value An address to the variable that will
 * contain the value read from memory.
 * @returns Zero on success, non-zero on failure.
 * */

int
cpu_read16(struct cpu *cpu,
           uint32_t addr,
           uint16_t *value);

/** Set the program counter to a
 * specified address.
 * @param cpu An initialized CPU structure.
 * @param addr The address to set the program
 * counter to.
 * */

void
cpu_set_pc(struct cpu *cpu,
           uint32_t addr);

/** Processes a specified number of instructions.
 * @param cpu An initialized cpu structure.
 * @param steps The number of steps (or instructions) to take.
 * @returns The number of instructions processed.
 * */

uintmax_t
cpu_step(struct cpu *cpu,
         uintmax_t steps);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_CPU_H */