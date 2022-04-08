FROM python:3.9-slim-buster

RUN pip install ipython

# Not running as root.
ADD https://github.com/Yelp/dumb-init/releases/download/v1.2.0/dumb-init_1.2.0_amd64 /usr/local/bin/dumb-init
RUN chmod +x /usr/local/bin/dumb-init

WORKDIR /app

# Add user so we don't need --no-sandbox.
RUN groupadd -r ipython && useradd -r -g ipython -G audio,video ipython \
    && mkdir -p /home/ipython \
    && chown -R ipython:ipython .

USER ipython

COPY flag.txt .
COPY enojail.py .

ENV LISTEN_PORT 5656
EXPOSE 5656
ENTRYPOINT ["dumb-init", "--"]
CMD ["python", "./enojail.py"]