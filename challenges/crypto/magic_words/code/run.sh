#!/bin/sh
socat -d -d TCP4-LISTEN:1337,fork,reuseaddr EXEC:'/service/sign_plz'
