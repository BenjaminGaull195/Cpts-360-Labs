#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "partition.h"



int main(int argc, char *argv[], char *env[]) {
	//establish local variables
	int fd, * mbr_addr, temp; 
	char buf[512];
	struct partition * table[4];

	//open vdisk
	fd = open("vdisk", O_RDONLY);  // check fd value: -1 means open() failed
	if (fd != -1) {						//if open does not fail
		read(fd, buf, 512);            // read sector 0 into buf[ ]
		mbr_addr = &buf;				//get address of buf to use to read MBR

		//read in partition table
		for (temp = 0; temp < 4; ++temp) {		//read partitions into struct partition table for easy reading
			table[temp] = mbr_addr + PART_TABLE_OFFSET + (temp * sizeof(struct partition));
		}

		//print the partition table
		for (temp = 0; temp < 4; ++temp) {
			//test if partition type is Extend
			if (table[temp]->sys_type != 5 && table[temp]->sys_type != '5') {
				//myprintf("Partition %d\n", (temp + 1));	//debug print 

				//print drive;
				myprintf("Drive: %c\n", table[temp]->drive);
				//print head;
				myprintf("Head: %c\n", table[temp]->head);
				//print sector;
				myprintf("Sector: %c\n", table[temp]->sector);
				//print cylinder;
				myprintf("Cylinder: %c\n", table[temp]->cylinder);
				//print sys_type;
				myprintf("Sys_type: %c\n", table[temp]->sys_type);
				//print end_head;
				myprintf("End_Head: %c\n", table[temp]->end_head);
				//print end_sector;
				myprintf("End_Sector: %c\n", table[temp]->end_sector);
				//print end_cylinder;
				myprintf("End_Cylinder: %c\n", table[temp]->end_cylinder);
				//print start_sector;
				myprintf("Start_Sector: %u\n", table[temp]->start_sector);
				//print nr_sectors;
				myprintf("Nr_Sectors: %u\n", table[temp]->nr_sectors);

				putchar('\n');

			}
			else {	//type is Extend
				//printf("Partition %d\n", (temp + 1));	//debug print 

				//print drive;
				myprintf("Drive: %c\n", table[temp]->drive);
				//print head;
				myprintf("Head: %c\n", table[temp]->head);
				//print sector;
				myprintf("Sector: %c\n", table[temp]->sector);
				//print cylinder;
				myprintf("Cylinder: %c\n", table[temp]->cylinder);
				//print sys_type;
				myprintf("Sys_type: %c\n", table[temp]->sys_type);
				//print end_head;
				myprintf("End_Head: %c\n", table[temp]->end_head);
				//print end_sector;
				myprintf("End_Sector: %c\n", table[temp]->end_sector);
				//print end_cylinder;
				myprintf("End_Cylinder: %c\n", table[temp]->end_cylinder);
				//print start_sector;
				myprintf("Start_Sector: %u\n", table[temp]->start_sector);
				//print nr_sectors;
				myprintf("Nr_Sectors: %u\n", table[temp]->nr_sectors);
				
				putchar('\n');

				//load extend partition table
				int extend_start = table[temp]->start_sector;	//get location of first local MBR
				struct partition * local_par;
				int temp2 = 0;
				
				while(extend_start) {
					extend_start = mbr_addr + PART_TABLE_OFFSET + (temp2 * sizeof(struct partition));

					lseek(fd, (long)extend_start * 512, SEEK_SET);		//find in local MBR
					read(fd, buf, 512);	

					//print drive;
				myprintf("Drive: %c\n", local_par->drive);
				//print head;
				myprintf("Head: %c\n", local_par->head);
				//print sector;
				myprintf("Sector: %c\n", local_par->sector);
				//print cylinder;
				myprintf("Cylinder: %c\n", local_par->cylinder);
				//print sys_type;
				myprintf("Sys_type: %c\n", local_par->sys_type);
				//print end_head;
				myprintf("End_Head: %c\n", local_par->end_head);
				//print end_sector;
				myprintf("End_Sector: %c\n", local_par->end_sector);
				//print end_cylinder;
				myprintf("End_Cylinder: %c\n", local_par->end_cylinder);
				//print start_sector;
				myprintf("Start_Sector: %u\n", local_par->start_sector);
				//print nr_sectors;
				myprintf("Nr_Sectors: %u\n", local_par->nr_sectors);
				
				putchar('\n');
				++temp2;

				}




				//


			}
		}


		//lseek(fd, (long)123 * 512, SEEK_SET);  // lseek to byte 123*512 OR sector 123
		//read(fd, buf, 512);                  // read sector 123 into buf[ ]







	}
	else {
		printf("Failed to open vdisk");
	}


}

