build: build_picohttpparser
	gcc -I/usr/include/python3.9 -I/usr/include/python3.9\
		-Wno-unused-result -Wsign-compare  -O2  -fexceptions -g -grecord-gcc-switches -pipe \
		-Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS \
		-fstack-protector-strong   -m64  -mtune=generic -fasynchronous-unwind-tables \
		-fstack-clash-protection -fcf-protection -D_GNU_SOURCE -fPIC -fwrapv \
		-DDYNAMIC_ANNOTATIONS_ENABLED=1 -DNDEBUG  -O2  -fexceptions -g -grecord-gcc-switches \
		-pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS \
		-fstack-protector-strong   -m64  -mtune=generic -fasynchronous-unwind-tables -fstack-clash-protection \
		-fcf-protection -D_GNU_SOURCE -fPIC -fwrapv \
		-shared -o pypicohp/request.so pypicohp/request.c \
		-L/usr/lib64 -Lpypicohp -lcrypt -lpthread -ldl  -lutil -lm -lm -l:picohttpparser.so
	gcc -I/usr/include/python3.9 -I/usr/include/python3.9\
		-Wno-unused-result -Wsign-compare  -O2  -fexceptions -g -grecord-gcc-switches -pipe \
		-Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS \
		-fstack-protector-strong   -m64  -mtune=generic -fasynchronous-unwind-tables \
		-fstack-clash-protection -fcf-protection -D_GNU_SOURCE -fPIC -fwrapv \
		-DDYNAMIC_ANNOTATIONS_ENABLED=1 -DNDEBUG  -O2  -fexceptions -g -grecord-gcc-switches \
		-pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -Wp,-D_GLIBCXX_ASSERTIONS \
		-fstack-protector-strong   -m64  -mtune=generic -fasynchronous-unwind-tables -fstack-clash-protection \
		-fcf-protection -D_GNU_SOURCE -fPIC -fwrapv \
		-shared -o pypicohp/parser.so pypicohp/parser.c \
		-L/usr/lib64 -Lpypicohp -lcrypt -lpthread -ldl  -lutil -lm -lm -l:picohttpparser.so

build_picohttpparser:
	gcc -fPIC -shared -o pypicohp/picohttpparser.so pypicohp/picohttpparser.c

install_picohttpparser: build_picohttpparser
	mv pypicohp/picohttpparser.so /usr/lib64

install:
	python setup.py install

clean:
	rm -rf build dist pypicohp.egg-info

sdist:
	python setup.py sdist

test_upload:
	twine upload --repository testpypi dist/*

upload:
	twine upload --repository pypi dist/*

test:
	python -m unittest discover tests