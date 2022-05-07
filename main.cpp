#include "moduls/Json.hpp"
#include "cJSON/cJSON.h"
#include <cstdlib>

int memCounter = 0;
int newCounter = 0;
std::vector<void *> address;

void *operator new(size_t size) {
    newCounter++;

    return malloc(size);
}

void operator delete (void *memory) {
    newCounter--;

    free(memory);
}

void *my_malloc(size_t size) {
    memCounter++;
    void *pointer = malloc(size);

    address.push_back((void *)pointer);
    return pointer;
}

void my_free(void *pointer) {
    if(pointer == nullptr)
        return;

    memCounter--;
    address.erase(std::remove(address.begin(), address.end(), (void *)pointer), address.end());
    free(pointer);
}

void change_allocators() {
    cJSON_Hooks custom_allocators = {my_malloc, my_free};
    cJSON_InitHooks(&custom_allocators);
}


const char text[] = ""
                        "{"
                            "\"my_number\": 1,"
                            "\"my_string\": \"test\","
                            "\"my_double\": 2.1,"
                            "\"my_bool\": true"
                        "}"
                    "";

int main() {
    change_allocators();
    {
        // Init objects
        Json j = text;
        Json h = text;
        Json z = text;

        // Adding a regular field
        j["newNum"] = 15;

        // Adding an array
        h["json"] = {1,2,3};

        // Adding an empty array
        z["array"] = {};
        z.Print();

        // Rewrite to the same key
        h["json"] = 44;

        // Adding advance array
        h["arr"] = {1.1, j,3, "HEY", 4, 8};

        // Copying object into another object
        h["test"] = z;

        // Adding a new number in second level
        h["test"]["sec_level"] = 4.5;

        // Moving object into another object
        h["test1"] = std::move(j);

        // Getting and printing field
        std::optional<double> fs = h["test"]["sec_level"];
        if(fs)
            printf("fs=%lf\n", fs.value());
        else
            printf("no value\n");

        // Printing the whole object
        h.Print();

        // Loops
        Json loop = text;
        for(auto json : loop.GetChildren()) {
            json->Print();
        }

        Json children = loop.GetChildren();
        for(; !children.IsNull(); children.Next()) {
            children.Print();
        }

        // Executing lambda in each node
        loop.LoopThrough([](cJSON *obj){ printf("%s\n", obj->string);});

    }

    printf("Amount of leaked c allocations: %d\n", memCounter);

}