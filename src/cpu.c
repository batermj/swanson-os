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

#include "cpu.h"

#include <signal.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static uintmax_t
cpu_step_once(struct cpu *cpu) {

	int err;
	uint32_t pc;
	uint16_t inst;
	uint32_t a;
	uint32_t b;

	pc = cpu_get_pc(cpu);

	err = cpu_read16(cpu, pc, &inst);
	if (err != 0)
		return 0;

	switch ((inst & 0xff00) >> 8) {
	case 0x26: /* and */
		a = (inst & 0x00f0) >> 4;
		b = (inst & 0x000f) >> 0;
		cpu->regs[a] = cpu->regs[a] & cpu->regs[b];
		break;
	default:
		/* illegal instruction */
		cpu->exception = SIGILL;
		return 0;
	}

	cpu_set_pc(cpu, pc + 2);

	return 1;
}

void
cpu_init(struct cpu *cpu) {
	cpu->data = NULL;
	cpu->read8 = NULL;
	cpu->read16 = NULL;
	cpu->read32 = NULL;
	cpu->write8 = NULL;
	cpu->write16 = NULL;
	cpu->write32 = NULL;
	memset(cpu->regs, 0, sizeof(cpu->regs));
	memset(cpu->sregs, 0, sizeof(cpu->sregs));
	cpu->condition = 0;
	cpu->exception = 0;
	cpu->instruction_count = 0;
}

uint32_t
cpu_get_pc(const struct cpu *cpu) {
	return cpu->regs[16];
}

int
cpu_read16(struct cpu *cpu,
           uint32_t addr,
           uint16_t *value) {
	if (cpu->read16 != NULL)
		return cpu->read16(cpu->data, addr, value);
	else
		return -1;
}

void
cpu_set_pc(struct cpu *cpu,
           uint32_t addr) {
	cpu->regs[16] = addr;
}

uintmax_t
cpu_step(struct cpu *cpu,
         uintmax_t steps) {

	uintmax_t i;

	for (i = 0; i < steps; i++) {
		if (cpu_step_once(cpu) != 1)
			break;
	}

	return i;
}
