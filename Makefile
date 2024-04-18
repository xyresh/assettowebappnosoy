all:
	g++ laptimes.cpp -o update_laptimes

install:
	mkdir /var/www
	mkdir /var/www/html
	cp update_laptimes /var/www/html/
	cp index.html /var/www/html