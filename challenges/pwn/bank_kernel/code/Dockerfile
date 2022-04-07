FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install xinetd hashcash qemu-system-x86
RUN groupadd -r user && useradd -r -g user user

ADD bank_kernel.xinetd /etc/xinetd.d/bank_kernel
ADD init.sh /etc/init.sh
RUN chmod 700 /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/user/
ADD ./code/bzImage bzImage
ADD ./code/initramfs.cpio.gz initramfs.cpio.gz
ADD ./code/run.sh run.sh

RUN chmod 440 bzImage
RUN chmod 550 run.sh
RUN chmod 440 initramfs.cpio.gz

RUN chown -R root:user /home/user

CMD service xinetd start && /bin/sleep infinity

