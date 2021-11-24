# miniSHELL
## Our own shell with @magueija

<br> .Our task was to replicate into certain limits the behaviour of bash:
- Show a prompt when waiting for a new command,
- have a working history,
- search and launch the right executable (based on the PATH variable or by using relative or absolute path),
- implement builtins: echo -n, cd, pwd, export, unset, env, exit,
- interpret behaviour of single ' and double " quotes, including behaviour of $ sing followed by environmental variable,
- redirections: < (redirect input), > (redirect output), << (heredoc), >> (redirect output with append mode),
- pipes | The output of each command in the pipeline is connected via a pipe to the input of the next command,
- environment variables ($ followed by characters) should expand to their values,
- $? should expand to the exit status of the most recently executed foreground pipeline,
- ctrl-C ctrl-D ctrl-\ should work like in bash

## How works shell?
<br> 1. Parse the input
<br> 2. Execute commands in order
<br> Sounds like there is not that much to do, however the project took over a month to be completed.

## Our soluion and challenges.
### PARSING
<br> This project had shown that sayig: "think twice code once" is true and elevated forward thinking ability is in price. Our 1st approach towars parsing was to save all input in two dimensional array. The moment we moved to pipes we realized that this will make our lifes very hard, as execve expects:
> int execve(const char *pathname, char **const argv, char **const envp);
> 1. command, 2. command and what follows untill pipe or redirection is encountered, 3. environmental variables

At this point we understood that better approach will be to create linked list, that contains all arguments untill redirection/pipe is found, will save type of redirection, we could also save output from access function (so we know if command is valid) as well as its path ex: 'ls' /bin/ls. Anytime redirection/pipe is found we create next node and link it. At this point executing it starts being pleasent.

>typedef struct s_arguments
<br>{
<br>&nbsp;&nbsp;	  char				**args;
<br>&nbsp;&nbsp;	  char				pipe_type[4];
<br>&nbsp;&nbsp;	  char				*cmd_w_path;
<br>&nbsp;&nbsp;	  bool				is_valid;
<br>&nbsp;&nbsp;	  bool				special;
<br>&nbsp;&nbsp;	  struct s_arguments	*next;
<br>}&nbsp;&nbsp;&nbsp;&nbsp;t_arguments;

### PIPING
<br> Every process (this is fancy name for a program) has its own redirection table, that looks this way:
```
standard input (stdin)    0
standard output (stdout)  1
standard error (stderr)   2
```
What do we know from it? Whenever you type anything in your command line, aka bash, zhs, whatever, your machine knows that input comes from file descriptor (fd) 0 (it is your keyboard) and it outputs to 1 that is the monitor. Nr 2 is working similar to 1 but it is used to show errors (later on when you redirect outputs with dup or dup2 and change your stdout to pipe writing end you may notice that your debugging printf is not working, the reason is that it actually works but put everything to pipe and you cannot see it, so you could use fprintf and try to print the message in standard error instead to see what is going on. But we will get there hopefully together :)).

Right now when we open new file with `open` process will create new fd with the lowest awaliable number, in this case 3. So when we open pipe with function `pipe(int_array_of_size_2_to_hold_2_numbers_aka_fd's)` we will add 2 new fd to our table:
```
standard input (stdin)    0
standard output (stdout)  1
standard error (stderr)   2
fd_pipe[0] - read part    3
fd_pipe[1] - write part   4
```
IMPORTANT!
Please train your brain: read FROM, write TO. So we write TO the pipe and read FROM the pipe. It was very confusing to me, maybe only to me, as my brain would rather write from and read to but anyway. It already sat in.

IMPORTANT2!
After you redirect output to the pipe (see example below) you have to close writing part of the pipe, otherwise you will get infinite loop (your pipe will be constantly waiting for the input, so by closing we are telling it that it has already got everything we wanted). Then YOU DO NOT CLOSE reading part of the pipe. Why? Oh man... (just kidding xd) You must leave it open so you in the next step (grep microshell) you can read from the pipe what you put there previously. 

IMPORTANT3!
Open pipe and close its fds in PARENT PROCESS! Anything that happens in child process dissapears after its end. 

LET US CONSIDER THIS INPUT:
> ls -l | grep minishell | wc -l

Pipe is simply sort of buffer: 
> ls -l -> goes to the pipe -> | output of ls -l is storred here instead of being displayed on your screen (a)| -> read from pipe, execute grep -> goes to the new pipe (b)| -> ls -l grep output is stored here | -> read from the pipe, execute wc -l -> write it to stdout. 

So this should give you a number as a final output.
(a) -> before going to grep, in your parent process you should close fd_pipe[1], as explained in IMPORTANT2! Before forking to execute grep we will open new pipe, so our table will look like this now:
```
standard input (stdin)    0
standard output (stdout)  1
standard error (stderr)   2
&mdash;fd_pipe[0] - read part    3 - closed, so fd 3 is avaliable
fd_pipe[1] - write part   4
fd_pipe[0] - read part    3
```

(b) -> 
