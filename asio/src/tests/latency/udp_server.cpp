//
// udp_server.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <asio/io_service.hpp>
#include <asio/ip/udp.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstdlib>
#include <vector>

using asio::ip::udp;

#include "yield.hpp"

class udp_server : coroutine
{
public:
  udp_server(asio::io_service& io_service,
      unsigned short port, std::size_t bufsize) :
    socket_(io_service, udp::endpoint(udp::v4(), port)),
    buffer_(bufsize)
  {
  }

  void operator()(asio::error_code ec, std::size_t n = 0)
  {
    reenter (this) for (;;)
    {
      yield socket_.async_receive_from(
          asio::buffer(buffer_),
          sender_, ref(this));

      if (!ec)
      {
        for (std::size_t i = 0; i < n; ++i) buffer_[i] = ~buffer_[i];
        socket_.send_to(asio::buffer(buffer_, n), sender_, 0, ec);
      }
    }
  }

  struct ref
  {
    explicit ref(udp_server* p) : p_(p) {}
    void operator()(asio::error_code ec, std::size_t n = 0) { (*p_)(ec, n); }
    private: udp_server* p_;
  };

private:
  udp::socket socket_;
  std::vector<unsigned char> buffer_;
  udp::endpoint sender_;
};

#include "unyield.hpp"

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::fprintf(stderr, "Usage: udp_server <port1> <nports> <bufsize>\n");
    return 1;
  }

  asio::io_service io_service;
  std::vector<boost::shared_ptr<udp_server> > servers;

  unsigned short first_port = static_cast<unsigned short>(std::atoi(argv[1]));
  unsigned short num_ports = static_cast<unsigned short>(std::atoi(argv[3]));
  std::size_t bufsize = std::atoi(argv[3]);
  for (unsigned short i = 0; i < num_ports; ++i)
  {
    unsigned short port = first_port + i;
    boost::shared_ptr<udp_server> s(new udp_server(io_service, port, bufsize));
    servers.push_back(s);
    (*s)(asio::error_code());
  }

  for (;;) io_service.poll();
}
