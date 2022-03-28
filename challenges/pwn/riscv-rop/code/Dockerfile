FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install xinetd hashcash qemu-user
RUN groupadd -r user && useradd -r -g user user

ADD rop.xinetd /etc/xinetd.d/rop
ADD init.sh /etc/init.sh
RUN chmod 700 /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/user/
ADD ./code/chal chal
ADD ./code/flag.txt flag.txt
ADD ./code/run.sh run.sh

RUN chmod 440 flag.txt
RUN chmod 550 chal
RUN chmod 550 run.sh

RUN mv flag.txt flag-$(md5sum flag.txt | awk '{print $1}').txt

RUN chown -R root:user /home/user

CMD service xinetd start
