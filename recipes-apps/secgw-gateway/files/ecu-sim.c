/* ecu-sim : simule un ECU qui envoie vitesse et temperature toutes les 100 ms */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

static void send_frame(int s, unsigned id, int16_t value)
{
    struct can_frame f = {0};
    f.can_id  = id;
    f.len     = 2;
    f.data[0] = (uint16_t)value >> 8;
    f.data[1] = (uint16_t)value & 0xFF;
    write(s, &f, sizeof(f));
}

int main(void)
{
    struct sockaddr_can addr = {0};
    struct ifreq ifr = {0};
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    int speed = 0, dir = 1, temp = 20;

    if (s < 0) { perror("socket"); return 1; }
    strncpy(ifr.ifr_name, "vcan0", IFNAMSIZ - 1);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) { perror("ioctl"); return 1; }
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }

    for (;;) {
        speed += dir * 2;
        if (speed >= 130) dir = -1;
        if (speed <= 0)   dir = 1;
        if (temp < 90 && speed % 10 == 0) temp++;

        send_frame(s, 0x100, (int16_t)speed);
        send_frame(s, 0x101, (int16_t)temp);
        usleep(100000);
    }
    return 0;
}
