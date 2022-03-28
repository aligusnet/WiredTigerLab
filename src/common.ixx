module;

#include <stdexcept>

export module common;

export class DbError : public std::runtime_error {
public:
    explicit DbError(const std::string& message) : std::runtime_error(message) {}
    explicit DbError(const char* message) : std::runtime_error(message) {}
};

export class NonCopyable {
public:
    NonCopyable() = default;

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
