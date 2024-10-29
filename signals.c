#include "minishell.h"

void set_terminal_attributes(void)
{
    struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
		return ;
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
void	signal_handler_int(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 1; // ??
}
// Function to ignore SIGINT (Ctrl+C)
void ignore_sigint(void)
{
    struct sigaction sa_int;

    sa_int.sa_handler = signal_handler_int; // Set handler to ignore SIGINT
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0; // No special flags
    sigaction(SIGINT, &sa_int, NULL);
}

// Function to ignore SIGQUIT (Ctrl+\) 
void ignore_sigquit(void)
{
    struct sigaction sa_quit;

    sa_quit.sa_handler = SIG_IGN; // Set handler to ignore SIGQUIT
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0; // No special flags
    sigaction(SIGQUIT, &sa_quit, NULL);
}

// Function to set terminal attributes


// Set up signal handlers and terminal attributes
void signals(void)
{
    // Ignore SIGINT (Ctrl+C)
    ignore_sigint();
    
    // Ignore SIGQUIT (Ctrl+\)
    ignore_sigquit();
    set_terminal_attributes();
    // Set terminal attributes to suppress echo and signal handling
}

// Example main loop of your shell
// void shell_loop(void)
// {
//     while (1) {
//         // Display prompt
//         printf("minishell> ");
        
//         // Your input reading and processing goes here

//         // For demonstration, let's just read input
//         char input[256];
//         if (fgets(input, sizeof(input), stdin) == NULL) {
//             break; // Handle EOF or error
//         }

//         // Process the input...
//     }
// }