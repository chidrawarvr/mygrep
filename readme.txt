Title of mini project: Implementation of grep command



Features:
	1) Searches pattern which is given on command line and prints the line in which it is found.
	2) It searches regular expressions as well as extended expressions.
	3) It has --help option which prints usage message.
	4) -G  - this is default.
	5) -i  - ignore cases.
	6) -v  - invert the sense of matching.
	7) -w  - matches the line that form whole word.
	8) -c  - counts the no. of lines which are matched.
	9) -H  - print filename for each match.
        10)-h  - supress filename.
	11)-x  - select only those matches that exactly matches whole line.
	12)-y  - obsolete synonym of -i.
	13)-L  - print the name of each input file from which no output would have been printed.
	14)-l  - print the name of each input file from which the output normally would have been printed.
	15)-o  - print only the matched parts of matching line.
	16)-q  - do not write anything to standard output.
	17)-m num  - stop reading a file after num matching lines.
	18)-n  - prefix each line of output with 1-based line number within its input files.
	19)-r  - read all files under given directory and searches pattern.
	20)-d action  - uses action to proceed it. By default action is read.
	21)-R  - read all files under directory unlike -r.
	22)-A num  - prints num lines of trailing context after matching lines.
	23)-B num  - prints num lines of leading context before matching lines.
	24)-C num  - prints num lines of trailing and leading context after and before matching lines.
	25)caret - matches the empty string at the beginning.
	26)dollar- matches the empty string at the end of line.
	27)square brackets - handles the string as grep does for the same input.
	28)Handles all basic expressions and backslash characters.
	29)bracket expressions like [:alnum:],[:digit:],etc.
	30)-s  - no messages.
	31)-E  - pattern as extended regular expressions.
	31)-f file - obtain pattern from file as one per line.
	32)-T  - initial tab.
