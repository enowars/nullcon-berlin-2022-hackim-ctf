FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install xinetd
RUN groupadd -r user && useradd -r -g user user

ADD bank.xinetd /etc/xinetd.d/bank
ADD init.sh /etc/init.sh
RUN chmod 700 /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/user/
ADD ./code/bank bank
ADD ./code/flag.txt flag.txt

RUN chmod 440 flag.txt
RUN chmod 550 bank

RUN mv flag.txt flag-$(md5sum flag.txt | awk '{print $1}').txt

RUN chown -R root:user /home/user

CMD service xinetd start && /bin/sleep infinity


