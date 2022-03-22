FROM ubuntu:latest

RUN apt-get update && \
DEBIAN_FRONTEND=noninteractive apt-get install -y xinetd qemu-system-x86 hashcash && \
addgroup user && \
adduser --disabled-password --gecos "" --ingroup user user

COPY app.xinetd /etc/xinetd.d/app

WORKDIR /home/user/
COPY ./code/bzImage ./code/initramfs.cpio.gz ./code/run.sh ./

CMD service xinetd start && tail -F /var/log/xinetdlog

