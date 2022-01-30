#include <wiredtiger.h>

#include <iostream>

int main(int argc, char **argv) {
  int ret;
  WT_CONNECTION *conn;
  if (ret = wiredtiger_open("C:/tmp", nullptr, "create", &conn), ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  WT_SESSION *session;
  if (ret = conn->open_session(conn, nullptr, nullptr, &session), ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  if (ret = session->create(session, "table:access",
                            "key_format=S,value_format=S"),
      ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  WT_CURSOR *cursor;
  if (ret = session->open_cursor(session, "table:access", nullptr, nullptr,
                                 &cursor),
      ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  cursor->set_key(cursor, "key1");
  cursor->set_value(cursor, "value1");
  if (ret = cursor->insert(cursor), ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  if (ret = cursor->reset(cursor), ret != 0) {
    std::cout << wiredtiger_strerror(ret) << std::endl;
    return -1;
  }

  while (ret = cursor->next(cursor), ret == 0) {
    const char *key, *value;
    cursor->get_key(cursor, &key);     // check
    cursor->get_value(cursor, &value); // check
    std::cout << "Got record: " << key << " : " << value << std::endl;
  }

  conn->close(conn, nullptr);

  return 0;
}
