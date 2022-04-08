#!/bin/sh
mkdir -p ../data/cookies ../data/priority_msgs ../data/msgs ../data/users;
chown -R web:web ../data;
chmod -R +rrr .
chmod 755 ./web1.0.py
chmod 755 ./cgi-bin
chmod 755 ./cgi-bin/web1.0
su web -c "python3 web1.0.py";
