#!/bin/bash
echo "Welcome to @gehaxelt's gitonator challenge."
echo "Let's dump a .git/ repository from a web server of your choice. Can I haz URL plz: "
read -r GITURL

DIR=$(mktemp -d)

cat <<EOF > "$DIR/clone.sh"
#!/bin/bash

cd "$DIR"
bash /opt/GitTools/Dumper/gitdumper.sh "$GITURL" "./repo"

if [ -d "./repo" ]; then
    cd "./repo"
    echo "Running git checkout ."
    git checkout . 2>&1 
else
    echo "Failed to dump the repository"
fi

echo "Bye"
EOF

chmod  700 "$DIR"
chmod +x "$DIR/clone.sh"
chown -R cloner:root "$DIR/"

timeout 10s su -l cloner "$DIR/clone.sh"

rm -rf "$DIR"