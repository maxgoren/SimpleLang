# Smolz

Smolz is a small (get it?), simple, toy programming language
meant to show the basics of lexing/parsing, AST construction,
and to a lesser extent, interpretation.

The grammar is purposefully small and easily parsed. 
Smolz has variables that are declared by use, with the only
supported data type being reals (single precision floating point numbers).
Smolz also features conditional branching and iteration through the use of
if statements and the 'loop' keyword which is analagous to 'while' loops. 
functions are defined using the 'def' keyword, and can call themselves recursively.

    def fib(m) {
        if (m < 2) {
            return 1;
        }
        return fib(m-1) + fib(m-2);
    }
    i := 1;
    loop (i < 14) {
        print fib(i);
        i := i + 1;
    }

    
