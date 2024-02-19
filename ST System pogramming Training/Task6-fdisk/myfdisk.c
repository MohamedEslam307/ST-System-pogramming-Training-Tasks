#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#define EXTENDED_PARTITION 5
typedef struct {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
} PartitionEntry;
char buf[512];

void print_table_entry_data(PartitionEntry * cp_table_entry_ptr, int i,
			    char **cp_argv);

void print_extended_entry_data(PartitionEntry * cp_table_entry_ptr,
			       char **cp_argv, int cp_fd,
			       uint64_t offset_val);

int main(int argc, char **argv)
{
    char extended_part_flag = 0;
    uint64_t offset_val = 0;
    int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
	printf("ERROR: Can't open %s\n", argv[1]);
    } else {
	ssize_t ret_read_size = read(fd, buf, 512);
	if (-1 == ret_read_size) {
	    printf("ERROR: Can't read %s\n", argv[1]);
	} else {
	    PartitionEntry *table_entry_ptr =
		(PartitionEntry *) & buf[446];
	    printf("%-5s     %-6s %-10s %-10s %-10s %-10s %-10s %-10s\n",
		   "Device", "Boot", "Start", "End", "Sectors", "Size(GB)",
		   "Id", "Type");
	    for (int i = 0; i < 4; i++) {
		if (table_entry_ptr[i].sector_count != 0) {
		    print_table_entry_data(table_entry_ptr, i, argv);
		    if (EXTENDED_PARTITION ==
			table_entry_ptr[i].partition_type) {
			extended_part_flag = i;
		    }
		} else {
		    /*Nothing */
		}
	    }
	    if (extended_part_flag) {
		offset_val =
		    (uint64_t) table_entry_ptr[extended_part_flag].lba *
		    512;
		print_extended_entry_data(table_entry_ptr, argv, fd,
					  offset_val);
	    }
	}
    }
    return 0;
}

void print_table_entry_data(PartitionEntry * cp_table_entry_ptr, int i,
			    char **cp_argv)
{
    printf("%s%-5d %-4c %-10u %-10u %-10u %-10u %-10X\n",
	   cp_argv[1],
	   i + 1,
	   cp_table_entry_ptr[i].status == 0x80 ? '*' : ' ',
	   cp_table_entry_ptr[i].lba,
	   cp_table_entry_ptr[i].lba +
	   cp_table_entry_ptr[i].sector_count - 1,
	   cp_table_entry_ptr[i].sector_count,
	   (uint32_t) (((uint64_t) cp_table_entry_ptr[i].sector_count
			* 512) / (1024 * 1024 * 1024)),
	   cp_table_entry_ptr[i].partition_type);

}

void print_extended_entry_data(PartitionEntry * cp_table_entry_ptr,
			       char **cp_argv, int cp_fd,
			       uint64_t offset_val)
{
    static char counter = 5;
    off_t ret_off = lseek(cp_fd,
			  offset_val,
			  SEEK_SET);
    if (-1 == ret_off) {
	printf("ERROR: Can't reposition read/write file offset\n");
    } else {

	uint64_t l_offset_val = offset_val;

	while (1) {
	    ssize_t ret_read_size = read(cp_fd, buf, 512);
	    if (-1 == ret_read_size) {
		printf("ERROR: Can't read from repositioned file\n");
		break;
	    } else {

		printf("%s%-5d %-4c %-10ld %-10ld %-10u %-10u %-10X\n",
		       cp_argv[1],
		       counter,
		       cp_table_entry_ptr[0].status == 0x80 ? '*' : ' ',
		       cp_table_entry_ptr[0].lba + l_offset_val / 512,
		       (cp_table_entry_ptr[0].lba + ((uint64_t)
						     cp_table_entry_ptr
						     [0].sector_count - 1 +
						     l_offset_val / 512)),
		       cp_table_entry_ptr[0].sector_count,
		       (uint32_t) (((uint64_t)
				    cp_table_entry_ptr[0].sector_count *
				    512) / (1024 * 1024 * 1024)),
		       cp_table_entry_ptr[0].partition_type);


		l_offset_val = offset_val + cp_table_entry_ptr[1].lba * 512;	//change the offset value to the 
		//frist EBR offset + the value of the next one

		if (cp_table_entry_ptr[1].sector_count == 0) {	//if it's the last EBR break from the loop
		    break;
		}
		counter++;	//incement the partition index
		off_t ret_off = lseek(cp_fd, l_offset_val, SEEK_SET);	//change the offset to the next EBRi
		if (-1 == ret_off) {
		    printf("ERROR: Can't reposition read/write file offset\n");
		} else {	/*Nothing */
		}
	    }
	}
    }
}
