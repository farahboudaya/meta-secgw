/* secgw-gateway : surveillance d'un bus CAN (SocketCAN) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define IFACE              "vcan0"
#define ID_SPEED           0x100
#define ID_TEMP            0x101
#define SPEED_MAX          250
#define TEMP_MIN           (-40)
#define TEMP_MAX           150
#define MAX_FRAMES_PER_SEC 30

struct rate {
    long window_start;
    int  count;
    int  alerted;
};

static long now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

/* Retourne 1 si le seuil de frequence est depasse (alerte une fois par fenetre) */
static int rate_exceeded(struct rate *r, unsigned id)
{
    long t = now_ms();

    if (t - r->window_start >= 1000) {
        r->window_start = t;
        r->count = 0;
        r->alerted = 0;
    }
    r->count++;
    if (r->count > MAX_FRAMES_PER_SEC) {
        if (!r->alerted) {
            printf("ALERT flood id=0x%03X (> %d trames/s)\n",
                   id, MAX_FRAMES_PER_SEC);
            r->alerted = 1;
        }
        return 1;
    }
    return 0;
}

int main(void)
{
    struct rate rates[3] = {{0}};   /* 0: vitesse, 1: temperature, 2: inconnus */
    struct sockaddr_can addr = {0};
    struct ifreq ifr = {0};
    struct can_frame f;
    int s;

    setvbuf(stdout, NULL, _IOLBF, 0);

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) { perror("socket"); return 1; }

    strncpy(ifr.ifr_name, IFACE, IFNAMSIZ - 1);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) { perror("ioctl " IFACE); return 1; }

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("secgw-gateway demarree, ecoute sur %s\n", IFACE);

    for (;;) {
        ssize_t n = read(s, &f, sizeof(f));
        unsigned id;

        if (n < (ssize_t)sizeof(f))
            continue;
        if (f.can_id & (CAN_ERR_FLAG | CAN_RTR_FLAG))
            continue;

        id = f.can_id & CAN_SFF_MASK;

        if (id == ID_SPEED) {
            if (rate_exceeded(&rates[0], id))
                continue;
            if (f.len != 2) {
                printf("ALERT longueur invalide id=0x%03X len=%d\n", id, f.len);
                continue;
            }
            unsigned v = (f.data[0] << 8) | f.data[1];
            if (v > SPEED_MAX)
                printf("ALERT valeur impossible vitesse=%u km/h\n", v);
            else
                printf("OK vitesse=%u km/h\n", v);

        } else if (id == ID_TEMP) {
            if (rate_exceeded(&rates[1], id))
                continue;
            if (f.len != 2) {
                printf("ALERT longueur invalide id=0x%03X len=%d\n", id, f.len);
                continue;
            }
            int t = (int16_t)((f.data[0] << 8) | f.data[1]);
            if (t < TEMP_MIN || t > TEMP_MAX)
                printf("ALERT valeur impossible temperature=%d C\n", t);
            else
                printf("OK temperature=%d C\n", t);

        } else {
            if (rate_exceeded(&rates[2], id))
                continue;
            printf("ALERT identifiant inconnu id=0x%03X\n", id);
        }
    }
    return 0;
}
