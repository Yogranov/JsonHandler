#include "moduls/Json.hpp"
#include "cJSON/cJSON.h"
#include <cstdlib>

void *operator new(size_t size) {
    return malloc(size);
}

const char text[] = ""
                        "{"
                            "\"my_number\": 1,"
                            "\"my_string\": \"test\","
                            "\"my_double\": 2.1,"
                            "\"my_bool\": true"
                        "}"
                    "";

int memCounter = 0;

void *my_malloc(size_t size) {
    memCounter++;
    void *pointer = malloc(size);
    return pointer;
}

void my_free(void *pointer) {
    if(pointer == nullptr)
        return;

    memCounter--;
    free(pointer);
}

void change_allocators() {
    cJSON_Hooks custom_allocators = {my_malloc, my_free};
    cJSON_InitHooks(&custom_allocators);
}

int main() {
    change_allocators();
    {
        // Init objects
        Json j = Json(cJSON_Parse(text));
        Json h = Json(cJSON_Parse(text));
        Json z = Json(cJSON_Parse(text));

        // Adding regular field
        j["newNum"] = 15;

        // Adding array
        h["json"] << Json({1,2,3});

        // Rewrite on same key
        h["json"] = 44;

        // Adding advance array
        h["arr"] << Json({1.1, &j,3, "HEY", 4, 8});

        // Inserting object into another object
        h["test"] += z;

        // Inserting object into another object
        h["test1"] << z;

        // Getting and printing field
        std::optional<double> fs = h["my_double"];
        if(fs) {
            printf("fs=%lf\n", fs.value());
        } else {
            printf("no value\n");
        }

        // Printing the whole object
        h.Print();
    }

    printf("\nAmount of leaked allocations: %d\n", memCounter);

}