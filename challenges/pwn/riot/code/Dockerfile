FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install xinetd hashcash qemu-system-arm
RUN groupadd -r user && useradd -r -g user user

ADD riot.xinetd /etc/xinetd.d/riot
ADD init.sh /etc/init.sh
RUN chmod 700 /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/user/
ADD ./code/ctf.elf ctf.elf
ADD ./code/run.sh run.sh

RUN chmod 550 ctf.elf
RUN chmod 550 run.sh

RUN chown -R root:user /home/user

CMD service xinetd start && /bin/sleep infinity

