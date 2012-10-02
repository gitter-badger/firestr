/*
 * Copyright (C) 2012  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FIRESTR_NETWORK_MESSAGE_QUEUE_H
#define FIRESTR_NETWORK_MESSAGE_QUEUE_H

#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <map>

#include <boost/lexical_cast.hpp>

#include "util/bytes.hpp"
#include "util/dbc.hpp"

namespace fire 
{
    namespace network 
    {
        struct socket_info
        {
            std::string local_address;
            std::string local_port;
            std::string remote_address;
            std::string remote_port;
        };

        class message_queue
        {
            public:
                bool send(const std::string& s)
                {
                    return send(util::to_bytes(s));
                }

                bool recieve(std::string& s)
                {
                    util::bytes b;
                    if(recieve(b)) 
                    {
                        s = util::to_str(b);
                        return true;
                    }
                    return false;
                }
                
                virtual bool send(const util::bytes&) = 0;
                virtual bool recieve(util::bytes&) = 0;

                virtual socket_info get_socket_info() const = 0;
        };

        typedef std::shared_ptr<message_queue> message_queue_ptr;
        typedef std::map<std::string, std::string> queue_options;

        template<class t>
            t get_opt(const queue_options& o, const std::string& k, t def)
            {
                auto i = o.find(k);
                if(i != o.end()) return boost::lexical_cast<t>(i->second);
                return def;
            }

        message_queue_ptr create_message_queue(
                const std::string& queue_address, 
                const queue_options& default_options = queue_options());

        enum message_type {zeromq, bst};
        struct address_components
        {
            std::string queue_address; 
            message_type type;
            std::string location;
            queue_options options;
        };

        address_components parse_address(
                const std::string& queue_address, 
                const queue_options& defaults = queue_options());

        std::string make_zmq_address(const std::string& host, const std::string& port);
        std::string make_bst_address(const std::string& host, const std::string& port);
        std::string make_bst_address(const std::string& host, const std::string& port, const std::string& local_port);
    }
}

#endif
