/*******************************************************************************
    Copyright (C) 2016 Vaishnavi Chidrawar
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    *************************************************************************/
#include<stdio.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<fcntl.h>
#include<error.h>
#include<sys/types.h>
#include<errno.h>
#include<stdlib.h>
void five_arguments(FILE *fp, char *search, char *opt, char *str, char *file);
void option_m(FILE *fp, char *search, char *str);
void print_after_match(FILE *fp, char *search, char *str);
void search_directory(char *dirname, char *search);
void recurse_without_search(char *dirname);
void caret(FILE *fp, char *search);
void sqbkt(FILE *fp, char *search);
void match_letter(FILE *fp, char *search);
void more_files(char *argv2, char *argv3, char *argv4, int flag);
void match_letter1(FILE *fp, char *search);
void only_matching(FILE *fp, char *search, int f);
void doller(FILE *fp, char *search);
void dot(FILE *fp, char *search, int flag );
void extended(FILE *fp, char *search);
void dot1(FILE *fp, char *search);
void plus(FILE *fp, char *search);
void question_mark(FILE *fp, char *search);
void option(FILE *fp, char *search, char *opt, char *file);
void sqbkt1(FILE *fp, char *search);
void linenumber(FILE *fp, char *search);
void grepmyfile(FILE *fp, char *search);
void invert(FILE *fp, char *search, int f);
void ignorecase(FILE *fp, char *search, int f);
void count(FILE *fp, char *search);
void search_alone(FILE *fp, char *search);
void nomatch(FILE *fp, char *search, char *file);
void matchfilename(FILE *fp, char *search, char *file);
void filename(FILE *fp, char *search, char *file, int f);
void no_option(FILE *fp, char *search){
	int i = 0,flag = 0;
	int l = strlen(search);
	if(search[0] == '^'){
		if(search[1] == '[' && search[l-1] == ']')
			match_letter(fp, search);
		else caret(fp,search);
	}
	else if(search[0] == '[' && search[l-1] == ']'){	//check for [a-d]
			if(search[2] == '-')
				sqbkt(fp,search);//function
			else{				//check for [abcd]
				for(i = 1; i < l-1; i++){
					if(!((search[i] >= 65 && search[i] <= 90)||(search[i] >= 97 && search[i] <= 122)))
					flag = 1;//...case to be solved
				}
				if(flag == 0)
					sqbkt1(fp, search);
			}
	}
	else if(search[l-1] == '$'){
		if(search[0] == '[' && search[l-2] == ']')
			match_letter1(fp, search);
		doller(fp, search);
	}
	else if(search[0] == '.' && search[l-1] == '.')
		dot(fp,search, flag);
	else if(search[0] == '.'){
		flag = 1;
		dot(fp, search, flag);		
	}
	else if(search[l-1] == '.')
		dot1(fp, search);
	else	
		grepmyfile(fp, search);
}
void option(FILE *fp, char *search, char *opt, char *file){
	int l = strlen(opt);
	int f = 0;
	if(l == 2){
		if(opt[0] == '-'){
			if(opt[1] == 'n')//for -n option
				linenumber(fp, search);
			if(opt[1] == 'v')
				invert(fp, search, f);
			if(opt[1] == 'i')
				ignorecase(fp, search, f);
			if(opt[1] == 'y')
				ignorecase(fp, search, f);
			if(opt[1] == 'c')
				count(fp, search);
			if(opt[1] == 'E')
				extended(fp, search);
			if(opt[1] == 'x')
				search_alone(fp, search);
			if(opt[1] == 'H')
				filename(fp, search, file, f);
			if(opt[1] == 'o')
				only_matching(fp, search, f);
			if(opt[1] == 'l')
				matchfilename(fp, search, file);
			if(opt[1] == 'L')
				nomatch(fp, search, file);
			if(opt[1] == 'G')
				grepmyfile(fp, search);	
			if(opt[1] == 'q')
				return;
			if(opt[1] == 's')
				grepmyfile(fp, search);		
		
		}
	}
	else if(l == 3){
		if(opt[0] == '-'){
			switch(opt[1]){
				case 'n':
					if(opt[2] == 'c')
						count(fp,search);
					else if(opt[2] == 'v'){
						f = 1;
						invert(fp, search, f);
					}
					else if(opt[2] == 'H'){
						f = 1;
						filename(fp, search, file, f);
					}
					else if(opt[2] == 'G'){
						filename(fp, search, file, f);
					}
					else if(opt[2] == 'o'){
						f = 1;
						only_matching(fp, search, f);
					}
					else if(opt[2] == 'i'){
						f = 1;
						ignorecase(fp, search, f);
					}
					break;
				case 'c':
					if(opt[2] == 'n')
						count(fp, search);
					else if(opt[2] == 'H'){
						f = 2;
						filename(fp, search, file, f);
					}
					else if(opt[2] == 'v'){
						f = 2;
						invert(fp, search, f);
					}
					else if(opt[2] == 'i'){
						f = 2;
						ignorecase(fp, search, f);
					}
					break;
				case 'v':
					if(opt[2] == 'n'){
						f = 1;
						invert(fp, search, f);
					}
					else if(opt[2] == 'c'){
						f = 2;
						invert(fp, search, f);
					}
					else if(opt[2] == 'i'){
						f = 3;
						ignorecase(fp, search, f);
					}
					break;
				case 'i':
					if(opt[2] == 'n'){
						f = 1;
						ignorecase(fp, search, f);
					}
					else if(opt[2] == 'c'){
						f = 2;
						ignorecase(fp, search, f);
					}
					else if(opt[2] == 'v'){
						f = 3;
						ignorecase(fp, search, f);
					}
					break;
			}
		}
	}
}
void five_arguments(FILE *fp, char *search, char *opt, char *str, char *file){
	int f = 0;
		if(!(strcmp(opt,"-m")))
			option_m(fp, search, str);
		else if(!(strcmp(opt,"-A")))
			print_after_match(fp, search, str);
		else if((strcmp(opt, "-n")) == 0){
			if((strcmp(str, "-c")) == 0){
				count(fp, search);
			}
			else if((strcmp(str, "-v")) == 0){
				f = 1;
				invert(fp, search, f);
			}
			else if((strcmp(str, "-H")) == 0){
				f = 1;
				filename(fp, search, file, f);
			}
			else if((strcmp(str, "-G")) == 0)
				filename(fp, search, file, f);
			else if((strcmp(str, "-o")) == 0){
				f = 1;
				only_matching(fp, search, f);
			}
			else if((strcmp(str, "-i")) == 0){
				f = 1;
				ignorecase(fp, search, f);
			}
		}
		else if((strcmp(opt, "-c")) == 0){
			if((strcmp(str, "-n")) == 0)
				count(fp,search);
			else if((strcmp(str, "-H")) == 0){
				f=2;
				filename(fp,search,file,f);
			}
			else if((strcmp(str, "-v")) == 0){
				f=2;
				invert(fp,search,f);
			}
			else if((strcmp(str, "-i")) == 0){
				f = 2;
				ignorecase(fp, search, f);
			}
		}	
		else if((strcmp(opt, "-v")) == 0){
			if((strcmp(str, "-n")) == 0){
				f = 1;
				invert(fp, search, f);
			}
			else if((strcmp(str, "-c")) == 0){
				f = 2;
				invert(fp, search, f);
			}
			else if((strcmp(str, "-i")) == 0){
				f = 3;
				ignorecase(fp, search, f);
			}
		}
		else if((strcmp(opt, "-i")) == 0){
			if((strcmp(str, "-n")) == 0){
				f = 1;
				ignorecase(fp, search, f);
			}
			else if((strcmp(str, "-c")) == 0){
				f = 2;
				ignorecase(fp, search, f);
			}
			else if((strcmp(str, "-i")) == 0){
				f = 3;
				ignorecase(fp, search, f);
			}
		}
}
void extended(FILE *fp, char *search){
	int i;
	for(i = 0; i < strlen(search); i++){
		if(search[i] == '+'){
			plus(fp, search);
			break;
		}
		else if(search[i] == '?'){
			question_mark(fp, search);
			break;
		}
			
	}
}
void plus(FILE *fp, char *search){
	char string[1000];
	char *s = (char *)malloc(sizeof(20));
	int i, j, k, loc;
	char *p, *q, *t;
	i = 0;
	int l = strlen(search);
	while(search[i] != '+' && i < l){
		s[i] = search[i];
		i++;
	}
	s[i] = '\0';
	int l1 = strlen(s);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		p = string;
		q = s;
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q == s)
					t = q;
				p++;
				q++;
			}
			else { 
				if(q == s)
					p++;
				else  {
					p = p - (q- s) + 1;
					q = s;
				}
			}
			if(*q == '\0'){
				loc = (p - (q - s) - string);
				j = loc + l1;
				while(string[j] == s[l1 - 1])
					j++;
				k = i + 1;
				while(search[k] != '\0'){
					if(string[j] == search[k]){
						j++;
						k++;
					}
					else
						break;
					
				}
				if(k == l)
					printf("%s", string);
				else
					q = search;
			}
		}
	}
	
	
}
void question_mark(FILE *fp, char *search){
	char string[1000];
	char *s =(char *)malloc(sizeof(20));
	int i, j, k, loc;
	char *p, *q, *t;
	i = 0;
	int l = strlen(search);
	while(search[i] != '?' && i < l){
		s[i] = search[i];
		i++;
	}
	s[i-1] = '\0';
	int l1 = strlen(s);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		p=string;
		q=s;
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q == s)
					t = q;
				p++;
				q++;
			}
			else { 
				if(q == s)
					p++;
				else  {
					p = p - (q- s) + 1;
					q = s;
				}
			}
			if(*q == '\0'){
				loc = (p - (q - s) - string);
				j = loc + l1;
				if(string[j] == search[i - 1])
					j++;
				k = i + 1;
				while(search[k] != '\0'){
					if(string[j] == search[k]){
						j++;
						k++;
					}
					else
						break;
					
				}
				if(k == l)
					printf("%s", string);
				else
					q = search;
			}
		}
	}
	
	
}
void option_m(FILE *fp, char *search, char *str){		//-m option
	char string[1000];
	int i = 1, n;
	n = atoi(str);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search) != NULL){
			if(i <= n){
				printf("%s", string);
				i++;
			}
			else
				break;
		}
	}
}
void print_after_match(FILE *fp, char *search, char *str){ 	//-A option
	char string[1000];
	int i = 1, n, flag = 0;
	n = atoi(str);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strstr(string,search) != NULL){
			i = 1;
			flag = 1;
			printf("%s", string);
		}
		else{
			if(flag == 1){
				if(i <= n){
					printf("%s", string);
					i++;
				}
				else 
					flag = 0;
			}
		}
	}
}
void search_directory(char *dirname, char *search){		//-r option
	DIR *fd;
	char string[1000];
	struct dirent* in_file;
	FILE *f;
	fd=opendir(dirname);
	if(fd==NULL){
		printf("failed to open directory");
		return ;
	}
	while((in_file=readdir(fd))){
		if (!strcmp (in_file->d_name, "."))
            		continue;
        	if (!strcmp (in_file->d_name, ".."))    
            		continue;
		f=fopen(in_file->d_name,"r");
		if(f==NULL){
			printf("failed to open file:\n");
			return ;
		}
		while(!feof(f)){
			fgets(string,sizeof(string),f);
			if(strstr(string,search)!=NULL)
				printf("./%s:%s",in_file->d_name,string);
		}
		fclose(f);
	}
}
void recurse_without_search(char *dirname){
	DIR *fd;
	char string[1000];
	struct dirent* in_file;
	FILE *f;
	fd=opendir(dirname);
	if(fd==NULL){
		printf("failed to open directory");
		return ;
	}
	while((in_file=readdir(fd))){
		if (!strcmp (in_file->d_name, "."))
            		continue;
        	if (!strcmp (in_file->d_name, ".."))    
            		continue;
		f=fopen(in_file->d_name,"r");
		if(f==NULL){
			printf("failed to open file:\n");
			return ;
		}
		while(!feof(f)){
			fgets(string,sizeof(string),f);
				printf("./%s:%s",in_file->d_name,string);
		}
		fclose(f);
	}
}
void linenumber(FILE *fp, char *search){
	char string[1000];
	int i = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		i++;
		if(strstr(string,search) != NULL)
			printf("%d:%s",i, string);
	}
}
void invert(FILE *fp, char *search, int f){
	char string[1000];
	int i = 0, c = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		i++;
		if(strstr(string,search) == NULL){
			c++;
			if(f == 1)
				printf("%d:%s",i, string);
			else if(f == 0)
				printf("%s", string);
		}
	}
	if(f == 2)
		printf("%d\n", c);
}
void ignorecase(FILE *fp, char *search, int f){
	char string[1000],string1[1000];
	char search1[20];
	int i, j = 0, c = 0;
	for(i = 0; i < strlen(search); i++){
		search1[i] = tolower(search[i]);
	}
	search1[i] = '\0';
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		j++;
		for(i = 0; i < strlen(string); i++)
			string1[i] = tolower(string[i]);
		string1[i] = '\0';
		if(strstr(string1,search1) != NULL){
			if(f == 0)
				printf("%s", string);
			else if(f == 1)
				printf("%d:%s", j, string);
			else if(f == 2)
				c++;
		}
		else{
			if( f == 3)
				printf("%s", string);
		}
	}
	if(f == 2)
		printf("%d\n", c);
}
void search_alone(FILE *fp, char *search){
	char string[1000];
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strcmp(string, search) == 0)
			printf("%s", string);
 	}	
}
void count(FILE *fp, char *search){
	char string[1000];
	int c = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strstr(string,search) != NULL)
			c++;
	}
	printf("%d\n", c);
}
void filename(FILE *fp, char *search, char *file, int f){
	char string[1000];
	int i = 0, c = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		i++;
		if(strstr(string,search) != NULL){
			c++;
			if(f == 1)
				printf("%s:%d:%s", file, i, string);
			else if(f == 0)	
				printf("%s : %s", file, string);
		}
	}
	if(f == 2)
		printf("%s:%d", file, c);
}
void more_files(char *argv2, char *argv3, char *argv4, int flag){
	FILE *fp, *fd;
	fp = fopen(argv3, "r");
	if(fp == NULL){
		printf("error in opening file\n");
		return;
	}
	fd = fopen(argv4, "r");
	if(fd == NULL){
		printf("errorn in opening file\n");
		return;
	}
	if(flag == 0){
		matchfilename(fp, argv2, argv3);
		matchfilename(fd, argv2, argv4);
	}
	else if(flag == 1){
		nomatch(fp, argv2, argv3);
		nomatch(fd, argv2, argv4);
	}
}
void matchfilename(FILE *fp, char *search, char *file){
	char string[1000];
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strstr(string,search) != NULL){
			printf("%s\n", file);
			return;
		}
	}
}
void nomatch(FILE *fp, char *search, char *file){
	char string[1000];
	int flag = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strstr(string,search) != NULL)
			flag = 1;
	}
	if(flag == 0)
		printf("%s\n", file);
}
void only_matching(FILE *fp, char *search, int f){
	char string[1000];
	char *p, *q, *t;
	int i = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		i++;
		p = string;
		q = search;
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q == search)
					t = q;
				p++;
				q++;
			}
			else { 
				if(q == search)
					p++;
				else  {
					p = p - (q- search) + 1;
					q = search;
				}
			}
			if(*q == '\0'){
				if( f == 0)
					printf("%s\n",search);
				else if(f == 1)
					printf("%d:%s\n", i, search);
				q=search;
			}
		}
	}
}
void caret(FILE *fp, char *search){
	int i,j;
	int l = strlen(search);
	char string[1000];
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		for(i = 0,j = 1; i < (l-1); i++, j++){
			if(string[i] == search[j])
				continue;
			else 
				break;
		}
		if(i == (l-1))
			printf("%s", string);

	}
}
void doller(FILE *fp, char *search){
	int i,j,l1;
	int l = strlen(search);
	char string[1000];
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		l1 = strlen(string);
		for(i = l1-1, j = l-2; j >= 0; i--, j--){
			if(string[i] == search[j])
				continue;
			else
				break;
		}
		if(j == (l-1))
			printf("%s",string);
	}
}
void sqbkt(FILE *fp, char *search){
	char string[1000];
	int i,flag = 0, j;
	int l = strlen(search);
	int a = search[1];
	int b = search[l-2];
	char *s = (char *)malloc(20);
	for(i = 0; i <= (b-a); i++)
		s[i] = a+i;
	int l1 = strlen(s);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		for( i= 0; i < l1; i++){
			for(j = 0; j < strlen(string); j++){
				if(string[j] == s[i])
					flag = 1;
			}
		}
		if(flag == 1)
			printf("%s", string);
		flag = 0;
	}
	
}
void match_letter(FILE *fp, char *search){
	char string[1000];
	int i;
	int flag = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		for(i = 2; i<(strlen(search)-1); i++){
			if(string[0] == search[i])
				flag = 1;
		}
		if(flag == 1)
			printf("%s", string);
		flag = 0;
	}
	
}
void match_letter1(FILE *fp, char *search){
	char string[1000];
	int i;
	int flag = 0;
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		for(i = 1; i<(strlen(search)-2); i++){
			if(string[strlen(string)-1] == search[i])
				flag = 1;
		}
		if(flag == 1)
			printf("%s", string);
		flag = 0;
	}
}
void dot(FILE *fp, char *search, int flag){
	char string[1000];
	char *p, *q, *r, *t;
	int l = strlen(search);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		int l1 = strlen(string);
		p = string;
		r = &search[1];
		q = r;
		if(flag == 0)
			search[l-1] = '\0';
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q == r)
					t = q;
				p++;
				q++;
			}
			else { 
				if(q == r)
					p++;
				else  {
					p = p - (q-r) + 1;
					q = r;
				}
			}
			if(*q == '\0'){
				if((p-(q-r)-string) == 0)
					q = search;
				else if((flag == 0) && ((p-(q-r)-string) == (l1-1-l)))
						break;	
				else{
					printf("%s", string);
					break;
				}
			}
		}
	}
}
void dot1(FILE *fp, char *search){
	char string[1000];
	char *p, *q, *t;
	int l = strlen(search);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		int l1 = strlen(string);
		p = string;
		q = search;
		search[l-1] = '\0';
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q == search)
					t = q;
				p++;
				q++;
			}
			else { 
				if(q == search)
					p++;
				else  {
					p = p - (q-search) + 1;
					q = search;
				}
			}
			if(*q == '\0'){
				 if((p-(q-search)-string) == (l1-1-l))
						break;	
				else{
					printf("%s", string);
					break;
				}
			}
		}
	}
}
void sqbkt1(FILE *fp, char *search){
	char string[1000];
	int i, j, flag = 0;
	int l1 = strlen(search);
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		for(i = 1; i < l1-1; i++){
			for(j = 0; j < strlen(string); j++){
				if(string[j] == search[i])
					flag = 1;
			}
		}
		if(flag == 1)
			printf("%s", string);
		flag = 0;
	}
}
void grepmyfile(FILE *fp, char *search){
	char string[1000];
	while(!feof(fp)){
		fgets(string, sizeof(string), fp);
		if(strstr(string,search) != NULL)
			printf("%s", string);
	}
}
void usage(){
	printf("usage:  ./project [option].. [pattern] [file].. \n");
	printf("\t./project [pattern] [file]  \n");
	printf("pattern is , by default, a basic regular expession\n");
	printf("-E	pattern is an extended regular expression\n");
	printf("-G	pattern is a basic regular expression\n");
	printf("-i	ignore case distinctions\n");
	printf("-x	force pattern to match only whole words\n");
	printf("-s	supress messeges\n");
	printf("-v	select non-matching lines\n");
	printf("-m NUM  stop after NUM matches\n");
	printf("-n	print line number with output lines\n");
	printf("-H	print the file name for each match\n");
	printf("-o	show only part of a line matching pattern\n");
	printf("-q	suppress all normal output\n");
	printf("-d ACTION hanndle directories\n");
	printf("	ACTIOn is 'recurse'\n");
	printf("-r	like -d recurse\n");
	printf("-L	print only names of FILEs containing no match\n");
	printf("-l	print only names of FILEs containing match\n");
	printf("-c	print only count of matching lines\n");
	printf("-A NUM print NUM lines of trailing context\n");
}
int main(int argc, char *argv[]){
	FILE *fp;
	int x, flag = 0;
	char *dirname;
	char *search;
	char *opt,*str;
	char *file;
	search=(char *)malloc(20);
	if(argc == 2){
		if((strcmp(argv[1], "--help"))==0)
			usage();
		return 0;
	}
	if(argc < 3){
		printf("usage: ./project option <substring> <filename>\n");
		printf("TRY 'grep --help' for more information\n");
		return EINVAL;
	}
	if(argc == 3){
		fp = fopen(argv[2], "r");
		if(fp == NULL){
		perror("error in opening file:\n"); 		
			return errno;
		}
		strcpy(search, argv[1]);
		no_option(fp, search);
	}
	else if(argc == 4){
		opt = (char *)malloc(5);
		strcpy(opt, argv[1]);
		if(!(strcmp(opt,"-d"))){
			if((strcmp(argv[2],"recurse"))==0){
				dirname = (char *)malloc(10);
				strcpy(dirname,argv[3]);
				recurse_without_search(dirname);
				return 0;
			}	
		}
		x = strcmp(opt,"-r");
		if(x == 0){
			strcpy(search, argv[2]);
			dirname = (char *)malloc(10);
			strcpy(dirname, argv[3]);
			search_directory(dirname, search);
			return 0;
		}	
		file = (char *)malloc(20);
		fp = fopen(argv[3],"r");
		if(fp == NULL){
			perror("error in opening file:\n");
			return errno;
		}
		strcpy(search, argv[2]);
		strcpy(file, argv[3]);
		option(fp, search, opt, file);
	}
	else if(argc == 5){
		if((strcmp(argv[1], "-l")) == 0){
			more_files(argv[2], argv[3], argv[4], flag);
		}
		else if((strcmp(argv[1], "-L")) == 0){
			flag = 1;
			more_files(argv[2], argv[3], argv[4], flag);
		}
		if((strcmp(argv[1], "-d")) == 0){
			if((strcmp(argv[2],"recurse")) == 0){
				dirname = (char *)malloc(10);
				strcpy(dirname, argv[4]);
				strcpy(search, argv[3]);
				search_directory(dirname, search);
				return 0;
			}
		}
		fp=fopen(argv[4],"r");
		if(fp == NULL){
			perror("error in opening file:\n");
			return errno;
		}
		opt = (char *)malloc(5);
		strcpy(opt, argv[1]);
		str = (char *)malloc(5);
		strcpy(str, argv[2]);
		strcpy(search, argv[3]);
		strcpy(file, argv[4]);
		fp = fopen(argv[4],"r");
		if(fp == NULL){
			perror("error in opening file:\n");
			return errno;
		}			
		five_arguments(fp, search, opt, str, file);	
	}
	free(search);
	return 0;	
}
