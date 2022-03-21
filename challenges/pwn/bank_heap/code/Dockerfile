FROM ubuntu:latest

RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y xinetd hashcash

RUN addgroup user && adduser --disabled-password --gecos "" --ingroup user user

COPY app.xinetd /etc/xinetd.d/app

WORKDIR /home/user/
RUN chmod 555 /home/user
COPY ./code/bank ./code/flag.txt ./

CMD service xinetd start && tail -F /var/log/xinetdlog

