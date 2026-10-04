#include "packed_data.hpp"
#include <iostream>
#include <bitset>
#include <iomanip>

PackedData::PackedData(std::uint32_t raw) : value_(raw) {}

PackedData::PackedData(std::uint32_t battery, DeviceMode mode, bool wifi, bool bt, 
                       std::uint32_t error_code, int temp_celsius, bool service) : value_(0) {
    set_battery(battery);
    set_mode(mode);
    set_wifi(wifi);
    set_bluetooth(bt);
    set_error_code(error_code);
    set_temperature_celsius(temp_celsius);
    set_service_required(service);
}

std::uint32_t PackedData::raw() const {
    return value_;
}

std::uint32_t PackedData::battery() const {
    return extract_field(value_, BATTERY_OFFSET, BATTERY_WIDTH);
}

void PackedData::set_battery(std::uint32_t value) {
    if (value > 100) {
        throw std::out_of_range("Заряд батареи не может превышать 100%!");
    }
    value_ = replace_field(value_, value, BATTERY_OFFSET, BATTERY_WIDTH);
}

DeviceMode PackedData::mode() const {
    std::uint32_t m = extract_field(value_, MODE_OFFSET, MODE_WIDTH);
    if (m > 5) {
        throw std::runtime_error("Недопустимый код режима работы в битовом поле!");
    }
    return static_cast<DeviceMode>(m);
}

void PackedData::set_mode(DeviceMode mode) {
    std::uint32_t m = static_cast<std::uint32_t>(mode);
    if (m > 5) {
        throw std::out_of_range("Недопустимый режим работы (разрешено 0-5)!");
    }
    value_ = replace_field(value_, m, MODE_OFFSET, MODE_WIDTH);
}

bool PackedData::wifi() const {
    return extract_field(value_, WIFI_OFFSET, WIFI_WIDTH) != 0;
}

void PackedData::set_wifi(bool value) {
    value_ = replace_field(value_, value ? 1u : 0u, WIFI_OFFSET, WIFI_WIDTH);
}

bool PackedData::bluetooth() const {
    return extract_field(value_, BT_OFFSET, BT_WIDTH) != 0;
}

void PackedData::set_bluetooth(bool value) {
    value_ = replace_field(value_, value ? 1u : 0u, BT_OFFSET, BT_WIDTH);
}

void PackedData::toggle_bluetooth() {
    std::uint32_t mask = 1u << BT_OFFSET;
    value_ ^= mask; // Операция XOR переключает бит
}

std::uint32_t PackedData::error_code() const {
    return extract_field(value_, ERROR_OFFSET, ERROR_WIDTH);
}

void PackedData::set_error_code(std::uint32_t value) {
    value_ = replace_field(value_, value, ERROR_OFFSET, ERROR_WIDTH);
}

int PackedData::temperature_celsius() const {
    std::uint32_t stored = extract_field(value_, TEMP_OFFSET, TEMP_WIDTH);
    return static_cast<int>(stored) - 40;
}

void PackedData::set_temperature_celsius(int temp_celsius) {
    if (temp_celsius < -40 || temp_celsius > 80) {
        throw std::out_of_range("Температура выходит за допустимый диапазон от -40 до +80 °C!");
    }
    std::uint32_t stored = static_cast<std::uint32_t>(temp_celsius + 40);
    value_ = replace_field(value_, stored, TEMP_OFFSET, TEMP_WIDTH);
}

bool PackedData::service_required() const {
    return extract_field(value_, SERVICE_OFFSET, SERVICE_WIDTH) != 0;
}

void PackedData::set_service_required(bool value) {
    value_ = replace_field(value_, value ? 1u : 0u, SERVICE_OFFSET, SERVICE_WIDTH);
}

bool PackedData::check_reserved_bits() const {
    return extract_field(value_, RESERVED_OFFSET, RESERVED_WIDTH) == 0;
}

void PackedData::print_raw() const {
    std::cout << "decimal: " << value_ << '\n';
    std::cout << "binary : " << std::bitset<32>(value_) << '\n';
    std::cout << "hex    : 0x"
              << std::hex << std::uppercase
              << std::setw(8) << std::setfill('0')
              << value_
              << std::dec << '\n';
}