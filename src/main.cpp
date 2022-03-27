#include <iostream>

import wtapi;

void test(Application& app) {
    auto session = app.open();

    session->create("table:access", "key_format=S,value_format=S");

    auto cursor = session->open("table:access");
    cursor->setKey("key1");
    cursor->setValue("value1");
    cursor->insert();

    cursor->reset();

    while (cursor->next()) {
        const char* key, * value;
        cursor->getKey(&key);
        cursor->getValue(&value);
        std::cout << "Got record: " << key << " : " << value << std::endl;
    }
}

int main(int argc, char **argv) {
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
