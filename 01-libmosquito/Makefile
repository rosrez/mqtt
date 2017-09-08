QUIET?=@

TARGETNAMES = \
	mosq_pub \
	mosq_sub 

-include ../project.mak

$(BLDDIR)/mosq_pub: mosq_pub.o
	$(CC) -o $@ $^ -lmosquitto

$(BLDDIR)/mosq_sub: mosq_sub.o
	$(CC) -o $@ $^ -lmosquitto
