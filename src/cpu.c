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

#define get_a(inst) ((inst & 0x00f0) >> 4)

#define get_b(inst) (inst & 0x000f)

#define get_v(inst) ((((int16_t)((inst & ((1 << 10) - 1)) << 6)) >> 6) << 1)

static uintmax_t
cpu_step_once(struct cpu *cpu) {

	int err;
	uint32_t pc;
	uint16_t inst;
	uint32_t a;
	uint32_t b;
	int16_t offset;
	uint8_t cc_required;
	uint8_t cc_actual;

	pc = cpu_get_pc(cpu);

	err = cpu_read16(cpu, pc, &inst);
	if (err != 0)
		return 0;

	/* check 6-bit instuctions */

	switch ((inst & 0xfc00) >> 0x0a) {
	case 0x30: /* beq */
	case 0x36: /* bge */
	case 0x38: /* bgeu */
	case 0x33: /* bgt */
	case 0x35: /* bgtu */
	case 0x37: /* ble */
	case 0x39: /* bleu */
	case 0x32: /* blt */
	case 0x34: /* bltu */
	case 0x31: /* bne */
		/* condition code required */
		cc_required = (inst & 0x3c00) >> 0x0a;
		/* actual condition code of the cpu */
		cc_actual = cpu->condition;
		/* get branch offset (if taken) */
		offset = get_v(inst);
		/* check for underflow/overflow */
		if (offset < 0) {
			if (((uint16_t)(offset * -1)) > pc) {
				cpu->exception = SIGILL;
				return 0;
			}
		} else if (pc > (UINT32_MAX - ((uint32_t) offset))) {
			cpu->exception = SIGILL;
			return 0;
		} else if (cc_required == cc_actual) {

			if (offset > 0)
				pc += (uint32_t)(offset);
			else
				pc -= (uint32_t)(offset * -1);

			cpu_set_pc(cpu, pc);
			return 1;

		} else {
			pc += 2;
			cpu_set_pc(cpu, pc);
			return 1;
		}
	default:
		break;
	}

	/* check 8-bit instuctions */

	switch ((inst & 0xff00) >> 8) {
	case 0x26: /* and */
		a = get_a(inst);
		b = get_b(inst);
		cpu->regs[a] = cpu->regs[a] & cpu->regs[b];
		break;
	case 0x05: /* add */
		a = get_a(inst);
		b = get_b(inst);
		cpu->regs[a] = cpu->regs[a] + cpu->regs[b];
		break;
	case 0x28: /* ashl */
		a = get_a(inst);
		b = get_b(inst);
		cpu->regs[a] = cpu->regs[a] << cpu->regs[b];
		break;
	case 0x2d: /* ashr */
		a = get_a(inst);
		b = get_b(inst);
		cpu->regs[a] = cpu->regs[a] >> cpu->regs[b];
		break;
	case 0x25:
		cpu->exception = SIGTRAP;
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
	cpu->condition = CPU_CONDITION_EQ;
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

	for (i = 0; (i < steps) && (cpu->exception == 0); i++) {
		if (cpu_step_once(cpu) != 1)
			break;
	}

	return i;
}
