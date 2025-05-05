#include <network.hpp>

onReceive(void *context)
{
    ((NETWORK *)context)->packetReceived = true;
}
