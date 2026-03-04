#include <unistd.h>


void    ggg()
{
        write(1, "abcdefghijklmnopqrstuvwxyz", 26);
}

int     main()
{
        ggg();
        return 0;
}