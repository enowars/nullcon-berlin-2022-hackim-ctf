#!/bin/sh
socat -d -d TCP4-LISTEN:1337,fork,reuseaddr EXEC:'python3 /service/sign.py'
