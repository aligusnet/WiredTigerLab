#include <string>
#include <stdexcept>
#include <memory>
#include <wiredtiger.h>

export module wtapi;

export class DbError : public std::runtime_error {
public:
  explicit DbError(const std::string& message) : std::runtime_error(message) {}
  explicit DbError(const char* message) : std::runtime_error(message) {}
};

inline void check(int result) {
    if (result != 0) {
        throw DbError(wiredtiger_strerror(result));
    }
}

class NonCopyable {
public:
    NonCopyable() = default;

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class Session;
class Cursor;

export class Application : NonCopyable {
public:
    explicit Application(const std::string& dbpath) : _conn{nullptr} {
        check( wiredtiger_open(dbpath.c_str(), nullptr, "create", &_conn) );
    }

    std::unique_ptr<Session> open() {
        WT_SESSION* session;
        check( _conn->open_session(_conn, nullptr, nullptr, &session) );
        return std::make_unique<Session>(session);
    }

    ~Application() {
        if (_conn != nullptr) {
            _conn->close(_conn, nullptr);
        }
    }

private:
    WT_CONNECTION* _conn;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
};

export class Session : NonCopyable {
public:
    explicit Session(WT_SESSION* session) :_session{ session } {}

    void create(const std::string& name, const std::string& config) {
        check( _session->create(_session, name.c_str(), config.c_str()) );
    }

    std::unique_ptr<Cursor> open(const std::string& uri) {
        WT_CURSOR* cursor;
        check( _session->open_cursor(_session, uri.c_str(), nullptr, nullptr, &cursor));
        return std::make_unique<Cursor>(cursor);
    }

    ~Session() {
        _session->close(_session, nullptr);
    }

private:
    WT_SESSION* _session;
};

export class Cursor : NonCopyable {
public:
    explicit Cursor(WT_CURSOR* cursor) : _cursor{ cursor } {}

    template <typename T>
    void setKey(const T& value) {
        _cursor->set_key(_cursor, value);
    }

    template <typename T>
    void setValue(const T& value) {
        _cursor->set_value(_cursor, value);
    }

    template <typename T>
    void getKey(T* key) {
        check(_cursor->get_key(_cursor, key));
    }

    template <typename T>
    void getValue(T* value) {
        check(_cursor->get_value(_cursor, value));
    }

    void insert() {
        check(_cursor->insert(_cursor));
    }

    void reset() {
        check( _cursor->reset(_cursor) );
    }

    bool next() {
        return _cursor->next(_cursor) == 0;
    }

    ~Cursor() {
        _cursor->close(_cursor);
    }

private:
    WT_CURSOR* _cursor;
};