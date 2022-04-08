FROM ubuntu:18.04

RUN apt update && apt-get -y upgrade
RUN apt install -y python3-pip
RUN apt install -y socat

RUN python3 -m pip install pycrypto

RUN mkdir /service
WORKDIR /service
RUN mkdir data

COPY sign.py secret.py privkey.pem run.sh ./

RUN groupadd -g 2000 cryptogroup \
&& useradd -m -u 2001 -g cryptogroup cryptodude

RUN chown -R cryptodude /service
RUN chmod +x sign.py run.sh
USER cryptodude

ENV PYTHONUNBUFFERED=1

CMD ["/service/run.sh"]
