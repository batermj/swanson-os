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

#include "cpu-test.h"

#include "cpu.h"

#include <assert.h>
#include <signal.h>
#include <stdint.h>

unsigned char code[256];

static int
read8(void *cpu_ptr, uint32_t addr, uint8_t *value) {

	struct cpu *cpu;

	cpu = (struct cpu *) cpu_ptr;

	if (addr < sizeof(code)) {
		*value = code[addr];
	} else {
		cpu->exception = SIGBUS;
		return -1;
	}

	return 0;
}

static int
read16(void *cpu_ptr, uint32_t addr, uint16_t *value) {

	struct cpu *cpu;

	cpu = (struct cpu *) cpu_ptr;

	if (addr < sizeof(code)) {
		*value = 0;
		*value |= code[addr + 0] << 0x08;
		*value |= code[addr + 1] << 0x00;
	} else {
		cpu->exception = SIGBUS;
		return -1;
	}

	return 0;
}

static int
read32(void *cpu_ptr, uint32_t addr, uint32_t *value) {

	struct cpu *cpu;

	cpu = (struct cpu *) cpu_ptr;

	if (addr < sizeof(code)) {
		*value = 0;
		*value |= code[addr + 0] << 0x18;
		*value |= code[addr + 1] << 0x10;
		*value |= code[addr + 2] << 0x08;
		*value |= code[addr + 3] << 0x00;
	} else {
		cpu->exception = SIGBUS;
		return -1;
	}

	return 0;
}

static void
init_fake_cpu(struct cpu *cpu) {
	cpu_init(cpu);
	cpu->data = cpu;
	cpu->read8 = read8;
	cpu->read16 = read16;
	cpu->read32 = read32;
}

static void
cpu_test_bitwise(struct cpu *cpu) {

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x26; /* 'and' */
	code[1] = 0x12; /* registers 1 and 2 */
	cpu->regs[1] = 6;
	cpu->regs[2] = 4;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[1] == 4);
	assert(cpu_get_pc(cpu) == 0x02);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x28; /* 'ashl' - arithmetic shift left  */
	code[1] = 0x23; /* registers 2 and 3 */
	cpu->regs[2] = 0x02;
	cpu->regs[3] = 0x03;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[2] == 0x10);
	assert(cpu_get_pc(cpu) == 0x02);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x2d; /* 'ashr' - arithmetic shift right  */
	code[1] = 0x27; /* registers 2 and 7 */
	cpu->regs[2] = 0x04;
	cpu->regs[7] = 0x01;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[2] == 0x02);
	assert(cpu_get_pc(cpu) == 0x02);
}

static void
cpu_test_arithmetic(struct cpu *cpu) {

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x05;
	code[1] = 0x34;
	cpu->regs[3] = 9;
	cpu->regs[4] = 22;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[3] == 31);
	assert(cpu_get_pc(cpu) == 0x02);
}

void
cpu_test(void) {

	struct cpu cpu;

	init_fake_cpu(&cpu);

	cpu_test_arithmetic(&cpu);
	cpu_test_bitwise(&cpu);
}
