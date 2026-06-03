cmake_minimum_required(VERSION 3.10)
project(SD_Projekt3_Mieszanie)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dołączenie katalogu z plikami nagłówkowymi
include_directories(include)

# Automatyczne wyszukanie plików źródłowych
file(GLOB SOURCES "src/*.cpp")

# Tworzenie pliku wykonywalnego
add_executable(SD_Projekt3 ${SOURCES})