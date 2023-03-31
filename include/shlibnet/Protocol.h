//
// Created by Luis Tadeo Sanchez on 3/30/23.
//

#ifndef SHLIBNET_PROTOCOL_H
#define SHLIBNET_PROTOCOL_H

#include <netinet/in.h>

namespace shlib
{
    enum Protocol
    {
        NONE = -1,
        TCP = IPPROTO_TCP,
        UDP = IPPROTO_UDP,
    };
}

#endif //SHLIBNET_PROTOCOL_H
