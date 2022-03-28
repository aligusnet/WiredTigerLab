#include <iostream>
#include <vector>
#include <string>

import wtapi;
import bson;

void test(Application& app) {
    auto session = app.open();

    session->create("table:autoinc", "key_format=r,value_format=u");
    auto cursor = session->open("table:autoinc", "append");

    std::vector<std::string> records{ "{\"hello\": \"world\"}", "{\"integer\": 100}", "{\"double\": 111.11}" };
    for (auto&& rec : records) {
        auto bson = BsonObject::from_json(rec);
        cursor->setBsonValue(*bson);
        cursor->insert();
    }

    cursor->reset();

    while (cursor->next()) {
        uint64_t key;
        cursor->getKey(&key);
        auto value = cursor->getBsonValue();
        std::cout << "Got record: " << key << " : " << value->toJson() << std::endl;
    }
}

int main(int argc, char** argv) {
    Application app{ "C:/tmp" };
    try {
        test(app);
    }
    catch (const std::runtime_error& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
