#pragma once

#include <any>
#include <atomic>
#include <filesystem>
#include <map>
#include <mutex>
#include <optional>
#include <vector>
#include <type_traits>
#include <utility>

namespace fe {
    template<typename T, typename = void>
    inline constexpr bool supportsPlusAssign = false;
    
    template<typename T>
    inline constexpr bool supportsPlusAssign<T, std::void_t<decltype(std::declval<T&>() += std::declval<const T&>())>> = true;

    class ApplicationRegistry {
        public:
            enum class Key {
                AUTHORIZATION_SKIPED,
                CURRENT_ACTION,
                DECRYPTION_INFO,
                FILE_PASSWORD,
                FILE_TO_DECRYPT,
                FILE_TO_ENCRYPT,
                FILES_TO_ENCRYPT,
                FOLDER_TO_ENCRYPT,
                OUPUT_FILE_NAME,
                PROCESSED,
                RUNNING,
                USB_ID
            };

            static void push(Key paramKey, std::any paramValue) {
                std::scoped_lock lock(mutex);
                state[paramKey] = paramValue;
            }

            template<typename T>
            static void additivePush(Key paramKey, const std::any& paramValue) {
                std::scoped_lock lock(mutex);

                if constexpr (supportsPlusAssign<T>) {
                    if (state.contains(paramKey) && state[paramKey].type() == typeid(T) && paramValue.type() == typeid(T)) {
                        const auto& a = std::any_cast<const T&>(state[paramKey]);
                        const auto& b = std::any_cast<const T&>(paramValue);
                        state[paramKey] = a + b;
                    } else {
                        throw std::runtime_error("Unable add this type");
                    }
                } else {
                    throw std::runtime_error("This type is not support adding");
                }
            }


            template <typename T>
            static std::optional<T> pull(Key paramKey) {
                std::scoped_lock lock(mutex);

                if (state.contains(paramKey)) {
                    return std::any_cast<T>(state[paramKey]);
                }

                return std::nullopt;
            }

            template<typename... Keys>
            static bool containsAny(Keys... keys) {
                std::scoped_lock lock(mutex);
                return (... || state.contains(keys));
            }

            static void flush() {
                std::scoped_lock lock(mutex);
                state.clear();
            }

        private:
            inline static std::map<Key, std::any> state;
            inline static std::mutex mutex;

            ApplicationRegistry() = default;
    };
}