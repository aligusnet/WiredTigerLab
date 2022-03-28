module;

#include <bson/bson.h>
#include <memory>
#include <string>

export module bson;

import common;

export class BsonObject : NonCopyable {
public:
    static std::unique_ptr<BsonObject> from_json(const std::string& json);

    explicit BsonObject(bson_t* bson) : _bson{ bson } {}

    BsonObject(const uint8_t* data, size_t size) {
        _bson = bson_new_from_data(data, size);
    }

    const bson_t* get() const { return _bson; }

    const uint8_t* data() const { return bson_get_data(_bson); }

    uint32_t size() const { return _bson->len; }

    std::string toJson() const {
        const auto& buffer = bson_as_json(_bson, nullptr);
        std::string result{ buffer };  // TODO: exception safety?
        bson_free(buffer);
        return result;
    }

    ~BsonObject() { destroy(); }

private:
    void destroy() noexcept {
        if (_bson != nullptr) {
            bson_destroy(_bson);
            _bson = nullptr;
        }
    }

    bson_t* _bson;
};

std::unique_ptr<BsonObject> BsonObject::from_json(const std::string& json) {
    bson_error_t error;
    bson_t* bson =
        bson_new_from_json(reinterpret_cast<const uint8_t*>(json.c_str()),
            static_cast<ssize_t>(json.size()), &error);
    if (bson == nullptr) {
        throw DbError(std::string{ error.message });
    }

    return std::make_unique<BsonObject>(bson);
}
