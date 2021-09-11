//
//  Result.hpp
//  ComplexNumberClass
//
//  Created by Egor Mikhailov on 05.09.2021.
//

#ifndef Result_hpp
#define Result_hpp

#include <variant>
#include <string>
#include <optional>

struct Error {
    std::string description;
    Error(std::string description) {
        this->description = description;
    }
};

template <typename Success>
class Result {
private:
    std::variant<Success, Error> result;
public:
    Result(std::variant<Success, Error> result): result(result) {};
    
    std::optional<Success> success() {
        try {
            auto success = std::get<Success>(result);
            return success;
        }
        catch (std::bad_variant_access) {
            return std::nullopt;
        }
    }

    std::optional<Error> error() {
        try {
            auto error = std::get<Error>(result);
            return error;
        }
        catch (std::bad_variant_access) {
            return std::nullopt;
        }
    }
};

#endif /* Result_hpp */
