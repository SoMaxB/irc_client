cmake_minimum_required(VERSION 3.21)
project(IrcClientQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Network
    Concurrent
    Test
    REQUIRED
)

# Headers
set(HEADERS
    include/IrcConnection.hpp
    include/MessageParser.hpp
    include/ChannelModel.hpp
    include/UserModel.hpp
    include/MainWindow.hpp
    include/ConfigManager.hpp
    include/CommandHandler.hpp
    include/IrcNumericHandler.hpp
)

# Sources
set(SOURCES
    src/main.cpp
    src/IrcConnection.cpp
    src/MessageParser.cpp
    src/ChannelModel.cpp
    src/UserModel.cpp
    src/MainWindow.cpp
    src/ConfigManager.cpp
    src/CommandHandler.cpp
    src/IrcNumericHandler.cpp
)

# Test headers
set(TEST_HEADERS
    tests/TestMessageParser.hpp
    tests/TestCommandHandler.hpp
    tests/TestConfigManager.hpp
)

# Test sources
set(TEST_SOURCES
    tests/TestMessageParser.cpp
    tests/TestCommandHandler.cpp
    tests/TestConfigManager.cpp
    tests/main.cpp
)

add_executable(irc_client_qt ${SOURCES} ${HEADERS})

target_include_directories(irc_client_qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(irc_client_qt
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
)

add_executable(run_tests ${TEST_SOURCES} ${TEST_HEADERS})

target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(run_tests PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Test
)

target_sources(run_tests PRIVATE
    src/MessageParser.cpp
    src/CommandHandler.cpp
    src/ConfigManager.cpp
)

# Compiler warnings
if(MSVC)
    target_compile_options(irc_client_qt PRIVATE /W4)
    target_compile_options(run_tests PRIVATE /W4)
else()
    target_compile_options(irc_client_qt PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(run_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()