


	Name: 	Vaishnavi Ravindra Chidrawar

	MIS NO:	111508020
	
	Title of mini project: Implementation of grep command

				
					**FEATURES**

	
	1) Searches pattern which is given on command line and prints the line in which it is found.
	2) It searches regular expressions as well as extended expressions.
	3) Generic programme information : --help output usage messeges and exit.
	
			
	Matcher selection:
		1)-G  - this is default.
		2)-E  - Interpreate pattern as extended regular expression.
			searches for pattern having '+' and '?' .
		

	Matching Control:
		1)-i  - ignore case distinctions in both pattern and input file.
		2)-v  - invert the sense of matching.
		3)-x  - select only those matches that exactly matches whole line.
		4)-y  - obsolete synonym of -i.
		

	General Output Control:
		1)-c  - counts the no. of lines which are matched and prints only count.	
		2)-L  - print the name of each input file from which no output would have been printed.
		3)-l  - print the name of each input file from which the output normally would have been printed.
		4)-o  - print only the matched parts of matching line.
		5)-q  - do not write anything to standard output.
		6)-m num  - stop reading a file after num matching lines.	
		7)-s  - supress error messeges.


	Output Line Prefix Control:
		1)-H  - print filename for each match.
		2)-n  - prefix each line of output with 1-based line number within its input files.
      
   	
	Context Line Control:
		1)-A NUM - prints num lines of trailing context after matching line.
	

	Directories :
		1)-r  - read all files under given directory and searches pattern.
		2)-d action  - uses action to proceed it. By default action is read.
				action is 'recurse'.
	
	
	Regular Expression:
		1)caret  '^': - matches the empty string at the beginning.
		2)dollar '$': - matches the empty string at the end of line.
		3)square brackets - handles the string as grep does for the same input.
				eg. [abcd],[a-d],etc
		4)dot    '.': - ignores dot for matching.
	
	
	
	
