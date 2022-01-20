#include "moduls/Json.hpp"
#include "cJSON/cJSON.h"
#include <variant>

//    char text[] = "{\"attributes\":{\"system\":{\"name\":\"RSMAT-753428055\",\"hw_type\":\"reef-mat\",\"hw_model\":\"RSMAT\",\"free_heap\":41124},\"firmware\":{\"version\":\"1.4.1\"},\"networking\":{\"wifi_ip\":\"192.168.137.6\",\"mac\":\"C4:DD:57:66:E8:2C\",\"network\":\"9DKBN132395\"}},\"specific_attributes\":{\"mode\":\"no_ec_sensor\",\"is_advancing\":false,\"last_advance_cause\":\"button\",\"roll_level\":\"full\",\"days_till_end_of_roll\":0,\"today_usage\":39.942001342773438,\"daily_average_usage\":0,\"total_usage\":39.778301239013672,\"internal_ec_average\":0,\"external_ec_average\":0,\"setup_date\":\"2022-01-11T12:48:26Z\",\"remaining_length\":2443.30517578125,\"cumulative_steps\":105,\"device_setup_date\":1641905307,\"material\":{\"name\":\"28Meter\",\"external_diameter\":10.899999618530273,\"thickness\":0.032,\"is_partial\":false}},\"arr\":[{\"one\":\"1\"},{\"two\":\"2\",\"doubleTwo\":\"22\"},{\"three\":\"3\"},{\"four\":\"4\"}]}";
const char text[] = ""
              "{"
              "\"my_number\": 1,"
              "\"my_string\": \"test\","
              "\"my_double\": 2.1,"
              "\"my_bool\": true"
              "}"
              "";

int main() {
    cJSON* obj = cJSON_Parse(text);
    cJSON* obj2 = cJSON_Parse(text);
//    Json j = Json(obj);
    Json h = Json(obj2);

    h["json"] = (Json(1));


//    h["arr"] = Json({1.1,j,3,"HEY",4,8});

//    h["arr"] = Json("HEY");

//    j.AddNewObject("newNum", false);

//    j["newNum"] = 15;
//    std::optional<double> fs = j["my_double"];

//    j["test"] = h;

//    if(fs) {
//        printf("fs=%lf\n", fs.value());
//    } else {
//        printf("no value\n");
//    }

    h.Print();

}