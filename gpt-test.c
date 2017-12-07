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

#include "gpt-test.h"

#include "gpt.h"
#include "null.h"

#include "assert.h"

struct gpt_fake_source {
	struct gpt_source source;
	struct gpt_header header;
	struct gpt_header header_backup;
	struct gpt_partition partitions[2];
	struct gpt_partition partition_backups[2];
};

static int
read_header(void *fake_source_ptr,
            struct gpt_header *header) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	*header = fake_source->header;

	return 0;
}

static int
read_header_backup(void *fake_source_ptr,
                   struct gpt_header *header) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	*header = fake_source->header_backup;

	return 0;
}

static int
read_partition(void *fake_source_ptr,
               uint32_t partition_index,
               struct gpt_partition *partition) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	assert(partition_index <= 1);

	*partition = fake_source->partitions[partition_index];

	return 0;
}

static int
read_partition_backup(void *fake_source_ptr,
                      uint32_t partition_index,
                      struct gpt_partition *partition) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	assert(partition_index <= 1);

	*partition = fake_source->partition_backups[partition_index];

	return 0;
}

static int
write_header(void *fake_source_ptr,
             const struct gpt_header *header) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	fake_source->header = *header;

	return 0;
}

static int
write_header_backup(void *fake_source_ptr,
                    const struct gpt_header *header) {

	struct gpt_fake_source *fake_source;

	fake_source = (struct gpt_fake_source *) fake_source_ptr;

	fake_source->header_backup = *header;

	return 0;
}

void
gpt_fake_source_init(struct gpt_fake_source *fake_source) {

	gpt_source_init(&fake_source->source);

	gpt_header_init(&fake_source->header);

	gpt_header_init(&fake_source->header_backup);

	gpt_partition_init(&fake_source->partitions[0]);
	gpt_partition_init(&fake_source->partitions[1]);

	gpt_partition_init(&fake_source->partition_backups[0]);
	gpt_partition_init(&fake_source->partition_backups[1]);

	fake_source->source.data = fake_source;

	fake_source->source.read_header = read_header;
	fake_source->source.read_header_backup = read_header_backup;

	fake_source->source.write_header = write_header;
	fake_source->source.write_header_backup = write_header_backup;

	fake_source->source.read_partition = read_partition;
	fake_source->source.read_partition_backup = read_partition_backup;
}

static struct gpt_source *
gpt_fake_source_to_source(struct gpt_fake_source *fake_source) {
	return &fake_source->source;
}

void
gpt_test_allocate(void) {

	enum gpt_error error;
	struct gpt_source *source;
	struct gpt_fake_source fake_source;
	uint64_t starting_lba;

	gpt_fake_source_init(&fake_source);

	/* two blocks available for partition
	 * data */

	fake_source.header.first_usable_lba = 44;
	fake_source.header.last_usable_lba = 45;

	source = gpt_fake_source_to_source(&fake_source);
	assert(source != NULL);

	/* First call should pass because
	 * there is 1024 bytes total for
	 * partition data. */

	error = gpt_source_allocate(source, 512, &starting_lba);
	assert(error == GPT_ERROR_NONE);
	assert(starting_lba == 44);

	/* Act as if the space were allocated by
	 * a partition. */

	fake_source.header.partition_count++;
	fake_source.header_backup.partition_count++;
	fake_source.partitions[0].first_lba = 44;
	fake_source.partitions[0].last_lba = 44;
	fake_source.partition_backups[0].first_lba = 44;
	fake_source.partition_backups[0].last_lba = 44;

	/* Second call should fail because
	 * there is only 512 bytes left for
	 * partition data. */

	error = gpt_source_allocate(source, 1024, &starting_lba);
	assert(error == GPT_ERROR_NEED_SPACE);

	/* Third call should pass because there
	 * is 512 bytes left for partition data. */

	error = gpt_source_allocate(source, 512, &starting_lba);
	assert(error == GPT_ERROR_NONE);
	assert(starting_lba == 45);

	/* Act as if the space were allocated
	 * by a second partition. */

	fake_source.header.partition_count++;
	fake_source.header_backup.partition_count++;
	fake_source.partitions[1].first_lba = 45;
	fake_source.partitions[1].last_lba = 45;
	fake_source.partition_backups[1].first_lba = 45;
	fake_source.partition_backups[1].last_lba = 45;

	/* Fourth call should fail because there
	 * is no more space leftover for partition
	 * data. */

	error = gpt_source_allocate(source, 512, &starting_lba);
	assert(error == GPT_ERROR_NEED_SPACE);
}

void
gpt_test_add_partition(void) {

	enum gpt_error error;
	struct gpt_fake_source fake_source;
	struct gpt_source *source;

	gpt_fake_source_init(&fake_source);

	source = gpt_fake_source_to_source(&fake_source);
	assert(source != NULL);

	error = gpt_source_format(source);
	assert(error == GPT_ERROR_NONE);
}

void
gpt_test_format(void) {

}

void
gpt_test(void) {
	gpt_test_allocate();
	gpt_test_add_partition();
	gpt_test_format();
}
