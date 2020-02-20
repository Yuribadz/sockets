#ifndef TCPHANDLER_HPP
#define TCPHANDLER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <uv.h>

#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128
#define RESULT_SUCCESS 0

class TcpHandler {
  typedef struct {
    uv_write_t req;
    uv_buf_t buf;
  } write_req_t;

 public:
  TcpHandler() {};
  ~TcpHandler(){};
  int run();
  int stop();
 protected:
  static void on_new_connection(uv_stream_t *server, int status);
  static void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
  static void echo_write(uv_write_t *req, int status);
  static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
  static void free_write_req(uv_write_t *req);

 private:
  static uv_loop_t _loop;
  static struct sockaddr_in _addr;
  TcpHandler(const TcpHandler&) = delete;
  TcpHandler(TcpHandler&) = delete;
  TcpHandler& operator=(const TcpHandler&) = delete;
};

#endif

