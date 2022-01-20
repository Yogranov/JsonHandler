//
// Created by Yogi on 14/01/2022.
//

#ifndef JSONCLASS_JSON_HPP
#define JSONCLASS_JSON_HPP

#include <optional>
#include <cstdio>
#include <variant>
#include <array>
#include "../cJSON/cJSON.h"
#include "../cJSON/cJSON.h"

class Json {

    using json_var_t =  std::variant<bool, const char *, int, long,long long, float, double>;

    class JsonField {
    public:
        JsonField(const char *field, cJSON *json) : m_field(field), m_json(json){};

        template<class T>
        operator std::optional<T>() {
            return Get<T>(cJSON_GetObjectItem(m_json, m_field));
        }

        template<class T>
        void operator = (T t) {
            Set(m_field, t, m_json);
        }

    private:
        const char* m_field;
        cJSON *m_json;
    };


    //constructors
    public:
    Json() : m_json(cJSON_CreateObject()){};
    Json(const Json &other) : m_json(cJSON_Duplicate(other.m_json, true)){};
    Json(Json &&other) : m_json(other.m_json){other.m_json = cJSON_CreateObject();};
    Json(cJSON *json) : m_json(json){};
    Json(char* json) : m_json(cJSON_Parse(json)) {};

    template<class T>
    Json(T type) {
        if constexpr(std::is_same_v<T, bool>){
            m_json = cJSON_CreateBool( type);
        }

        else if constexpr(std::is_integral_v<T> || std::is_same_v<T, float> || std::is_same_v<T, double>) {
            m_json = cJSON_CreateNumber(type);
        }

        else if constexpr (std::is_pointer_v<T>) {
            m_json = cJSON_CreateString(type);
        }

        else if constexpr(std::is_same_v<T, Json>){
            cJSON *newObj = cJSON_Duplicate(type.GetcJSON(), true);
            cJSON_AddItemToObject(m_json, NULL, newObj);
        }
    };

    Json(std::initializer_list<Json> list) : m_json(cJSON_CreateArray()){
        for(Json item : list) {
            cJSON_AddItemToArray(m_json, cJSON_Duplicate(item.GetcJSON(), true));
        }
    };

    ~Json() {
        cJSON_Delete(m_json);
    }



    ////////// Operators  //////////
    Json &operator = (const Json &other) {
        cJSON_Delete(m_json);
        m_json = cJSON_Duplicate(other.m_json, true);

        return *this;
    }

    // Set
    JsonField operator[] (const char *key) {
        return JsonField(key, m_json);
    }

    private:
    // members
    cJSON *m_json;

    //functions
    public:

    /// static functions
    template<class TYPE>
    static std::optional<TYPE> Get(cJSON *json) {
        if constexpr(std::is_same_v<TYPE, bool>){
            if(cJSON_IsBool(json)){
                return cJSON_IsTrue(json);
            }
        }

        else if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
            if(cJSON_IsNumber(json)){
                double tmp = cJSON_GetNumberValue(json);
                return tmp;
            }
        }

        else if constexpr (std::is_pointer_v<TYPE>) {
            if(cJSON_IsString(json)){
                return cJSON_GetStringValue(json);
            }
        }

        return {};
    }

    template<class TYPE>
    static void Set(const char* key, TYPE value, cJSON *json) {
        if constexpr(std::is_same_v<TYPE, bool>){
            cJSON_AddBoolToObject(json, key, value);
        }

        else if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
            cJSON_AddNumberToObject(json, key, value);
        }

        else if constexpr (std::is_pointer_v<TYPE>) {
            cJSON_AddStringToObject(json, key, value);
        }

        else if constexpr(std::is_same_v<TYPE, Json>){
            cJSON *newObj = cJSON_Duplicate(value.GetcJSON(), true);
            cJSON_AddItemToObject(json, key, newObj);
        }
    }


    ////// functions /////

    cJSON *GetcJSON() {
        return m_json;
    }

    bool HaveChildren() {
        return m_json->child == nullptr;
    }

    bool IsNull() {
        return m_json == nullptr;
    }

    bool IsField() {
        return m_json->valuedouble == 0 || m_json->valueint == 0 || m_json->valuestring == nullptr;
    }

    bool IsValid() {
        return m_json->type != 0;
    }

    bool IsNumber() {
        return m_json->type == (1 << 3);
    }

    bool IsString() {
        return m_json->type == (1 << 4);
    }

    bool IsArray() {
        return m_json->type == (1 << 5);
    }

    bool IsObject() {
        return m_json->type == (1 << 6);
    }

    bool IsRaw() {
        return m_json->type == (1 << 7);
    }

    void Print() {
        char * buffer{};

        buffer = cJSON_Print(m_json);
        printf("%s", buffer);
        cJSON_free(buffer);
    }
};



#endif //JSONCLASS_JSON_HPP
