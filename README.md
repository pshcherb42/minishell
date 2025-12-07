## Minishell
## Our own simple bash.
We did this project with my teammate to deepen our understanding of Unix shell concepts.
It supports executing commands, handling environment variables, and implementing key shell features such as pipes, redirections, and built-in commands.

![shell_image](./resources/shell.jpeg)

Minishell is a simple Unix shell made in C. It allows users to manage processes, run commands, and interact with the system in a straightforward and secure manner. 

It supports key shell features. These include:

🪠Piping between commands

🏞️Handling environment variables

🔣Input/output redirection

🚀Command execution

Minishell is a great tool for students and developers. Doing this project helped us understand how shells work.
This project gave us hands-on experience. We worked with system calls, managed processes, and explored low-level programming concepts.

![Minishell Workflow](./resources/minishell-diagram.git)

## Installation

To use Minishell, you need a Unix-like environment (Linux or macOS) and a C compiler (like `gcc`). Follow these steps:

1. **Clone the repository:**
  ```bash

git clone https://github.com/your-username/minishell.git

cd minishell 
```
2. **Compile the project:**
```bash
make
```
This will create the executable minishell.

## Usage
1.	**Start Minishell:**

  	```bash
    ./minishell
  `
  2.	**Run commands just like in a regular shell:**

```bash
ls
pwd
echo "Hello, Minishell!"
  ```
  4.	**Use pipes and redirections:**
```bash
ls -l | grep ".c"
echo "Hello World" > file.txt
cat < file.txt
  ```
  5.	**Work with environment variables:**
```bash
export MYVAR="Minishell Test"
echo $MYVAR
  ```
  6.	**Exit Minishell:**
```bash
exit
```
