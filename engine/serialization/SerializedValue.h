#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <cstdint>

class SerializedValue
{
public:
    using Array = std::vector<SerializedValue>;
    using Object = std::map<std::string, SerializedValue>;

    using Value = std::variant<std::nullptr_t, bool, double, std::uint64_t, std::string, Array, Object>;

    SerializedValue()
        : value_(nullptr)
    {
    }

    SerializedValue(bool value)
        : value_(value)
    {
    }

    SerializedValue(double value)
        : value_(value)
    {
    }

    SerializedValue(uint64_t value)
        : value_(value)
    {
    }

    SerializedValue(std::string value)
        : value_(value)
    {
    }

    SerializedValue(Array value)
        : value_(value)
    {
    }

    SerializedValue(Object value)
        : value_(value)
    {
    }

    template<typename T>
    bool is() const
    {
        return std::holds_alternative<T>(value_);
    }

    template<typename T>
    const T& get() const
    {
        return std::get<T>(value_);
    }

    

private:
    Value value_;
};