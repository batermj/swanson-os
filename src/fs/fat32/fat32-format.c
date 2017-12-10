#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diskfile.h"
#include "fat_filelib.h"

static int
create_zero_img(const char *imagepath,
                size_t imagesize) {

	FILE *image;
	unsigned char zerobuffer[512];
	size_t sector_size = 512;
	size_t sector_count;
	size_t write_count;
	size_t i;

	sector_count = (imagesize + sector_size) / sector_size;

	memset(zerobuffer, 0, sizeof(zerobuffer));

	image = fopen(imagepath, "wb");
	if (image == NULL) {
		return EXIT_FAILURE;
	}

	write_count = 0;

	while (write_count < (sector_count * sector_size)) {
		write_count += fwrite(zerobuffer, 1, sizeof(zerobuffer), image);
	}

	fclose(image);

	return EXIT_SUCCESS;
}

int main(int argc, const char **argv)
{
	const char *diskpath;
	enum fat32_error error;
	struct fat32_disk *disk;
	struct fat32_diskfile diskfile;
	size_t disksize;
	size_t sector_size;
	int zero_img;

	if (argc >= 2)
		diskpath = argv[1];
	else
		diskpath = "fat32.img";

	sector_size = 512;

	/* 256MiB */
	disksize = 256 * 1024 * 1024;

	zero_img = 0;

	if (zero_img && (create_zero_img(diskpath, disksize) != EXIT_SUCCESS)) {
		fprintf(stderr, "Failed to zero '%s'.\n", diskpath);
		return EXIT_FAILURE;
	}

	fat32_diskfile_init(&diskfile);

	error = fat32_diskfile_open(&diskfile, diskpath, "r+");
	if (error != FAT32_ERROR_NONE) {
		fprintf(stderr, "Failed to open '%s': %s.\n", diskpath, fat32_strerror(error));
		return EXIT_FAILURE;
	}

	disk = fat32_diskfile_to_disk(&diskfile);

	FL_FILE *file;

	// Initialise File IO Library
	fl_init();

	// Attach media access functions to library
	if (fl_attach_media(disk) != FAT_INIT_OK) {
		printf("Failed to assign disk file to library.\n");
		return EXIT_FAILURE;
	}

	fl_format((disksize + sector_size) / sector_size, "Swanson FAT32");

	fl_shutdown();

	fat32_diskfile_done(&diskfile);
}
