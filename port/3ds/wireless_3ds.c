#include "wireless_3ds.h"

/*
 * TODO(uds): implement with libctru UDS.
 *
 * This file is intentionally a truthful unavailable backend for now. The
 * upstream transport contract is synchronous at the MultiSio exchange seam,
 * while UDS is packet-oriented and asynchronous. The adapter must buffer
 * frame packets and preserve the 16 transfer slots/frame before it can be
 * considered compatible. Returning failure prevents a fake local session.
 */
int K3D_MP_Init(void) { return 0; }
void K3D_MP_Exit(void) {}
int K3D_MP_Host(uint8_t max_players) { (void)max_players; return 0; }
int K3D_MP_Join(const char *passphrase) { (void)passphrase; return 0; }
void K3D_MP_Disconnect(void) {}
void K3D_MP_Poll(K3D_MPStatus *status)
{
    if (status) {
        status->connected = 0;
        status->player_id = 0;
        status->player_count = 1;
    }
}
int K3D_MP_Send(uint8_t player_id, const void *packet, size_t size)
{
    (void)player_id; (void)packet; (void)size; return 0;
}
int K3D_MP_Receive(uint8_t *sender, void *packet, size_t capacity, size_t *size)
{
    (void)sender; (void)packet; (void)capacity;
    if (size) *size = 0;
    return 0;
}
