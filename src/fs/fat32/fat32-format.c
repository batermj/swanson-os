#include <stdio.h>
#include <stdlib.h>

#include "fat_filelib.h"

struct fat32_diskfile {
	struct fat32_disk disk;
	FILE *file;
};

static int
diskfile_read(void *data,
              uint32_t sector_index,
              void *buf,
              uint32_t sector_count) {

    struct fat32_diskfile *diskfile;

    diskfile = (struct fat32_diskfile *) data;

    fseek(diskfile->file, sector_index * 512, SEEK_SET);

    fread(buf, 1, sector_count * 512, diskfile->file);

    return 1;
}

static int
diskfile_write(void *data,
               uint32_t sector_index,
               const void *buf,
               uint32_t sector_count) {

    struct fat32_diskfile *diskfile;

    diskfile = (struct fat32_diskfile *) data;

    fseek(diskfile->file, sector_index * 512, SEEK_SET);

    fwrite(buf, 1, sector_count * 512, diskfile->file);

    return 1;
}

int main(int argc, const char **argv)
{
    const char *diskpath;
    struct fat32_diskfile diskfile;

    if (argc >= 2)
        diskpath = argv[1];
    else
        diskpath = "fat32.img";

    diskfile.file = fopen(diskpath, "r+");
    if (diskfile.file == NULL) {
        fprintf(stderr, "Failed to open '%s'.\n", diskpath);
        return EXIT_FAILURE;
    }

    diskfile.disk.data = &diskfile;
    diskfile.disk.read = diskfile_read;
    diskfile.disk.write = diskfile_write;

    FL_FILE *file;

    // Initialise File IO Library
    fl_init();

    // Attach media access functions to library
    if (fl_attach_media(&diskfile.disk) != FAT_INIT_OK)
    {
        printf("ERROR: Media attach failed\n");
        return EXIT_FAILURE;
    }

    // List root directory
    fl_listdirectory("/");

    // Create File
    file = fl_fopen("/file.bin", "w");
    if (file)
    {
        // Write some data
        unsigned char data[] = { 1, 2, 3, 4 };
        if (fl_fwrite(data, 1, sizeof(data), file) != sizeof(data))
            printf("ERROR: Write file failed\n");
    }
    else
        printf("ERROR: Create file failed\n");

    // Close file
    fl_fclose(file);

    // Delete File
    if (fl_remove("/file.bin") < 0)
        printf("ERROR: Delete file failed\n");

    // List root directory
    fl_listdirectory("/");

    fl_shutdown();

    fclose(diskfile.file);
}
