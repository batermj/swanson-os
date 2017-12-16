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

#ifndef SWANSON_MODULE_H
#define SWANSON_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

/** A kernel module.
 * Essentially, a driver or
 * series of drivers used for
 * extended the functionality
 * of the kernel.
 * */

struct module {
	/** Implementation data. */
	void *data;
	/** Gets the name of the module. */
	const char *(*get_name)(const void *data);
};

/** Initializes a module.
 * This should be called before
 * any other module function.
 * @param module An uninitialized
 * module structure.
 * */

void
module_init(struct module *module);

/** Gets the name of the module.
 * If a module has no name, then
 * an empty string is returned.
 * @param module An initialized module.
 * @returns The name of the module.
 * This is never @ref NULL.
 * */

const char *
module_get_name(const struct module *module);

/** Called when a module is loaded.
 * This is defined by the module itself.
 * @param module An initialized module.
 * @returns Zero on success, non-zero on
 * failure.
 * */

int
module_load(struct module *module);

/** Called when a module is unloaded
 * from the kernel.
 * @param module An initialized module.
 * */

void
module_unload(struct module *module);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_MODULE_H */
