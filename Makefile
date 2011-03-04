
# test/test1.c makefile

lib =  lib_ftp_explore.o lib_ftp_backup.o lib_ftp_delete.o lib_ftp_upload.o

%.o: src/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags glib-2.0` -I/usr/include -lcurl

objects = test1.o

%.o: test/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags glib-2.0`

compile: $(lib) $(objects)
	gcc $(lib) $(objects) -o test1 `pkg-config --libs glib-2.0` -lcurl -I/usr/include

clean:
	rm *.o -fv
