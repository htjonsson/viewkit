VKDIR   = ../../ViewKit
INCDIRS = -I${VKDIR}  -I/usr/X11R6/include
LIBDIRS = -L${VKDIR}/lib -L/usr/X11R6/lib
LIBS    = -lvk -lvkhelp -lvk -lXpm -lXmu -lXm -lXt -lX11 -lstdc++
CC	= gcc -O0 -g
OBJS	= main.o VkTabPanel.o
LIBDEPS = ${VKDIR}/lib/libvk.a ${VKDIR}/lib/libvkhelp.a
SRCS    = $(OBJS:.o=.cc)
HDRS    = 	

all : vktabpanel

vktabpanel : ${OBJS} ${LIBDEPS}
	${CC} $(OBJS) -Wall -o vktabpanel $(LIBDIRS) $(LIBS) $(EFENCE)

main.o : main.cc
	${CC} main.cc -c -Wall -o main.o $(INCDIRS)

VkTabPanel.o : VkTabPanel.C
	${CC} VkTabPanel.C -c -Wall -o VkTabPanel.o $(INCDIRS)

clean :
	-rm -f vktabpanel *.o core *~

