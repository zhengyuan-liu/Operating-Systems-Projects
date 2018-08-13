# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu


.SILENT:

default:
	echo "#!/bin/bash" > lab3b
	echo 'python lab3b.py $$1' >> lab3b
	chmod a+x lab3b
	
build: default

clean:
	rm -f lab3b.tar.gz lab3b

dist: 
	tar -czf lab3b.tar.gz lab3b.py Makefile README
