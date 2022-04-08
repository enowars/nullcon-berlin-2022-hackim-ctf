#!/bin/sh
# Pack it up for maximum open-sourceness

rm -rf data
make clean
sed -i '/ADMIN_PASS_HASH: /s/: .*$/: "VALUE"/g' docker-compose.yml
sed -i '/RESET_PASS: /s/: .*$/: "VALUE"/g' docker-compose.yml
sed -i '/FLAG: /s/: .*$/: "ENO{VALUE}"/g' docker-compose.yml

tar -cvf web1.0source.tar.gz ./*

