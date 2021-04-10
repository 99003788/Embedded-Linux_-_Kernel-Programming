obj-m += waitqueue.o

KSRC=/home/user/Workspace/package/linux-4.14.202

all:
	make -C ${KSRC} M=${PWD} modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

clean:
	make -C ${KSRC} M=${PWD} clean
