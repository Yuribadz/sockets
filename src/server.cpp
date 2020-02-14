#include <iostream>
#include <string>
#include <thread>
#include "ThreadPool.hpp"
#include <deque>
#include <vector>
#include <utility>
#include <assert.h>
#include <uv.h>

namespace {
uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;
uv_fs_t close_req;
uv_buf_t iov;

char buf[0xFFFF];
void on_write(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_open(uv_fs_t *req);

void on_write(uv_fs_t *req) {
  if (req->result < 0) {
    std::cout << "Write error : " << uv_strerror(static_cast<int>(req->result));
  } else {
    std::cout << "\nON_WRITE LOOP\n";
    uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1,
               &on_read);
  }
}
void on_read(uv_fs_t *req) {
  if (req->result < 0) {
    std::cout << "Read error : " << uv_strerror(static_cast<int>(req->result));
  } else if (req->result == 0) {
    std::cout << "ON_READ RESULT 0\n";
    uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
  } else if (req->result > 0) {
    std::cout << "ON_READ LOOP\n";
    iov.len = req->result;
    uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
  }
}

void on_open(uv_fs_t *req) {
  std::cout << "ASSERT ON OPEN\n";
  assert(req == &open_req);
  if (req->result >= 0) {
    iov = uv_buf_init(buf, sizeof(buf));
    // OFFSET -1 MEANS CURRENT FILE POSITION. RETARDED
    std::cout << "BUFFER INIT\n";
    uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
  } else {
    std::cout << "Error opening file : "
              << uv_strerror(static_cast<int>(req->result));
  }
}
}
int main(int argc, char **argv) {
  uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&read_req);
  uv_fs_req_cleanup(&write_req);
  return 0;
}
