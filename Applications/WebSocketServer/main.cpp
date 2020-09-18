#include <algorithm>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

#include <kissnet/kissnet.hpp>

namespace kn = kissnet;

int main()
{
    char msg[] = "HTTP/1.1 200 OK\r\n"
                 "Content-Length: 5\r\n"
                 "\r\n\r\n"
                 "Hi!";

    kn::port_t port = 13001;

    std::vector<std::thread> threads;
    std::vector<kn::tcp_socket> sockets;

    kn::tcp_socket listen_socket({ "0.0.0.0", port });
    listen_socket.bind();
    listen_socket.listen();

    while (true) {
        std::cout << "Waiting for a client on port " << port << '\n';
        sockets.emplace_back(listen_socket.accept());

        auto& sock = sockets.back();
        sock.set_non_blocking(true);

        //Create thread that will echo bytes received to the client
        threads.emplace_back([&] {
            //Internal loop
            bool continue_receiving = true;
            //Static 1k buffer
            kn::buffer<1024> buff;

            //While connection is alive
            while (continue_receiving) {
                //attept to receive data
                if (auto [size, valid] = sock.recv(buff); valid) {
                    if (valid.value == kn::socket_status::cleanly_disconnected)
                        continue_receiving = false;
                    else
                        sock.send((std::byte*)msg, sizeof(msg));
                }

                //If not valid remote host closed conection
                else {
                    continue_receiving = false;
                }
            }

            //Now that we are outside the loop, erase this socket from the "sokets" list:
            std::cout << "detected disconnect\n";
            if (const auto me_iterator = std::find(sockets.begin(), sockets.end(), std::ref(sock)); me_iterator != sockets.end()) {
                std::cout << "closing socket...\n";
                sockets.erase(me_iterator);
            }
        });

        threads.back().detach();
    }
}