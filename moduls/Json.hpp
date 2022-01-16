//
// Created by Yogi on 14/01/2022.
//

#ifndef JSONCLASS_JSON_HPP
#define JSONCLASS_JSON_HPP

#include <optional>
#include <cstdio>
#include "../cJSON/cJSON.h"
#include "../cJSON/cJSON.h"

template<class TYPE>
void Set(const char* key, TYPE value, cJSON *json) {
    if constexpr(std::is_same_v<TYPE, bool>){
        cJSON_AddBoolToObject(json, key, value);
    }

    else if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
        cJSON_AddNumberToObject(json, key, value);
    }

    else if constexpr (std::is_pointer_v<TYPE>) {
        cJSON_AddStringToObject(json, key, value);
    }
}

template<class TYPE>
std::optional<TYPE> Get(cJSON *json) {
    if constexpr(std::is_same_v<TYPE, bool>){
        if(cJSON_IsBool(json)){
            return cJSON_IsTrue(json);
        }
    }

    else if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
        if(cJSON_IsNumber(json)){
            return cJSON_GetNumberValue(json);
        }
    }

    else if constexpr (std::is_pointer_v<TYPE>) {
        if(cJSON_IsString(json)){
            return cJSON_GetStringValue(json);
        }
    }

    return {};
}


class Json {
    //constructors
    public:
    Json() : m_json(cJSON_CreateObject()){};
    Json(const Json &other) : m_json(cJSON_Duplicate(other.m_json, true)){};
    Json(Json &&other) : m_json(other.m_json){other.m_json = cJSON_CreateObject();};
    Json(cJSON *json) : m_json(json){};
    Json(const char* json) : m_json(cJSON_Parse(json)) {};

    ~Json() {
        cJSON_Delete(m_json);
    }

    Json &operator = (const Json &other) {
        cJSON_Delete(m_json);
        m_json = cJSON_Duplicate(other.m_json, true);

        return *this;
    }
    private:
    class JsonField {
    public:
        JsonField(const char *field, cJSON *json) : m_field(field), m_json(json){};

        template<class T>
        operator std::optional<T>() {
            auto j = cJSON_GetObjectItem(m_json, m_field);
            Get<T>(j);
        }

        template<class T>
        void operator = (T t) {
            Set(m_field, t, m_json);
        }

    private:
        const char* m_field;
        cJSON *m_json;
    };

    public:
    // set
    JsonField operator[] (const char *key) {
        return JsonField(key, m_json);
    }

    // get
    JsonField operator[] (const char *key) const {
        return JsonField(key, m_json);
    }

private:
    // members
    cJSON *m_json;

    //functions
    public:

    cJSON *GetObject() {
        return m_json;
    }

    template<class TYPE>
    std::optional<TYPE> GetValue() {
        Get<TYPE>(m_json);
//        if (IsNull()) {
//            return {};
//        }
//
//        if constexpr(std::is_same_v<TYPE, bool>){
//            if(cJSON_IsBool(m_json)){
//                return cJSON_IsTrue(m_json);
//            }
//        }
//
//        if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
//            if(cJSON_IsNumber(m_json)){
//                return cJSON_GetNumberValue(m_json);
//            }
//        }
//
//        if constexpr (std::is_pointer_v<TYPE>) {
//            if(cJSON_IsString(m_json)){
//                return cJSON_GetStringValue(m_json);
//            }
//        }
//
//        return {};
    }

    template<class TYPE>
    void AddNewObject(const char* key, TYPE value) {
        Set(key,  value, m_json);


//        if constexpr(std::is_same_v<TYPE, bool>){
//            cJSON_AddBoolToObject(m_json, key, value);
//        }
//
//        else if constexpr(std::is_integral_v<TYPE> || std::is_same_v<TYPE, float> || std::is_same_v<TYPE, double>) {
//            cJSON_AddNumberToObject(m_json, key, value);
//        }
//
//        else if constexpr (std::is_pointer_v<TYPE>) {
//            cJSON_AddStringToObject(m_json, key, value);
//        }
    }




    cJSON *GetFirstMatch() {
        return nullptr;
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
