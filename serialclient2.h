/*
    This file is part of Kismet

    Kismet is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Kismet is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Kismet; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __SERIALCLIENT_V2_H__
#define __SERIALCLIENT_V2_H__

#include "config.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "messagebus.h"
#include "globalregistry.h"
#include "buffer_handler.h"
#include "pollable.h"

// New serial client code
//
// This code replaces serialclient and clinetframework with a cleaner serial
// implementation which interacts with a bufferhandler
//
// This doesn't need to implement a ringbuffer interface directly because
// it checks using the poll() sequence.  The consumer will use a rb interface
// for reading incoming data.
class serial_client_v2 : public kis_pollable {
public:
    serial_client_v2(global_registry *in_globalreg, std::shared_ptr<buffer_handler_generic> in_rbhandler);
    virtual ~serial_client_v2();

    virtual void set_mutex(std::shared_ptr<kis_recursive_timed_mutex> in_parent);

    // Open a serial port @ a given baud rate
    int open_device(std::string in_device, unsigned int in_baud);
    void close_device();

    // kis_pollable interface
    virtual int pollable_merge_set(int in_max_fd, fd_set *out_rset, fd_set *out_wset);
    virtual int pollable_poll(fd_set& in_rset, fd_set& in_wset);

    bool get_connected();

protected:
    global_registry *globalreg;

    std::shared_ptr<kis_recursive_timed_mutex> serial_mutex;

    std::shared_ptr<buffer_handler_generic> handler;

    int device_fd;

    std::string device;
    unsigned int baud;
};

#endif
