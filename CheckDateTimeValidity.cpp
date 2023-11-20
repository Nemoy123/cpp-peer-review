#include <optional>

enum class NotValidity {
    Year_is_too_small,
    Year_is_too_big,
    Month_is_too_small,
    Month_is_too_big,
    Day_is_too_small,
    Day_is_too_big,
    Hour_is_too_small,
    Hour_is_too_big,
    Minute_is_too_small,
    Minute_is_too_big,
    Second_is_too_small,
    Second_is_too_big
};

//optional выдает std::nullopt если дата валидна, если нет - enum
std::optional <NotValidity> CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        
        return {NotValidity::Year_is_too_small};
    }
    if (dt.year > 9999) {
        
        return {NotValidity::Year_is_too_big};
    }

    if (dt.month < 1) {
        
        return {NotValidity::Month_is_too_small};
    }
    if (dt.month > 12) {
       
        return {NotValidity::Month_is_too_big};
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1) {
       
        return {NotValidity::Day_is_too_small};
    }
    if (dt.day > month_lengths[dt.month - 1]) {
       
        return {NotValidity::Day_is_too_big};
    }

    if (dt.hour < 0) {
        
        return {NotValidity::Hour_is_too_small};
    }
    if (dt.hour > 23) {
        
        return {NotValidity::Hour_is_too_big};
    }

    if (dt.minute < 0) {
        
        return {NotValidity::Minute_is_too_small};
    }
    if (dt.minute > 59) {
        
        return {NotValidity::Minute_is_too_big};
    }

    if (dt.second < 0) {
        
        return {NotValidity::Second_is_too_small};
    }
    if (dt.second > 59) {
        
        return {NotValidity::Second_is_too_big};
    }

    return std::nullopt;
}