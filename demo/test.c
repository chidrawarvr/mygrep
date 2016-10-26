#include<stdio.h>
#include<stdlib.h>
#include"integer.h"
int main(){
	char *str;
	str=(char *)malloc(128);
	integer *a;
	init(a);
	a=createintegerfromstring(str);
	printinteger(*a);
	return 1;
}
