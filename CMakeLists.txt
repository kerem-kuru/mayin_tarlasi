cmake_minimum_required(VERSION 3.20)
project(mayn_tarlas)

set(CMAKE_CXX_STANDARD 17)

# 1. Kütüphane başlık (include) ve derlenmiş kütüphane (lib) dosyalarının yolları
include_directories("${CMAKE_SOURCE_DIR}/external/include")
link_directories("${CMAKE_SOURCE_DIR}/external/lib")

# 2. Çalıştırılabilir dosyayı oluştur (main.cpp'den)

add_executable(${PROJECT_NAME} main.cpp resource.rc)
# 3. Kütüphaneleri projeye bağla
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_ttf)

# 4. Gerekli DLL ve BMP dosyalarını derleme (build) klasörüne otomatik kopyala
file(COPY "${CMAKE_SOURCE_DIR}/SDL3.dll" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/SDL3_ttf.dll" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/flag.bmp" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/mine.bmp" DESTINATION "${CMAKE_BINARY_DIR}")