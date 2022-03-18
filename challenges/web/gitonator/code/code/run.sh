#!/bin/bash
socat TCP-LISTEN:31337,fork,reuseaddr,pktinfo,pf=ip4 EXEC:"/app/task.sh"
