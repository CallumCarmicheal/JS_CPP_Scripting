#pragma once

#include <easylogging++.h>
#include <iostream>
#include <string>

#define START_UP_ARGS(argc, argv) int argc, char *argv[]

/// ===== Classes =====
class ELDispatchColourHandler : public el::LogDispatchCallback
{
protected:
    void handle(const el::LogDispatchData* data) noexcept override;
private:
    const el::LogDispatchData* m_data;

#ifdef __WINDOWS__
    void dispatchWithColourLevel(el::Level level, el::base::type::string_t&& logLine);
#endif

    void dispatch(el::base::type::string_t&& logLine) noexcept;
};


/// ===== Libraries to configure =====
void  config_EasyLoggingPP( START_UP_ARGS(argc, argv) );
void  cleanup_EasyLoggingPP();

/// ===== Global instanced variables =====
#ifdef __WINDOWS__ 
ELDispatchColourHandler* elppConfigDispatcher = nullptr;
#endif
