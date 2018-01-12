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

#ifndef SWANSON_PROCESS_H
#define SWANSON_PROCESS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A section in the address space
 * of a process.
 * */

struct addr_section {
	/** The virtual address */
	uint32_t vaddr;
	/** The physical address */
	void *paddr;
	/** The number of bytes occupied by
	 * the section. */
	uint32_t size;
	/** Memory in this section may
	 * be executed. */
	bool execute_permission;
	/** Memory in this section may
	 * be read from. */
	bool read_permission;
	/** Memory in this section may
	 * be written to. */
	bool write_permission;
};

/** Initializes an address space section.
 * @param section An uninitialized address
 * space section.
 * */

void
addr_section_init(struct addr_section *section);

/** Releases memory allocated by an address
 * space section.
 * @param section An initialized address space section.
 * */

void
addr_section_free(struct addr_section *section);

/** Resizes an address space section.
 * Care should be taken that the new size of the
 * address space section doesn't cause it to overlap
 * with another section.
 * @param section An initialized address space section.
 * @param size The new size of the address space section.
 * @returns Zero on success, non-zero on failure.
 * */

int
addr_section_resize(struct addr_section *section,
                    uint32_t size);

/** The address space of a process.
 * */

struct addr_space {
	/** The section array. Every
	 * section contains part of the
	 * address space. */
	struct addr_section *section_array;
	/** The number of sections in the
	 * address space. */
	uint32_t section_count;
};

/** Initializes an address space.
 * @param space An uninitialized address space.
 * */

void
addr_space_init(struct addr_space *space);

/** Releases all of the memory in the address space.
 * @param space An initialized address space.
 * */

void
addr_space_free(struct addr_space *space);

/** Allocates memory in the address space.
 * @param space An initialized address space.
 * @param vaddr The virtual address to use.
 * @param size The number of bytes to allocate.
 * @returns The section structure, if it is
 * allocated (null if it is not).
 * */

struct addr_section *
addr_space_alloc(struct addr_space *space,
                 uint32_t vaddr,
                 uint32_t size);

/** Releases a section of the address space.
 * @param space An initialized address.
 * @param vaddr The virtual address of the
 * section to be released.
 * */

void
addr_space_release(struct addr_space *space,
                   uint32_t vaddr);

/** Represents a stack of a thread.
 * This stack doesn't grow downwards,
 * like normal stacks do. It grows upwards.
 * For applications, it appears to grow downward.
 * From the native address space, it grows upwards.
 * */

/** A thread running in the application
 * address space.
 * */

struct thread {
	/** The CPU that executes instructions
	 * on behalf of the thread. */
	struct cpu cpu;
};

/** Initializes the thread.
 * @param thread An uninitialized thread.
 * */

void
thread_init(struct thread *thread);

/** Moves the thread's execution by
 * a specified number of steps.
 * @param thread An initialized
 * thread structure.
 * @param steps The number of steps
 * for the CPU to take.
 * @returns The number of steps
 * actually taken.
 * */

uintmax_t
thread_step(struct thread *thread,
            uintmax_t steps);

/** A running process in the application
 * address space.
 * */

struct process {
	/** The address space of the process. */
	struct addr_space addr_space;
	/** The threads that make up the process. */
	struct thread *thread_array;
	/** The number of threads in the process. */
	uintmax_t thread_count;
	/** Processes started by this one. */
	struct process *child_array;
	/** The number of processes that are
	 * children to this process. */
	uintmax_t child_count;
};

/** Initializes the process.
 * @param process An uninitialized process.
 * */

void
process_init(struct process *process);

/** Moves the process' execution by a
 * specified number of steps. All threads
 * in this process and all sub processes are
 * incremented by the same number of steps.
 * @param process An initialized process structure.
 * @param steps The number of steps for the process to take.
 * @returns The number of steps actually taken.
 * */

uintmax_t
process_step(struct process *process,
             uintmax_t steps);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_PROCESS_H */
