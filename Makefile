CXX=g++
OPTS=-O0 -fexceptions -g -std=c++0x
LD=-L/usr/local/lib -lboost_program_options -lmsgpack -lmpio -lmsgpack-rpc
PATH_MSGPACK_RPC=../msgpack-rpc/cpp/src/msgpack/rpc
TEST_LD= -lpthread $(LD)
CCLOG_LD=../msgpack-rpc/cpp/src/cclog/*.o
GTEST_INC= -I$(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_DIR=/opt/google/gtest-1.5.0
GMOCK_DIR=/opt/google/gmock-1.5.0
WARNS= -W -Wall -Wextra -Wformat=2 -Wstrict-aliasing=4 -Wcast-qual -Wcast-align \
	-Wwrite-strings -Wfloat-equal -Wpointer-arith -Wswitch-enum
NOTIFY=&& notify-send Test success! -i ~/themes/ok_icon.png || notify-send Test failed... -i ~/themes/ng_icon.png
SRCS=$(HEADS) $(BODYS)
MSGPACK_RPC_OBJS=$(PATH_MSGPACK_RPC)/*.o

#target:msgcache
#logic_test: logic_test.o gtest_main.a libgmock.a logic_detail.o sg_objects.o
#	$(CXX) $^ -o $@ $(GTEST_INC) $(TEST_LD) $(OPTS) $(WARNS)  $(CCLOG_LD)
msgcache: msgcache.o interface.o
	$(CXX) $^ -o $@ $(LD) $(OPTS) $(WARNS)  $(PATH_MSGPACK_RPC)/*.o -I$(PATH_MSGPACK_RPC)/  $(CCLOG_LD)

msgcache.o:msgcache.cc
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)
interface.o:interface.cc
	$(CXX) -c $< -o $@ $(OPTS) $(WARNS)
# gtest
gtest_main.o:
	$(CXX) $(GTEST_INC) -c $(OPTS) $(GTEST_DIR)/src/gtest_main.cc -o $@
gtest-all.o:
	$(CXX) $(GTEST_INC) -c $(OPTS) $(GTEST_DIR)/src/gtest-all.cc -o $@
gtest_main.a:gtest-all.o gtest_main.o
	ar -r $@ $^

libgmock.a:
	g++ ${GTEST_INC} -I${GTEST_DIR} -I${GMOCK_DIR}/include -I${GMOCK_DIR} -c ${GTEST_DIR}/src/gtest-all.cc
	g++ ${GTEST_INC} -I${GTEST_DIR} -I${GMOCK_DIR}/include -I${GMOCK_DIR} -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgmock.a gtest-all.o gmock-all.o

clean:
	rm -f *.o
	rm -f *~
