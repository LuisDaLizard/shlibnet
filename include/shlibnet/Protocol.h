//
// Created by Luis Tadeo Sanchez on 3/30/23.
//

#ifndef SHLIBNET_PROTOCOL_H
#define SHLIBNET_PROTOCOL_H

#include <netinet/in.h>

namespace shlib
{
    /**
     * Represents an internet protocol. Possible values are NONE, TCP, and UDP.
     */
    enum Protocol
    {
        NONE = 0,
        TCP,
        UDP,
    };
}

#endif //SHLIBNET_PROTOCOL_H
