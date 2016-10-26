#include<stdio.h>
#include<string.h>
//chaaaaaar fg
int stringreplace(char *text, char *orig, char *new)
{
	int count = 0, c = 0, flag;
	char *p, *q, *r, *t;
	int lo, ln;

	p = text;
	q = orig;
	r = new;

	lo = strlen(orig);
	ln = strlen(new);

	while((*p != '\0')&&(*q != '\0'))
	{
		if(*p == *q)
		{
			if(q == orig)
				t = p;		//t for 1st pos of match
	 		p++;
			q++;
		}
		else if(q == orig)
			p++;
		else
		{
			p = p - (q - orig) + 1;
			q = orig;
		}

		//if match found
		if(*q == '\0')
		{
			q = orig;    //reset q

			if(lo > ln)
			{
				while(*r != '\0')
				{
					if(*t != *r)
						count++;
					*t = *r;
					t++;
					r++;
				}
				r = new;    //reset r
				flag = 1;
				c = 0;
				while(flag != 0)
				{
					*t = *p;
					t++;
					c++;
					if(*p == '\0')
						flag = 0;
					p++;
				}
				p = p - c - (lo - ln);     //reset p
			}
			else          //lo <= ln
			{
				c = 0;
				while(*p != 0)
				{
					p++;
					c++;
				}
				while( (t - p) != lo)
				{
					*(p+(ln-lo)) = *p;
					p--;
				}
				while(*r != '\0')
				{
					if(*t != *r)
						count++;
					*t = *r;
					t++;
					r++;
				}
				r = new;     //reset r
				p = p + (ln - lo);
			}
		}
	}

	return count;
}


int main() 
{
	char text[128], orig[128] ,new[128];
	int count;
	while(scanf("%s%s%s", text, orig, new) != -1)
	{
		count = stringreplace(text, orig, new);
		printf("%d %s\n", count, text);
	}
	return 0;
}
