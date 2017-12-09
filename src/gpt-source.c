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

#include "gpt.h"

#include "crc32.h"
#include "null.h"
#include "sstream.h"

void
gpt_source_init(struct gpt_source *source) {
	source->data = NULL;
	source->read_header = NULL;
	source->read_header_backup = NULL;
	source->write_header = NULL;
	source->write_header_backup = NULL;
	source->read_partition = NULL;
	source->read_partition_backup = NULL;
	source->write_partition = NULL;
	source->write_partition_backup = NULL;
}

enum gpt_error
gpt_source_read_header(struct gpt_source *source,
                       struct gpt_header *header) {
	if (source->read_header == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->read_header(source->data, header);
}

enum gpt_error
gpt_source_write_header(struct gpt_source *source,
                        const struct gpt_header *header) {
	if (source->write_header == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->write_header(source->data, header);
}

enum gpt_error
gpt_source_write_header_backup(struct gpt_source *source,
                               const struct gpt_header *header) {
	if (source->write_header_backup == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->write_header_backup(source->data, header);
}

enum gpt_error
gpt_source_update_header(struct gpt_source *source,
                         const struct gpt_header *header) {

	enum gpt_error error;

	error = gpt_source_write_header(source, header);
	if (error != GPT_ERROR_NONE)
		return error;

	error = gpt_source_write_header_backup(source, header);
	if (error != GPT_ERROR_NONE)
		return error;

	return GPT_ERROR_NONE;
}

enum gpt_error
gpt_source_read_partition(struct gpt_source *source,
                          uint32_t partition_index,
                          struct gpt_partition *partition) {
	if (source->read_partition == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->read_partition(source->data, partition_index, partition);
}

enum gpt_error
gpt_source_write_partition(struct gpt_source *source,
                           uint32_t partition_index,
                           const struct gpt_partition *partition) {
	if (source->write_partition == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->write_partition(source->data, partition_index, partition);
}

enum gpt_error
gpt_source_write_partition_backup(struct gpt_source *source,
                                  uint32_t partition_index,
                                  const struct gpt_partition *partition) {
	if (source->write_partition_backup == NULL)
		return GPT_ERROR_NOT_IMPLEMENTED;
	else
		return source->write_partition_backup(source->data, partition_index, partition);
}

enum gpt_error
gpt_source_update_partition(struct gpt_source *source,
                            uint32_t partition_index,
                            const struct gpt_partition *partition) {

	enum gpt_error error;

	error = gpt_source_write_partition(source, partition_index, partition);
	if (error != GPT_ERROR_NONE)
		return error;

	error = gpt_source_write_partition_backup(source, partition_index, partition);
	if (error != GPT_ERROR_NONE)
		return error;

	return GPT_ERROR_NONE;
}

enum gpt_error
gpt_source_format(struct gpt_source *source) {

	struct gpt_header header;

	gpt_header_init(&header);

	return gpt_source_update_header(source, &header);
}

enum gpt_error
gpt_source_allocate(struct gpt_source *source,
                    uint64_t size,
                    uint64_t *starting_lba_ptr) {

	enum gpt_error error;
	struct gpt_header header;
	struct gpt_partition partition;
	uint64_t size_available;
	uint32_t i;
	uint64_t starting_lba;
	uint64_t lba_count;

	gpt_header_init(&header);

	error = gpt_source_read_header(source, &header);
	if (error != GPT_ERROR_NONE)
		return error;

	if (header.last_usable_lba < header.first_usable_lba)
		return GPT_ERROR_UNKNOWN;

	size_available = 512 * ((header.last_usable_lba - header.first_usable_lba) + 1);

	if (size_available < size)
		return GPT_ERROR_NEED_SPACE;

	starting_lba = header.first_usable_lba;

	for (i = 0; i < header.partition_count; i++) {

		gpt_partition_init(&partition);

		error = gpt_source_read_partition(source, i, &partition);
		if (error != GPT_ERROR_NONE)
			return error;

		if (partition.last_lba >= starting_lba) {
			starting_lba = partition.last_lba + 1;
			lba_count = (header.last_usable_lba - starting_lba) + 1;
			if ((lba_count * 512) < size)
				return GPT_ERROR_NEED_SPACE;
		}
	}

	if (starting_lba_ptr != NULL)
		*starting_lba_ptr = starting_lba;

	return GPT_ERROR_NONE;
}

enum gpt_error
gpt_source_add_partition(struct gpt_source *source,
                         uint64_t partition_size,
                         uint32_t *partition_index_ptr) {

	enum gpt_error error;
	struct gpt_header header;
	struct gpt_partition partition;
	uint32_t partition_index;
	uint64_t starting_lba;
	uint64_t lba_count;

	if (partition_size == 0)
		lba_count = 1;
	else if ((partition_size % 512) == 0)
		lba_count = partition_size / 512;
	else
		lba_count = (partition_size / 512) + 1;

	gpt_header_init(&header);

	error = gpt_source_read_header(source, &header);
	if (error != GPT_ERROR_NONE)
		return error;

	header.partition_count++;

	partition_index = header.partition_count - 1;

	error = gpt_source_allocate(source, partition_size, &starting_lba);
	if (error != GPT_ERROR_NONE)
		return error;

	gpt_partition_init(&partition);

	partition.first_lba = starting_lba;
	partition.last_lba = partition.first_lba + (lba_count - 1);

	error = gpt_source_update_partition(source, partition_index, &partition);
	if (error != GPT_ERROR_NONE)
		return error;

	error = gpt_source_update_header(source, &header);
	if (error != GPT_ERROR_NONE)
		return error;

	if (partition_index_ptr != NULL)
		*partition_index_ptr = partition_index;

	return GPT_ERROR_NONE;
}

enum gpt_error
gpt_source_do_checksums(struct gpt_source *source) {
	(void) source;
	return GPT_ERROR_NOT_IMPLEMENTED;
}
