﻿//////////////////////////////////////////////
//       Concept:
//   1. Create array of srtuctures to list all files
//	 2. Traverse through them and check their type
//   3. Create new directory with type name(if doesn't exist)
//	 4. Move file to directory
//  Warning! Do not move:  .  ..  PROGRAMNAME   PROGRAMNAMEC
//////////////////////////////////////////////
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include  <grp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void do_struct(char[]);
void dostat(char*, int i);
void get_file_info(char*, struct stat*, int i);
void mode_to_letters(int, char[]);
char* uid_to_name(uid_t);
char* gid_to_name(gid_t);
void do_struct_sort(char[], bool reverse);
void print();
void type_group(const char* s2);
void date_group(const char* s2);
char* merge_path(const char* s1, const char* s2);
void move_file(char* source, const char* dest, const char* s2);

#define SIZE 70
#define PROGRAMNAME "dc"
#define PROGRAMNAMEC "dc.c"

struct filestruct {

	char mode[11];
	long int ino;
	int st_nlink;
	char uid[8];
	long int st_size;
	char time[30];
	char filename[1024];

};
struct filestruct d1[SIZE];


struct stat st = { 0 };


int main(int ac, char* argv[])
{
	//const char* pt =  path();
	char pt[PATH_MAX];
	if (getcwd(pt, sizeof(pt)) != NULL) {
		//	printf("Current working dir: %s\n", s1);
	}
	else {
		perror("getcwd() error");
	}
	do_struct(".");
	print();
	printf("\nDone! \n");
	printf("\nargv1 = %s \n" , argv[1]);
	if (ac == 1)
	{
		printf("\n no arguments, only deletion of directories");
		//delete_dir(); TO DO!!
	}
	if (strcmp(argv[1], "type") == 0)
	{
		printf("\nType group option \n");
		type_group(pt);
	}
	else if (strcmp(argv[1], "date") == 0)
	{
		printf("\nDate group option, time = %c%c%c  cal = %s \n", d1[0].time[0], d1[0].time[1], d1[0].time[2], d1[0].time);
		date_group(pt);
	}
	else
	printf("\nTo nie type \n");
	/*
		printf("directory 1 : \n");
	do_struct(argv[1], 1);
	printf(" struct directory 2 : \n");
	do_struct(argv[2], 2);
	if(ac==1)
		do_struct(".");
	if(argv[ac-1][0] == '-')
		{if((argv[ac-1][1] == 't' && argv[ac-1][2] == 'r') || (argv[ac-1][1] == 'r' && argv[ac-1][2] == 't'))
			do_struct_sort(*argv, true);
				
		if (argv[ac-1][1] == 't')
			do_struct_sort(*argv, false);	
		}	
	else
		while( --ac){
			printf("%s:\n", *++argv);
			do_struct(*argv);
		}


*/
	return 0;
}
void date_group(const char* s2)
{
	//protect OUR program, don't move yourself!
	int z, i, k,d;
	char mnth[SIZE][6];
	char year[SIZE][6];
	for (z = 0; z < SIZE; z++)
	{
		if (d1[z].ino == 0) break;
		if ((strcmp(d1[z].filename,"..") == 0) || (strcmp(d1[z].filename, ".")==0) || (strcmp(d1[z].filename,PROGRAMNAME) ==0) || (strcmp(d1[z].filename,PROGRAMNAMEC) == 0))
		{
			//don't move file
		}
		else {
			//get Year
			year[z][0] = '/';
			year[z][1] = d1[z].time[20];
			year[z][2] = d1[z].time[21];
			year[z][3] = d1[z].time[22];
			year[z][4] = d1[z].time[23];
			year[z][5] = '\0';
			printf("YEAR = %s \n", year[z]);
			//create Directory
			const char* path = merge_path(s2, year[z]);
			if (stat(path, &st) == -1) {
				mkdir(path, 0700);
			}

			mnth[z][0] = '/';
			mnth[z][1] = d1[z].time[4];
			mnth[z][2] = d1[z].time[5];
			mnth[z][3] = d1[z].time[6];
			mnth[z][4] = '\0';
			printf("MONTH = %s \n", mnth[z]);
			const char* path2 = merge_path(path, mnth[z]);
			if (stat(path2, &st) == -1) {
				mkdir(path2, 0700);
			}

			//move file
			char* r_name = merge_path(s2, "/");
			char* o_name = merge_path(r_name, d1[z].filename);

			char* new_name = merge_path(path2, "/");
			char* new_name2 = merge_path(new_name, d1[z].filename);
			printf("MOVE! old name = %s, new name = %s\n", o_name, new_name2);

			if (rename(o_name, new_name2) == 0)
			{
				printf("File renamed successfully.\n");
			}
			else
			{
				printf("Unable to rename files. Please check files exist and you have permissions to modify files.\n");
			}
		/*	{//if type of file was given after dot in filename field
				//create directories
					const char* path = merge_path(s2, type[z]);
					if (stat(path, &st) == -1) {
						printf("error");
						mkdir(path, 0700);
					}
					//move file
					char* r_name = merge_path(s2, "/");
					char* o_name = merge_path(r_name, d1[z].filename);
					char* new_name = merge_path(path, "/");
					char* new_name2 = merge_path(new_name, d1[z].filename);
					printf("MOVE! old name = %s, new name = %s\n",  o_name, new_name2 );
					
					if (rename(o_name, new_name2) == 0)
					{
						printf("File renamed successfully.\n");
					}
					else
					{
						printf("Unable to rename files. Please check files exist and you have permissions to modify files.\n");
					}
				}*/

			
		}//end else
	}
}

