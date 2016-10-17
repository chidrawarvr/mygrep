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
void five_arguments(FILE *fp, char *search, char *opt, char *str);
void option_m(FILE *fp, char *search, char *str);
void print_after_match(FILE *fp, char *search, char *Str);
void search_directory(char *dirname, char *search);
void recurse_without_search(char *dirname);
void caret(FILE *fp, char *search);
void sqbkt(FILE *fp, char *search);
void match_letter(FILE *fp, char *search);
void match_letter1(FILE *fp, char *search);
void only_matching(FILE *fp, char *search);
void doller(FILE *fp, char *search);
void dot(FILE *fp, char *search, int flag );
void dot1(FILE *fp, char *search);
void option(FILE *fp, char *search, char *opt, char *file);
void sqbkt1(FILE *fp, char *search);
void linenumber(FILE *fp, char *search);
void grepmyfile(FILE *fp, char *search);
void invert(FILE *fp, char *search, int f);
void ignorecase(FILE *fp, char *search);
void count(FILE *fp, char *search);
void search_alone(FILE *fp, char *search);
void nomatch(FILE *fp, char *search, char *file);
void matchfilename(FILE *fp, char *search, char *file);
void filename(FILE *fp, char *search, char *file, int f);
void no_option(FILE *fp, char *search){
	int i=0,flag=0;
	int l=strlen(search);
	if(search[0]=='^'){
		if(search[1]=='[' && search[l-1]==']')
			match_letter(fp,search);
		else caret(fp,search);
	}
	else if(search[0]=='[' && search[l-1]==']'){	//check for [a-d]
			if(search[2]=='-')
				sqbkt(fp,search);//function
			else{				//check for [abcd]
				for(i=0;i<l;i++){
					if(!((search[i]>=65 && search[i]<=90)||(search[i]>=97 && search[i]<=122)))
					flag=1;//...case to be solved
				}
				if(flag==0)
					sqbkt1(fp,search);
			}
	}
	else if(search[l-1]=='$'){
		if(search[0]=='[' && search[l-2]==']')
			match_letter1(fp,search);
		doller(fp,search);
	}
	else if(search[0] == '.' && search[l-1] == '.')
		dot(fp,search,flag);
	else if(search[0]=='.'){
		flag=1;
		dot(fp,search,flag);		
	}
	else if(search[l-1]=='.')
		dot1(fp,search);
	else	
		grepmyfile(fp,search);
}
void option(FILE *fp, char *search, char *opt, char *file){
	int l=strlen(opt);
	int f=0;
	if(l==2){
		if(opt[0]=='-'){
			if(opt[1]=='n')//for -n option
				linenumber(fp,search);
			if(opt[1]=='v')
				invert(fp,search,f);
			if(opt[1]=='i')
				ignorecase(fp,search);
			if(opt[1]=='y')
				ignorecase(fp,search);
			if(opt[1]=='c')
				count(fp,search);
			if(opt[1]=='x')
				search_alone(fp,search);
			if(opt[1]=='H')
				filename(fp,search,file,f);
			if(opt[1]=='o')
				only_matching(fp,search);
			if(opt[1]=='l')
				matchfilename(fp,search,file);
			if(opt[1]=='L')
				nomatch(fp,search,file);
			if(opt[1]=='G')
				grepmyfile(fp,search);	
			//if(opt[1]=='q')
			//	do_nothing(fp,search);
			if(opt[1]=='s')
				grepmyfile(fp,search);		
		
		}
	}
	else if(l==3){
		if(opt[0]=='-'){
			switch(opt[1]){
				case 'n':
					if(opt[2] == 'c')
						count(fp,search);
					else if(opt[2] == 'v'){
						f=1;
						invert(fp,search,f);
					}
					else if(opt[2] == 'H'){
						f=1;
						filename(fp,search,file,f);
					}
					else if(opt[2] == 'G'){
						filename(fp,search,file,f);
					}
					break;
				case 'c':
					if(opt[2] == 'n')
						count(fp,search);
					else if(opt[2] == 'H'){
						f=2;
						filename(fp,search,file,f);
					}
					else if(opt[2] == 'v'){
						f=2;
						invert(fp,search,f);
					}
			
			}
		}
	}
}
void five_arguments(FILE *fp, char *search, char *opt, char *str){
		if(!(strcmp(opt,"-m")))
			option_m(fp,search,str);
		else if(!(strcmp(opt,"-A")))
			print_after_match(fp,search,str);	
}
void option_m(FILE *fp, char *search, char *str){		//-m option
	char string[1000];
	int i=1,n;
	n=atoi(str);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL){
			if(i<=n){
				printf("%s",string);
				i++;
			}
			else
				break;
		}
	}
}
void print_after_match(FILE *fp, char *search, char *str){ 	//-A option
	char string[1000];
	int i=1,n,flag=0;
	n=atoi(str);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL){
			i=1;
			flag=1;
			printf("%s",string);
		}
		else{
			if(flag==1){
				if(i<=n){
					printf("%s",string);
					i++;
				}
				else 
					flag=0;
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
	int i=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		i++;
		if(strstr(string,search)!=NULL)
			printf("%d:%s",i,string);
	}
}
void invert(FILE *fp, char *search, int f){
	char string[1000];
	int i=0,c=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		i++;
		if(strstr(string,search)==NULL){
			c++;
			if(f==1)
				printf("%d:%s",i,string);
			else if(f==0)
				printf("%s",string);
		}
	}
	if(f==2)
		printf("%d",c);
}
void ignorecase(FILE *fp, char *search){
	char string[1000],string1[1000];
	char search1[20];
	int i;
	for(i=0;i<strlen(search);i++){
		search1[i]=tolower(search[i]);
	}
	search1[i]='\0';
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=0;i<strlen(string);i++)
			string1[i]=tolower(string[i]);
		string1[i]='\0';
		if(strstr(string1,search1)!=NULL)
			printf("%s",string);
	}
}
void search_alone(FILE *fp, char *search){
	char string[1000];
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		//case to be solved
	}	
}
void count(FILE *fp, char *search){
	char string[1000];
	int c=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL)
			c++;
	}
	printf("%d",c);
}
void filename(FILE *fp, char *search, char *file, int f){
	char string[1000];
	int i=0,c=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		i++;
		if(strstr(string,search)!=NULL){
			c++;
			if(f==1)
				printf("%s:%d:%s", file,i,string);
			else if(f==0)	
				printf("%s : %s",file,string);
		}
	}
	if(f==2)
		printf("%s:%d", file,c);
}
void matchfilename(FILE *fp, char *search, char *file){
	char string[1000];
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL){
			printf("%s",file);
			return;
		}
	}
}
void nomatch(FILE *fp, char *search, char *file){
	char string[1000];
	int flag=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL)
			flag=1;
	}
	if(flag==0)
		printf("%s",file);
}
void only_matching(FILE *fp, char *search){
	char string[1000];
	char *p, *q, *t;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		p=string;
		q=search;
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q==search)
					t=q;
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
				printf("%s\n",search);
				q=search;
			}
		}
	}
}
void caret(FILE *fp, char *search){
	int i,j;
	int l=strlen(search);
	char string[1000];
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=0,j=1;i<(l-1);i++,j++){
			if(string[i]==search[j])
				continue;
			else 
				break;
		}
		if(i==(l-1))
			printf("%s",string);

	}
}
void doller(FILE *fp, char *search){
	int i,j,l1;
	int l=strlen(search);
	char string[1000];
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		l1=strlen(string);
		for(i=l1-1,j=l-2;j>=0;i--,j--){
			if(string[i]==search[j])
				continue;
			else
				break;
		}
		if(j==(l-1))
			printf("%s",string);
	}
}
void sqbkt(FILE *fp, char *search){
	char string[1000];
	int i,flag=0,j;
	int l=strlen(search);
	int a=search[1];
	int b=search[l-2];
	char *s=(char *)malloc(20);
	for(i=0;i<=(b-a);i++)
		s[i]=a+i;
	int l1=strlen(s);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=0;i<l1;i++){
			for(j=0;j<strlen(string);j++){
				if(string[j]==s[i])
					flag=1;
			}
		}
		if(flag==1)
			printf("%s",string);
		flag=0;
	}
	
}
void match_letter(FILE *fp, char *search){
	char string[1000];
	int i;
	int flag=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=2;i<(strlen(search)-1);i++){
			if(string[0]==search[i])
				flag=1;
		}
		if(flag==1)
			printf("%s",string);
		flag=0;
	}
	
}
void match_letter1(FILE *fp, char *search){
	char string[1000];
	int i;
	int flag=0;
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=1;i<(strlen(search)-2);i++){
			if(string[strlen(string)-1] == search[i])
				flag=1;
		}
		if(flag==1)
			printf("%s",string);
		flag=0;
	}
}
void dot(FILE *fp, char *search, int flag){
	char string[1000];
	char *p, *q, *r, *t;
	int l=strlen(search);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		int l1=strlen(string);
		p=string;
		r=&search[1];
		q=r;
		if(flag==0)
			search[l-1]='\0';
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q==r)
					t=q;
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
					q=search;
				else if((flag==0) && ((p-(q-r)-string) == (l1-1-l)))
						break;	
				else{
					printf("%s",string);
					break;
				}
			}
		}
	}
}
void dot1(FILE *fp, char *search){
	char string[1000];
	char *p, *q, *t;
	int l=strlen(search);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		int l1=strlen(string);
		p=string;
		q=search;
		search[l-1]='\0';
		while(*p != '\0' && *q != '\0') {
			if(*p == *q)  {
				if(q==search)
					t=q;
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
					printf("%s",string);
					break;
				}
			}
		}
	}
}
void sqbkt1(FILE *fp, char *search){
	char string[1000];
	int i,j,flag=0;
	int l1=strlen(search);
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		for(i=1;i<l1-1;i++){
			for(j=0;j<strlen(string);j++){
				if(string[j]==search[i])
					flag=1;
			}
		}
		if(flag==1)
			printf("%s",string);
		flag=0;
	}
}
void grepmyfile(FILE *fp, char *search){
	char string[1000];
	while(!feof(fp)){
		fgets(string,sizeof(string),fp);
		if(strstr(string,search)!=NULL)
			printf("%s",string);
	}
}
void usage(){
	printf("usage:  ./mygrep [option] [pattern] [file] || \n");
	printf("\t./mygrep [pattern] [file] || \n");
	printf("\t./mygrep [option] [number] [pattern] [file] \n");
}
int main(int argc, char *argv[]){
	FILE *fp;
	int x;
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
	if(argc <3){
		printf("usage: ./mygrep option <substring> <filename>\n");
		return EINVAL;
	}
	if(argc==3){
		fp=fopen(argv[2],"r");
		if(fp==NULL){
		perror("error in opening file:\n"); 		
			return errno;
		}
		strcpy(search,argv[1]);
		no_option(fp,search);
	}
	else if(argc==4){
		opt=(char *)malloc(5);
		strcpy(opt,argv[1]);
		if(!(strcmp(opt,"-d"))){
			if((strcmp(argv[2],"recurse"))==0){
				dirname=(char *)malloc(10);
				strcpy(dirname,argv[3]);
				recurse_without_search(dirname);
				return 0;
			}	
		}
		x=strcmp(opt,"-r");
		if(x==0){
			dirname=(char *)malloc(10);
			strcpy(dirname,argv[3]);
			search_directory(dirname,search);
			return 0;
		}	
		file=(char *)malloc(20);
		fp=fopen(argv[3],"r");
		if(fp==NULL){
			perror("error in opening file:\n");
			return errno;
		}
		strcpy(search,argv[2]);
		strcpy(file,argv[3]);
		option(fp,search,opt,file);
	}
	else if(argc==5){
		if((strcmp(argv[1],"-d"))==0){
			if((strcmp(argv[2],"recurse"))==0){
				dirname=(char *)malloc(10);
				strcpy(dirname,argv[4]);
				strcpy(search,argv[3]);
				search_directory(dirname,search);
				return 0;
			}
		}
		fp=fopen(argv[4],"r");
		if(fp==NULL){
			perror("error in opening file:\n");
			return errno;
		}
		opt=(char *)malloc(5);
		strcpy(opt,argv[1]);//..............................
		str=(char *)malloc(5);
		strcpy(str,argv[2]);
		strcpy(search,argv[3]);
		fp=fopen(argv[4],"r");
		if(fp==NULL){
			perror("error in opening file:\n");
			return errno;
		}			
		five_arguments(fp,search,opt,str);	
	}
	free(search);
	return 0;	
}
