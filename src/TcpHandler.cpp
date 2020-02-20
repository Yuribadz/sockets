#include "TcpHandler.hpp"

uv_loop_t TcpHandler::_loop = {};
struct sockaddr_in TcpHandler::_addr = {};

int TcpHandler::stop() {
  uv_stop(&_loop);
  return 0;
}

int TcpHandler::run() {
  _loop = *uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(&_loop, &server);

  uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &_addr);

  uv_tcp_bind(&server, (const struct sockaddr*) &_addr, 0);
  int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG,
                    TcpHandler::on_new_connection);
  if (r) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    return 1;
  }
  return uv_run(&_loop, UV_RUN_DEFAULT);
}
void TcpHandler::free_write_req(uv_write_t *req) {
  write_req_t *wr = reinterpret_cast<write_req_t*>(req);
  free(wr->buf.base);
  free(wr);
}

void TcpHandler::alloc_buffer(uv_handle_t *handle, size_t suggested_size,
                              uv_buf_t *buf) {
  buf->base = (char*) malloc(suggested_size);
  buf->len = suggested_size;
}

void TcpHandler::echo_write(uv_write_t *req, int status) {
  if (status) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }
  free_write_req(req);
}

void TcpHandler::echo_read(uv_stream_t *client, ssize_t nread,
                           const uv_buf_t *buf) {
  if (nread > 0) {
    write_req_t *req = reinterpret_cast<write_req_t*>(malloc(
        sizeof(write_req_t)));
    req->buf = uv_buf_init(buf->base, nread);
    uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
    return;
  }
  if (nread < 0) {
    if (nread != UV_EOF)
      std::cout << "Read error " << uv_err_name(nread) << "\n";
    uv_close((uv_handle_t*) client, NULL);
  }

  free(buf->base);
}

void TcpHandler::on_new_connection(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    // error!
    return;
  }

  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(&_loop, client);
  if (uv_accept(server, (uv_stream_t*) client) == 0) {
    uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
  } else {
    uv_close((uv_handle_t*) client, NULL);
  }
}
