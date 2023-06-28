#!/bin/bash

grep -e '^hello' -e bye << BYE | sed "s/you/they/g" >> out/expected22
hello
this
is
a
test
...

it was a empty line
...
hello are you still here ?
	
there's a space on the line before
...

this time I tried with a tab
...
i think we're good
hello ? you already left
well
bye then
hmmmm 
more like :
Bye!
BYE

cat << STDIN | $1 here_doc BYE "grep -e '^hello' -e bye" "sed \"s/you/they/g\"" out/actual22
hello
this
is
a
test
...

it was a empty line
...
hello are you still here ?
	
there's a space on the line before
...

this time I tried with a tab
...
i think we're good
hello ? you already left
well
bye then
hmmmm 
more like :
Bye!
BYE
STDIN

exit $?
