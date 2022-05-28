CFLAGS = -O2 -Wall -I. -Icommon
CC = gcc

COMMON_OBJS=common/log.o common/datetime.o
TEMP_OBJS=temp/test_debug.o

CH05_CLIENT_OBJS=ch05/socket_client.o
CH05_SERVER_OBJS=ch05/socket_server.o

CH06_CLIENT_OBJS=ch06/udp_client.o
CH06_SERVER_OBJS=ch06/udp_server.o

CH07_TCP_CLIENT_OBJS=ch07/local_socket_stream_client.o
CH07_TCP_SERVER_OBJS=ch07/local_socket_stream_server.o
CH07_UDP_CLIENT_OBJS=ch07/local_socket_dgram_client.o
CH07_UDP_SERVER_OBJS=ch07/local_socket_dgram_server.o

ch05: $(COMMON_OBJS) $(CH05_CLIENT_OBJS) $(CH05_SERVER_OBJS)
	$(CC) $(CFLAGS) -o ch05/socket_client $(COMMON_OBJS) $(CH05_CLIENT_OBJS)
	$(CC) $(CFLAGS) -o ch05/socket_server $(COMMON_OBJS) $(CH05_SERVER_OBJS)

ch06: $(COMMON_OBJS) $(CH06_CLIENT_OBJS) $(CH06_SERVER_OBJS)
	$(CC) $(CFLAGS) -o ch06/udp_client $(COMMON_OBJS) $(CH06_CLIENT_OBJS)
	$(CC) $(CFLAGS) -o ch06/udp_server $(COMMON_OBJS) $(CH06_SERVER_OBJS)

ch07: $(COMMON_OBJS) $(CH07_TCP_CLIENT_OBJS) $(CH07_TCP_SERVER_OBJS) $(CH07_UDP_CLIENT_OBJS) $(CH07_UDP_SERVER_OBJS)
	$(CC) $(CFLAGS) -o ch07/local_socket_stream_client $(COMMON_OBJS) $(CH07_TCP_CLIENT_OBJS)
	$(CC) $(CFLAGS) -o ch07/local_socket_stream_server $(COMMON_OBJS) $(CH07_TCP_SERVER_OBJS)
	$(CC) $(CFLAGS) -o ch07/local_socket_dgram_client  $(COMMON_OBJS) $(CH07_UDP_CLIENT_OBJS)
	$(CC) $(CFLAGS) -o ch07/local_socket_dgram_server  $(COMMON_OBJS) $(CH07_UDP_SERVER_OBJS)

test_debug: $(COMMON_OBJS) $(TEMP_OBJS)
	$(CC) $(CFLAGS) -o temp/test_debug $(COMMON_OBJS) $(TEMP_OBJS)

common/%.o: common/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ch05/%.o: ch05/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ch06/%.o: ch06/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ch07/%.o: ch07/%.c
	$(CC) $(CFLAGS) -c $< -o $@

temp/%.o: temp/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f common/*.o ch05/*.o ch06/*.o ch07/*.o temp/*.o

.PHONY: clean
