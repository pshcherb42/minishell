#include "../minishell.h"

void    handle_sigint(int sig)
{
    (void)sig; // que es sigint?
    write(1, "\n", 1);
    rl_on_new_line();
    // aqui deberia ir rl_replace_line pero no me aparece 
    rl_redisplay();
}

void    setup_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN); // que es sig_ign?
}