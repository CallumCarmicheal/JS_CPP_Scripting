#include "stdafx.h"
#include "configuration.h"
#include "configuration_def.h"
#include "utility/utility.h"

/// ========== Enable configurations
INITIALIZE_EASYLOGGINGPP

void CONFIGURATION_LIBRARIES() {
    char *argv[] = {NULL};
    CONFIGURATION_LIBRARIES(0, argv);
}

void CONFIGURATION_LIBRARIES(START_UP_ARGS(argc, argv)) {
    config_EasyLoggingPP(argc, argv);
}

void CONFIGURATION_CLEANUP() {
    cleanup_EasyLoggingPP();
}

/// ==========  EASY LOGGING PP ==========

void config_EasyLoggingPP(START_UP_ARGS(argc, argv)) {
	START_EASYLOGGINGPP(argc, argv);

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    el::Configurations defaultConf;
    defaultConf.setToDefault();

    defaultConf.setGlobally(el::ConfigurationType::Format, CONFIG_LOG_FORMAT);
    defaultConf.setGlobally(el::ConfigurationType::Filename, CONFIG_LOG_FILE_NAME);


#ifdef __DEBUG__
    defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
#else 
    defaultConf.setGlobally(el::ConfigurationType::ToFile, CONFIG_LOG_FILE_ENABLED);
#endif

#ifdef __WINDOWS__
    // Disable stdout as we are already handling that
    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");

    el::Helpers::installLogDispatchCallback<ELDispatchColourHandler>("ColourDispatcher");
    elppConfigDispatcher = el::Helpers::logDispatchCallback<ELDispatchColourHandler>("ColourDispatcher");
    elppConfigDispatcher->setEnabled(true);
#elif defined(__XBOX360__)
    // Xbox specific logger here
#endif

    // default logger uses default configurations
    el::Loggers::reconfigureLogger("default", defaultConf);
}

void cleanup_EasyLoggingPP() {
#ifdef __WINDOWS__
    if (elppConfigDispatcher != nullptr) {
        // Remove hooks
        el::Helpers::uninstallLogDispatchCallback<ELDispatchColourHandler>("ColourDispatcher");
        elppConfigDispatcher->setEnabled(false);
        
        // Delete and free the memory
        delete elppConfigDispatcher;
        elppConfigDispatcher = nullptr;
    }
#endif
}

void ELDispatchColourHandler::handle(const el::LogDispatchData* data) noexcept {
    m_data = data;

    #ifdef __WINDOWS__
        // Dispatch using colours
        if (ELPP->hasFlag(el::LoggingFlag::ColoredTerminalOutput)) {
            dispatchWithColourLevel(m_data->logMessage()->level(),
                m_data->logMessage()->logger()->logBuilder()->build(m_data->logMessage(),
                    m_data->dispatchAction() == el::base::DispatchAction::NormalLog));
        }
    #else
        // Dispatch using default log builder of logger
        dispatch(m_data->logMessage()->logger()->logBuilder()->build(m_data->logMessage(),
                    m_data->dispatchAction() == el::base::DispatchAction::NormalLog));
    #endif
}

#ifdef __WINDOWS__
void ELDispatchColourHandler::dispatchWithColourLevel(el::Level level, el::base::type::string_t&& logLine) {
    bool resetColour = true;

    switch (level) {
    default: // Info, Global, Unknown
        resetColour = false;
        SetStdConsoleTextAttributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;

    case el::Level::Debug:
        /* Purple */                SetStdConsoleTextAttributes(FG_BLUE | FOREGROUND_INTENSITY); break;
    case el::Level::Trace:
        /* Grey */                  SetStdConsoleTextAttributes(FOREGROUND_INTENSITY); break;
    case el::Level::Warning:
        /* Yellow */                SetStdConsoleTextAttributes(FOREGROUND_RED | FOREGROUND_GREEN); break;
    case el::Level::Verbose:
        /* Yellow | Intense */      SetStdConsoleTextAttributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
    case el::Level::Fatal:
        /* Red | Intense */         SetStdConsoleTextAttributes(FOREGROUND_RED); break;
    case el::Level::Error:
        /* Red */                   SetStdConsoleTextAttributes(FOREGROUND_RED | FG_INTENSITY); break;
    }

    std::cout << logLine << std::flush;

    if (resetColour) SetStdConsoleTextAttributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
#endif

void ELDispatchColourHandler::dispatch(el::base::type::string_t&& logLine) noexcept {
    std::cout << logLine << std::flush;
}