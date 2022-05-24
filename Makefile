CFLAGS = -O2 -Wall -I. -Icommon

COMMON_OBJS=common/log.o common/datetime.o

CH05_CLIENT_OBJS=ch05/socket_client.o
CH05_SERVER_OBJS=ch05/socket_server.o
TEMP_OBJS=temp/test_debug.o

ch05: $(COMMON_OBJS) $(CH05_CLIENT_OBJS) $(CH05_SERVER_OBJS)
	gcc $(CFLAGS) -o ch05/socket_client $(COMMON_OBJS) $(CH05_CLIENT_OBJS)
	gcc $(CFLAGS) -o ch05/socket_server $(COMMON_OBJS) $(CH05_SERVER_OBJS)

test_debug: $(COMMON_OBJS) $(TEMP_OBJS)
	gcc $(CFLAGS) -o temp/test_debug $(COMMON_OBJS) $(TEMP_OBJS)

common/%.o: common/%.c
	gcc $(CFLAGS) -c $< -o $@

ch05/%.o: ch05/%.c
	gcc $(CFLAGS) -c $< -o $@

temp/%.o: temp/%.c
	gcc $(CFLAGS) -c $< -o $@

clean05:
	rm -f common/*.o ch05/*.o ch05/socket_client ch05/socket_server

.PHONY: clean
