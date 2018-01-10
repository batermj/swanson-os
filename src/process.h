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

/** Represents a stack of a thread.
 * This stack doesn't grow downwards,
 * like normal stacks do. It grows upwards.
 * For applications, it appears to grow downward.
 * From the native address space, it grows upwards.
 * */

struct stack {
	/** The starting address of the
	 * stack. */
	void *addr;
	/** The number of bytes occupied
	 * by the stack. */
	uintmax_t size;
	/** The current position within
	 * the stack. */
	uintmax_t pos;
};

/** Initializes the stack.
 * @param stack An uninitialized stack.
 * */

void
stack_init(struct stack *stack);

/** Pushes a value to the stack.
 * @param stack An initialized stack.
 * @param value The value to push to
 * the stack. This may be NULL.
 * @param size The number of bytes
 * to push to the stack.
 * @returns Zero on success, non-zero
 * on failure.
 * */

int
stack_push(struct stack *stack,
           const void *value,
           uintmax_t size);

/** Pop a value from the stack.
 * @param stack An initialized stack.
 * @param size The number of bytes to
 * pop from the stack.
 * @returns Zero on success, non-zero
 * on failure.
 * */

int
stack_pop(struct stack *stack,
          uintmax_t size);

/** A thread running in the application
 * address space.
 * */

struct thread {
	/** The thread's stack. */
	struct stack stack;
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
	/** The threads that make up the process. */
	struct thread *thread_array;
	/** The number of threads in the process. */
	uintmax_t thread_count;
	/** The index of the thread that the process
	 * is currently executing. */
	uintmax_t thread_index;
};

/** Initializes the process.
 * @param process An uninitialized process.
 * */

void
process_init(struct process *process);

/** Moves the process' execution by a
 * specified number of steps. The process
 * will use the current thread and then increment
 * the thread index.
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
