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
'''
standard input&nbsp; 0
standard output 1
standard error&nbsp;	&nbsp;2
'''

