all:
	@make -f libcommon.mk
	@make install -f libcommon.mk
	@make -f libnet.mk
	@make install -f libnet.mk
	@make -f test.mk

install:

clean:
	-${RM} *.so
	-${RM} *.o
	-${RM} *.d
