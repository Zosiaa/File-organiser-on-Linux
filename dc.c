//////////////////////////////////////////////
//       Concept:
//      *use man diff*
//      do ls for directory 1
//      do ls for directory 2
//      compare them
//
// Basically when doing a directory comparison a file can have 1 of 4 statuses:
//1. File in both directories, and the same
//2. File in both directories, but different
//3. File only in first directory
//4. File only in second directory
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
void type_group();

#define SIZE 70

struct filestruct {

	char mode[11];
	long int ino;
	int st_nlink;
	char uid[8];
	long int st_size;
	char time[12];
	char filename[1024];

};
struct filestruct d1[SIZE];


struct stat st = { 0 };


int main(int ac, char* argv[])
{

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
		type_group();
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


	if (argv[ac - 1][0] == '-')
	{//printf("taaak\n");
		if ((argv[ac - 1][1] == 's'))
		{
			compares();
		}
		if ((argv[ac - 1][1] == 'i'))
		{
			comparei();
		}
		if ((argv[ac - 1][1] == 'p'))
			comparep();
		if ((argv[ac - 1][1] == 't'))
			comparet();
		if ((argv[ac - 1][1] == 'u'))
			compareu();

	}
*/
	return 0;
}
void type_group()
{

	//protect OUR program, don't move yourself!
	int z, i, k,d;
	char type[SIZE][10];
	for (z = 0; z < SIZE; z++)
	{
		if (d1[z].ino == 0) break;
		if ((strcmp(d1[z].filename,"..") == 0) || (strcmp(d1[z].filename, ".")==0) || (strcmp(d1[z].filename,"dc.c") ==0) || (strcmp(d1[z].filename,"dc") == 0))
		{
			//don't move file
		}
		else {
			for (i = 0; d1[z].filename[i] != '\0'; i++)
			{
				//printf("%d ", d1[z].filename[i]);
				if (d1[z].filename[i] == '.')
				{
					d = 0;
					printf("\ndot!!!! letter by letter = ");
					for (k = i + 1; d1[z].filename[k] != '\0'; ++k)
					{
						if (d1[z].filename[k] == '.')
						{
							i++;
						}
						type[z][d] = d1[z].filename[k];
						printf("%c ", type[z][d]);
						d++;
					}
					type[z][d + 1] = '\0';
					printf("\npo dot = %s", type[z]);
				}

			}
			
			if (stat("./September/ZOSIA1", &st) == -1) {
				printf("make DIR!!\n");
				mkdir("./September/%s", "ZOSIA1", 0700);
			}
			//execlp("mkdir","mkdir", type[z], NULL);
			
		}//end else
	}
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
		strcpy(d1[i].time, 4 + ctime(&info_p->st_mtime));
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




