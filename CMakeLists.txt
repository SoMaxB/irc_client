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

add_executable(irc_client_qt ${SOURCES} ${HEADERS})

target_include_directories(irc_client_qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(irc_client_qt
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
)

# Compiler warnings
if(MSVC)
    target_compile_options(irc_client_qt PRIVATE /W4)
else()
    target_compile_options(irc_client_qt PRIVATE -Wall -Wextra -Wpedantic)
endif()
