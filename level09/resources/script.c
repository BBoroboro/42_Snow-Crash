# include <stdio.h>
# include <string.h>

int     main(int ac, char **av) {
    (void)ac;
    for(int i = 0; i < strlen(av[1]); i++) {
        printf("%c", (av[1][i] - i));
    }
}