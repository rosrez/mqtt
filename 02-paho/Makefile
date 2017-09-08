QUIET?=@

TARGETNAMES = \
	paho_pub \

-include ../project.mak

$(BLDDIR)/paho_pub: paho_pub.o
	$(CC) -o $@ $^ -lpaho-mqtt3cs