void type_group(const char* s2)
{
	//protect OUR program, don't move yourself!
	int z, i, k, d;
	char type[SIZE][10];
	for (z = 0; z < SIZE; z++)
	{
		if (d1[z].ino == 0) break;
		if ((strcmp(d1[z].filename, "..") == 0) || (strcmp(d1[z].filename, ".") == 0) || (strcmp(d1[z].filename, PROGRAMNAME) == 0) || (strcmp(d1[z].filename, PROGRAMNAMEC) == 0))
		{
			//don't move file
		}
		else {
			for (i = 0; d1[z].filename[i] != '\0'; i++)
			{
				if (d1[z].filename[i] == '.')
				{
					d = 0;
					type[z][d] = '/';
					d++;
					printf("\ndot!!!! letter by letter = ");
					for (k = i + 1; d1[z].filename[k] != '\0'; ++k)
					{
						type[z][d] = d1[z].filename[k];
						printf("%c ", type[z][d]);
						d++;
					}
					type[z][d] = '\0';
					printf("\npo dot = %s", type[z]);
				}

			}
			if (type[z][0] == '/')
			{//if type of file was given after dot in filename field
				//create directories
				const char* path = merge_path(s2, type[z]);
				if (stat(path, &st) == -1) {
					mkdir(path, 0700);
				}
				//move file
				char* r_name = merge_path(s2, "/");
				char* o_name = merge_path(r_name, d1[z].filename);
				char* new_name = merge_path(path, "/");
				char* new_name2 = merge_path(new_name, d1[z].filename);
				printf("MOVE! old name = %s, new name = %s\n", o_name, new_name2);

				if (rename(o_name, new_name2) == 0)
				{
					printf("File renamed successfully.\n");
				}
				else
				{
					printf("Unable to rename files. Please check files exist and you have permissions to modify files.\n");
				}
			}
			else
			{
				//create directory for programs
				const char* path = merge_path(s2, "/programs");
				if (stat(path, &st) == -1) {
					printf("error");
					mkdir(path, 0700);
				}
				//move file
				char* r_name = merge_path(s2, "/");
				char* o_name = merge_path(r_name, d1[z].filename);
				char* new_name = merge_path(path, "/");
				char* new_name2 = merge_path(new_name, d1[z].filename);
				printf("MOVE! old name = %s, new name = %s\n", o_name, new_name2);

				if (rename(o_name, new_name2) == 0)
				{
					printf("File renamed successfully.\n");
				}
				else
				{
					printf("Unable to rename files. Please check files exist and you have permissions to modify files.\n");
				}

			}

		}//end else
	}
}
void move_file(char* source, const char* dest, const char* s2)
{
	printf("\n");
	char* file = malloc(strlen(source) + 1);
	char* location = malloc(strlen(dest) + 1);
	char* path = malloc(strlen(s2) + 1);
	/*const char* file = source;
	const char* location = dest;
	*/
	
	strcpy(location, dest);
	//printf("skopieowane 1!\n");
	strcpy(file,source);
	strcpy(path, s2);
	
	printf("source = %s, destination = %s \n", file, location);
	char newplace[50];

	if (location[0] == '/')				//check to see if input is a path
	{
		strcat(location, "/");			//if argument is a Full Path, prepare to mv to end of path.
		strcat(location, file);
		if (rename(file, location) == 0)
			printf("Successful\n");
		else
			printf("Error:\nDirectory not found file =%s, location = %s\n", file, location);
	}
	else
	{
		DIR* isD;
		isD = opendir(location); 				// check if argument is a DIR in CWD

		if (isD == NULL)
		{
			if (rename(file, location) != 0)
				printf("Error: File not moved\n");
			else
				printf("Successful\n");
		}
		else
		{
			char* ptrL;
			ptrL = getcwd(newplace, 50);		// get current working directory path 
			strcat(newplace, "/");
			strcat(newplace, location);			// attach mv location to path ptrL
			strcat(newplace, "/");
			strcat(newplace, file);				// keep original file name
			if (rename(file, ptrL) != -1)
				printf("Successful\n");
			else
				printf("Error:\nDirectory not found in CWD\n");
			closedir(isD);
		}
	}


}


