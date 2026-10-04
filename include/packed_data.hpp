#pragma once

#include <cstdint>
#include <stdexcept>

// Перечисление для режимов работы устройства
enum class DeviceMode : std::uint32_t {
    Off = 0,
    Standby = 1,
    Active = 2,
    Sleep = 3,
    Charging = 4,
    Diagnostic = 5
};

// Универсальные вспомогательные функции для работы с битами
constexpr std::uint32_t low_mask(std::uint32_t width) {
    return width == 32 ? 0xFFFFFFFFu : ((1u << width) - 1u);
}

constexpr std::uint32_t extract_field(std::uint32_t packed, std::uint32_t offset, std::uint32_t width) {
    return (packed >> offset) & low_mask(width);
}

inline std::uint32_t replace_field(std::uint32_t packed, std::uint32_t field_value, std::uint32_t offset, std::uint32_t width) {
    const std::uint32_t mask = low_mask(width);
    if (field_value > mask) {
        throw std::out_of_range("Введенное значение превышает ширину битового поля!");
    }
    const std::uint32_t shifted_mask = mask << offset;
    return (packed & ~shifted_mask) | ((field_value & mask) << offset);
}

class PackedData {
public:
    // Константы для разметки 32-битного числа
    static constexpr std::uint32_t BATTERY_OFFSET = 0;
    static constexpr std::uint32_t BATTERY_WIDTH  = 7;

    static constexpr std::uint32_t MODE_OFFSET    = 7;
    static constexpr std::uint32_t MODE_WIDTH     = 3;

    static constexpr std::uint32_t WIFI_OFFSET    = 10;
    static constexpr std::uint32_t WIFI_WIDTH     = 1;

    static constexpr std::uint32_t BT_OFFSET      = 11;
    static constexpr std::uint32_t BT_WIDTH       = 1;

    static constexpr std::uint32_t ERROR_OFFSET   = 12;
    static constexpr std::uint32_t ERROR_WIDTH    = 8;

    static constexpr std::uint32_t TEMP_OFFSET    = 20;
    static constexpr std::uint32_t TEMP_WIDTH     = 7;

    static constexpr std::uint32_t SERVICE_OFFSET = 27;
    static constexpr std::uint32_t SERVICE_WIDTH  = 1;

    static constexpr std::uint32_t RESERVED_OFFSET = 28;
    static constexpr std::uint32_t RESERVED_WIDTH  = 4;

    // Конструктор
    explicit PackedData(std::uint32_t raw = 0);

    // Конструктор для сборки из отдельных полей (с валидацией)
    PackedData(std::uint32_t battery, DeviceMode mode, bool wifi, bool bt, 
               std::uint32_t error_code, int temp_celsius, bool service);

    // Геттер «сырого» значения
    std::uint32_t raw() const;

    // Геттеры и сеттеры для каждого поля
    std::uint32_t battery() const;
    void set_battery(std::uint32_t value);

    DeviceMode mode() const;
    void set_mode(DeviceMode mode);

    bool wifi() const;
    void set_wifi(bool value);

    bool bluetooth() const;
    void set_bluetooth(bool value);
    void toggle_bluetooth(); // Переключение (требование методички)

    std::uint32_t error_code() const;
    void set_error_code(std::uint32_t value);

    int temperature_celsius() const; // Возвращает фактическую температуру
    void set_temperature_celsius(int temp_celsius);

    bool service_required() const;
    void set_service_required(bool value);

    bool check_reserved_bits() const; // Проверка, что резерв равен 0

    void print_raw() const; // Метод вывода двоеточного/hex представления

private:
    std::uint32_t value_;
};

// Альтернативная обычная структура для сравнения размеров sizeof
struct StandardDeviceState {
    std::uint32_t battery;
    DeviceMode mode;
    bool wifi;
    bool bt;
    std::uint32_t error_code;
    int temperature_celsius;
    bool service_required;
    std::uint32_t reserved;
};