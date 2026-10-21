#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    srand((unsigned)time(NULL));
    setvbuf(stdout, NULL, _IOLBF, 0);

    while (1) {
        double temp = 20.0 + (rand() % 100) / 10.0;      /* 20.0 - 29.9 C */
        double hum  = 40.0 + (rand() % 400) / 10.0;      /* 40.0 - 79.9 % */
        double pres = 1000.0 + (rand() % 300) / 10.0;    /* 1000 - 1029.9 hPa */

        printf("temperature=%.1fC humidity=%.1f%% pressure=%.1fhPa\n",
               temp, hum, pres);
        sleep(5);
    }
    return 0;
}
