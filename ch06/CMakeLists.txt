add_executable(udp_client udp_client.c)
target_link_libraries(udp_client tcp_ip_common)

add_executable(udp_server udp_server.c)
target_link_libraries(udp_server tcp_ip_common)
