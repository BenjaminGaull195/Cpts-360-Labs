#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "partition.h"



int main(int argc, char *argv[], char *env[]) {

	int fd, * mbr_addr, temp; 
	char buf[512];
	struct partition * table[4];


	fd = open("vdisk", O_RDONLY);  // check fd value: -1 means open() failed
	if (fd != -1) {
		read(fd, buf, 512);            // read sector 0 into buf[ ]
		mbr_addr = &buf;

		//read in partition table
		for (temp = 0; temp < 4; ++temp) {
			table[temp] = mbr_addr + (temp * PART_TABLE_OFFSET);
		}

		//print the partition table
		for (temp = 0; temp < 4; ++temp) {
			if (table[temp]->sys_type != 5 && table[temp]->sys_type != '5') {
				printf("Partition %d\n", (temp + 1));	//debug print 

				//print drive;
				printf("Drive: %c", table[temp]->drive);
				//print head;
				printf("Head: %c", table[temp]->head);
				//print sector;
				printf("Sector: %c", table[temp]->sector);
				//print cylinder;
				printf("Cylinder: %c", table[temp]->cylinder);
				//print sys_type;
				printf("Sys_type: %c", table[temp]->sys_type);
				//print end_head;
				printf("End_Head: %c", table[temp]->end_head);
				//print end_sector;
				printf("End_Sector: %c", table[temp]->end_sector);
				//print end_cylinder;
				printf("End_Cylinder: %c", table[temp]->end_cylinder);
				//print start_sector;
				printf("Start_Sector: %u", table[temp]->start_sector);
				//print nr_sectors;
				printf("Nr_Sectors: %u", table[temp]->nr_sectors);
			}
			else {
				printf("Partition %d\n", (temp + 1));	//debug print 

				//print drive;
				printf("Drive: %c", table[temp]->drive);
				//print head;
				printf("Head: %c", table[temp]->head);
				//print sector;
				printf("Sector: %c", table[temp]->sector);
				//print cylinder;
				printf("Cylinder: %c", table[temp]->cylinder);
				//print sys_type;
				printf("Sys_type: %c", table[temp]->sys_type);
				//print end_head;
				printf("End_Head: %c", table[temp]->end_head);
				//print end_sector;
				printf("End_Sector: %c", table[temp]->end_sector);
				//print end_cylinder;
				printf("End_Cylinder: %c", table[temp]->end_cylinder);
				//print start_sector;
				printf("Start_Sector: %u", table[temp]->start_sector);
				//print nr_sectors;
				printf("Nr_Sectors: %u", table[temp]->nr_sectors);
				
				//print EXTEND Partition



			}
		}


		//lseek(fd, (long)123 * 512, SEEK_SET);  // lseek to byte 123*512 OR sector 123
		//read(fd, buf, 512);                  // read sector 123 into buf[ ]







	}
	else {
		printf("Failed to open vdisk");
	}


}

