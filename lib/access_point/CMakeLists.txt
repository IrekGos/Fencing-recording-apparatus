add_library(access_point INTERFACE)

target_sources(access_point INTERFACE
        access_point.c
        dhcpserver/dhcpserver.c
        dnsserver/dnsserver.c
)

target_include_directories(access_point INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
        ${CMAKE_CURRENT_LIST_DIR}/dnsserver
        )

target_link_libraries(access_point INTERFACE
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        )