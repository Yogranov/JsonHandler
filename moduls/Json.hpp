#ifndef JSONCLASS_JSON_HPP
#define JSONCLASS_JSON_HPP

#include <optional>
#include <cstdio>
#include <variant>
#include "../cJSON/cJSON.h"
#include "../cJSON/cJSON.h"
#include <cassert>
int count = 0;

class Json {
    using json_var_t =  std::variant<bool, char *, const char *, long long, double, Json>;

    class JsonField {
    public:
        JsonField(const char *field, cJSON *json) : m_field(field), m_json(json){assert(m_json != nullptr);};

        template<class T>
        operator std::optional<T>() {
            return get<T>(cJSON_GetObjectItem(m_json, m_field));
        }

        template<class T>
        void operator = (T val) {
            if(checkDuplication(m_json, m_field))
                cJSON_DeleteItemFromObject(m_json, m_field);

            addNode(m_field, val, m_json);
        }

        void operator = (Json &json) {
            if(checkDuplication(m_json, m_field))
                cJSON_DeleteItemFromObject(m_json, m_field);

            addNode(m_field, json, m_json);
        }

        void operator = (Json &&json) {
            if(checkDuplication(m_json, m_field))
                cJSON_DeleteItemFromObject(m_json, m_field);

            addNode(m_field, &json, m_json);
        }

        void operator = (std::initializer_list<json_var_t> list) {
            if(checkDuplication(m_json, m_field))
                cJSON_DeleteItemFromObject(m_json, m_field);

            *this = Json{list};
        }

        JsonField operator[] (const char *key) {
            return JsonField{key, m_json};
        }

    private:
        const char* m_field;
        cJSON *m_json;
    };

    /// ------------ Constructors ------------ ///
    public:
    Json() : m_json(cJSON_CreateObject()){};
    Json(const char* jsonString) : m_json(cJSON_Parse(jsonString)) { assert(m_json != nullptr);};
    Json(cJSON *json) : m_json(json){};

    Json(const Json &other) {
        count++;
        m_json = cJSON_Duplicate(other.m_json,true);
    }

    Json(Json &&other) {
        m_json = other.m_json;
        other.m_json = nullptr;
    }

    template<class T>
    Json(const char* key, T value) {
        m_json = cJSON_CreateObject();
        cJSON_AddItemToObject(m_json, key, createNode(value));
    };

    Json(std::initializer_list<json_var_t> &list) : m_json(cJSON_CreateArray()){
        for(json_var_t item : list) {
            cJSON *tmpNode{};

            if(std::holds_alternative<Json>(item))
                tmpNode = createNode(std::get<Json>(item));
            else
                std::visit([this, &tmpNode] (const auto &arg){tmpNode = createNode(arg);}, item);

            cJSON_AddItemToArray(m_json, tmpNode);
        }
    };

    /// Private constructor, use by initializer list to build an array
    private:
        template<class T>
        Json(T value) {
            m_json = createNode(value);
        };
    public:

    ~Json() {
        cJSON_Delete(m_json);
    }

    /// ------------ Operators overloading ------------ ///
    Json &operator = (Json &&other) {
        cJSON_Delete(m_json);
        m_json = other.m_json;
        other.m_json = nullptr;

        return *this;
    }

    JsonField operator[] (const char *key) {
        return JsonField{key, m_json};
    }

    /// members
    private:

    cJSON *m_json;


    /// ------------ Functions - private ------------ ///
    private:

    /// Static
    template<class T>
    static std::optional<T> get(cJSON *json) {
        if constexpr(std::is_same_v<T, bool>){
            if(cJSON_IsBool(json)){
                return cJSON_IsTrue(json);
            }
        }

        else if constexpr(std::is_arithmetic_v<T>) {
            if(cJSON_IsNumber(json)){
                double tmp = cJSON_GetNumberValue(json);
                return tmp;
            }
        }

        else if constexpr (std::is_pointer_v<T>) {
            if(cJSON_IsString(json)){
                return cJSON_GetStringValue(json);
            }
        }

        return {};
    }

    template<class T>
    static void addNode(const char* key, T val, cJSON *json) {
        if constexpr(std::is_same_v<T, bool>){
            cJSON_AddBoolToObject(json, key, val);
        }

        else if constexpr(std::is_arithmetic_v<T>) {
            cJSON_AddNumberToObject(json, key, val);
        }

        else if constexpr(std::is_same_v<T, Json*>){
            assert(val->m_json != nullptr);

            cJSON_AddItemToObject(json, key, val->m_json);
            val->m_json = nullptr;
        }

        else if constexpr ((std::is_same_v<T, const char*> || std::is_same_v<T, char*>)) {
            cJSON_AddStringToObject(json, key, val);
        }

        else if constexpr(std::is_same_v<T, Json>){
            assert(val.m_json != nullptr);

            cJSON_AddItemToObject(json, key, val.m_json);
            val.m_json = nullptr;
        }


    }

    static bool checkDuplication(cJSON *json, const char *key) {
        return cJSON_GetObjectItem(json, key) != nullptr;
    }

    cJSON *createNode(Json &value) {
        assert(value.m_json != nullptr);

        cJSON *tmpPtr = value.m_json;
        value.m_json = nullptr;

        return tmpPtr;
    }

    /// Non-static
    template<class T>
    cJSON *createNode(T value) {
        if constexpr(std::is_same_v<T, bool>){
            return cJSON_CreateBool( value);
        }

        else if constexpr(std::is_arithmetic_v<T>) {
            return cJSON_CreateNumber(value);
        }

        else if constexpr(std::is_same_v<T, Json*>){
            assert(value->m_json != nullptr);

            cJSON *tmpPtr = value->m_json;
            value->m_json = nullptr;

            return tmpPtr;
        }

        else if constexpr(std::is_same_v<T, Json>){
            assert(value.m_json != nullptr);

            cJSON *tmpPtr = value.m_json;
            value.m_json = nullptr;

            return tmpPtr;
        }

        else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            return cJSON_CreateString(static_cast<const char*>(value));
        }

        assert(0);
    }

    /// ------------ Functions - public ------------ ///

    public:

    /// Static
    // ...

    /// Non-static
    cJSON *GetCJson() {
        return m_json;
    }

    bool HaveChildren() {
        return m_json->child != nullptr;
    }

    bool HaveBrothers() {
        return m_json->prev != m_json;
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
        assert(m_json != nullptr);

        char *buffer = cJSON_Print(m_json);
        printf("%s\n", buffer);
        cJSON_free(buffer);
    }
};

#endif //JSONCLASS_JSON_HPP
