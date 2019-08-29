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
			else {	//type is Extend
				//printf("Partition %d\n", (temp + 1));	//debug print 

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
				int extend_start = table[temp]->start_sector;		//get location of first local MBR
				struct partition * local_par;
				int temp2 = 0;

				while (extend_start != 0) {
					lseek(fd, (long)extend_start * 512, SEEK_SET);		//find in local MBR
					read(fd, buf, 512);									//load in local MBR

					//load extend partition table
					local_par = mbr_addr + PART_TABLE_OFFSET + (temp2 * sizeof(struct partition));


					//print partition

					//print drive;
					printf("Drive: %c", local_par->drive);
					//print head;
					printf("Head: %c", local_par->head);
					//print sector;
					printf("Sector: %c", local_par->sector);
					//print cylinder;
					printf("Cylinder: %c", local_par->cylinder);
					//print sys_type;
					printf("Sys_type: %c", local_par->sys_type);
					//print end_head;
					printf("End_Head: %c", local_par->end_head);
					//print end_sector;
					printf("End_Sector: %c", local_par->end_sector);
					//print end_cylinder;
					printf("End_Cylinder: %c", local_par->end_cylinder);
					//print start_sector;
					printf("Start_Sector: %u", local_par->start_sector);
					//print nr_sectors;
					printf("Nr_Sectors: %u", local_par->nr_sectors);

					//int extend_start = table[temp]->start_sector;		//get location of local MBR
					++temp2;

				}

			}
		}


		//lseek(fd, (long)123 * 512, SEEK_SET);  // lseek to byte 123*512 OR sector 123
		//read(fd, buf, 512);                  // read sector 123 into buf[ ]







	}
	else {
		printf("Failed to open vdisk");
	}


}

