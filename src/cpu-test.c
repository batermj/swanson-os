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
cpu_test_arithmetic(struct cpu *cpu) {

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x05; /* add */
	code[1] = 0x34;
	cpu->regs[3] = 9;
	cpu->regs[4] = 22;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[3] == 31);
	assert(cpu_get_pc(cpu) == 0x02);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x91; /* dec */
	code[1] = 0x07;
	cpu->regs[1] = 0x09;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[1] == 2);
	assert(cpu_get_pc(cpu) == 0x02);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x31; /* div */
	code[1] = 0x56;
	cpu->regs[5] = 6;
	cpu->regs[6] = 2;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[5] == 3);
	assert(cpu_get_pc(cpu) == 0x02);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x82; /* inc */
	code[1] = 0x24;
	cpu->regs[2] = 2;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[2] == 0x26);
	assert(cpu_get_pc(cpu) == 0x02);
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
cpu_test_branching(struct cpu *cpu) {

	cpu_set_pc(cpu, 0x00);
	code[0] = 0xc0; /* 'beq' - branch if equal */
	code[1] = 0x04; /* address of 0x004 */
	cpu->condition = CPU_CONDITION_NE;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu_get_pc(cpu) == 2);
	/* test again, but with a positive branch */
	cpu_set_pc(cpu, 0x00);
	cpu->condition = CPU_CONDITION_EQ;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu_get_pc(cpu) == 8);
}

static void
cpu_test_jumping(struct cpu *cpu) {

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x25; /* jmp */
	code[1] = 0x30;
	cpu->regs[3] = 0x27;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu_get_pc(cpu) == 0x27);

	cpu_set_pc(cpu, 0x00);
	code[0] = 0x1a;
	code[1] = 0x00;
	code[2] = 0x01;
	code[3] = 0x02;
	code[4] = 0x03;
	code[5] = 0x04;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu_get_pc(cpu) == 0x01020304);
}

static void
cpu_test_misc(struct cpu *cpu) {

	/* test breakpoint */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0x35; /* brk */
	code[1] = 0;    /* (ignored) */
	code[2] = 0x0f; /* nop */
	assert(cpu_step(cpu, 2) == 1);
	assert(cpu->exception == SIGTRAP);
	assert(cpu_get_pc(cpu) == 2);

	cpu_recover(cpu);

	/* test cmp */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0x0e;
	code[1] = 0x12;
	cpu->regs[1] = 0x01;
	cpu->regs[2] = 0x02;
	cpu->condition = CPU_CONDITION_EQ;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->condition == (CPU_CONDITION_LT | CPU_CONDITION_LTU));
	assert(cpu_get_pc(cpu) == 2);

	/* test gsr (get special register) */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0xa1;
	code[1] = 0xff;
	cpu->regs[1] = 0;
	cpu->sregs[0xff] = 42;
	assert(cpu_step(cpu, 1) == 1);
	assert(cpu->regs[1] == 42);
	assert(cpu_get_pc(cpu) == 2);
}

void
cpu_test(void) {

	struct cpu cpu;

	init_fake_cpu(&cpu);

	cpu_test_arithmetic(&cpu);
	cpu_test_bitwise(&cpu);
	cpu_test_branching(&cpu);
	cpu_test_misc(&cpu);
	cpu_test_jumping(&cpu);
}
