<?php 
// Challenge by @gehaxelt.
include "config.php";

function showcode($file) {
    $source = show_source($file, true);
    print $source;
    exit(0);
}

if(isset($_GET['src'])) {
    showcode("index.php");
}

if(isset($_GET['file'])) {
    $file = $_GET['file'];
    if(str_contains($file, "..")) {
        die("No automated tools pls.");
    }

    if(!file_exists($file . FLAG . ".php")) {
        die("File not found :-/");
    }

    showcode($file);
}
?>

<html>
<head><title>Sourcer</title></head>
<body>
    <h1>May the source be with you!</h1>
</body>
<!--- view me /?src=1 --->
</html>