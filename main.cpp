#include <iostream>
#include "moduls/Json.hpp"
#include "cJSON/cJSON.h"
#include <optional>
#include <cstdint>
#include <variant>

void Print(cJSON* object);

std::variant<bool, int, double> var;

template<class TYPE>
TYPE Get(cJSON *obj) {
    if constexpr(std::is_same_v<TYPE, bool>){
        if(cJSON_IsBool(obj)){
            return cJSON_IsTrue(obj);
        }
    }

    if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
        if(cJSON_IsNumber(obj)){
            return cJSON_GetNumberValue(obj);
        }
    }

    if constexpr (std::is_pointer_v<TYPE>) {
        if(cJSON_IsString(obj)){
            return cJSON_GetStringValue(obj);
        }
    }
//    static_assert(0);
}



int main() {

//    char text[] = "{\"attributes\":{\"system\":{\"name\":\"RSMAT-753428055\",\"hw_type\":\"reef-mat\",\"hw_model\":\"RSMAT\",\"free_heap\":41124},\"firmware\":{\"version\":\"1.4.1\"},\"networking\":{\"wifi_ip\":\"192.168.137.6\",\"mac\":\"C4:DD:57:66:E8:2C\",\"network\":\"9DKBN132395\"}},\"specific_attributes\":{\"mode\":\"no_ec_sensor\",\"is_advancing\":false,\"last_advance_cause\":\"button\",\"roll_level\":\"full\",\"days_till_end_of_roll\":0,\"today_usage\":39.942001342773438,\"daily_average_usage\":0,\"total_usage\":39.778301239013672,\"internal_ec_average\":0,\"external_ec_average\":0,\"setup_date\":\"2022-01-11T12:48:26Z\",\"remaining_length\":2443.30517578125,\"cumulative_steps\":105,\"device_setup_date\":1641905307,\"material\":{\"name\":\"28Meter\",\"external_diameter\":10.899999618530273,\"thickness\":0.032,\"is_partial\":false}},\"arr\":[{\"one\":\"1\"},{\"two\":\"2\",\"doubleTwo\":\"22\"},{\"three\":\"3\"},{\"four\":\"4\"}]}";
    char text[] = ""
                    "{"
                        "\"number\": 1,"
                        "\"string\": \"test\","
                        "\"double\": 2.1,"
                        "\"bool\": true"
                    "}"
                  "";


    cJSON* obj = cJSON_Parse(text);

    Json j = Json(obj);

    j.SetNewObject("newNum", false);


    j.Print();


}


void Print(cJSON* object) {
    char* buffer;

    buffer = cJSON_Print(object);

    if (buffer == nullptr) {
        printf("Failed to print json\n");
        return;
    }

    printf("%s\n", buffer);
    cJSON_free(buffer);
}