char* merge_path(const char* s1, const char* s2)
{
	char* result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
	// in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}
 

void do_struct(char dirname[])
{
	DIR* dir_ptr;
	struct dirent* direntp;
	int i = 0;
	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		chdir(dirname);
		while ((direntp = readdir(dir_ptr)) != NULL)
		{
			dostat(direntp->d_name, i);
			i++;
		}
		closedir(dir_ptr);
		chdir("..");
	}
}

void dostat(char* filename, int i) {
	struct stat info;
	if (stat(filename, &info) == -1)
		perror(filename);
	else
		get_file_info(filename, &info, i);
}

void print()
{
	printf("D1: \n\n\n");
	for (int i = 0; i < 70; i++)
	{
		printf(" %s, %lu, %4d, %-8s, %-8ld, %.12s, %s\n", d1[i].mode, d1[i].ino, d1[i].st_nlink, d1[i].uid, d1[i].st_size, d1[i].time, d1[i].filename);
	}
}

void get_file_info(char* filename, struct stat* info_p, int i) {

	char modestr[11];
	mode_to_letters(info_p->st_mode, modestr);


		strcpy(d1[i].mode, modestr);
		//printf("%s ", d1[i].mode );
		d1[i].ino = info_p->st_ino;
		//printf("%lu ", d1[i].ino );

		d1[i].st_nlink = (int)info_p->st_nlink;
		strcpy(d1[i].uid, uid_to_name(info_p->st_uid));
		d1[i].st_size = (long)info_p->st_size;
		strcpy(d1[i].time, ctime(&info_p->st_mtime));
		 //        printf("%s ", d1[i].time );
	   //
		strcpy(d1[i].filename, filename);
		//printf("%s /n", d1[i].filename);
		
}




void mode_to_letters(int mode, char str[]) {
	strcpy(str, "----------");
	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

char* uid_to_name(uid_t uid) {
	struct passwd* getpwuid(), * pw_ptr;
	static char numstr[10];
	if ((pw_ptr = getpwuid(uid)) == NULL) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char* gid_to_name(gid_t gid) {
	struct group* getgrid(), * grp_ptr;
	static char numstr[10];
	if ((grp_ptr = getgrgid(gid)) == NULL)
	{
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
