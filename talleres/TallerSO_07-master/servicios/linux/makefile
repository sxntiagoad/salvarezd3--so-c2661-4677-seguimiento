CC=gcc

all:
	$(CC) systemd-service.c -o systemd-service
	$(CC) servicio1.c -o servicio1
	$(CC) cliente1.c  -o cliente1
	$(CC) servicio2.c -o servicio2

install:
	cp systemd-service /usr/local/bin/
	cp systemd-service.service /etc/systemd/system/

	cp servicio1 /usr/local/bin/
	cp servicio1.service /etc/systemd/system/

	cp servicio2 /usr/local/bin/
	cp servicio2.service /etc/systemd/system/

	systemctl daemon-reload
	systemctl enable servicio1
	systemctl enable servicio2

clean:
	rm -f systemd-service
	rm -f servicio1
	rm -f cliente1
	rm -f servicio2
