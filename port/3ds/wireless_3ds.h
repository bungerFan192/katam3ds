#ifndef KATAM3DS_WIRELESS_3DS_H
#define KATAM3DS_WIRELESS_3DS_H
#include <stddef.h>
#include <stdint.h>

#define K3D_MP_MAX_PLAYERS 4
#define K3D_MP_MAX_PACKET  256

typedef struct {
    uint8_t connected;
    uint8_t player_id;
    uint8_t player_count;
} K3D_MPStatus;

/* The transport boundary deliberately uses packets, not GBA register
   addresses. A separate SIO adapter will translate MultiSio words to this
   queue. */
int K3D_MP_Init(void);
void K3D_MP_Exit(void);
int K3D_MP_Host(uint8_t max_players);
int K3D_MP_Join(const char *passphrase);
void K3D_MP_Disconnect(void);
void K3D_MP_Poll(K3D_MPStatus *status);
int K3D_MP_Send(uint8_t player_id, const void *packet, size_t size);
int K3D_MP_Receive(uint8_t *sender, void *packet, size_t capacity, size_t *size);
#endif
