<?php
	include "config.php";
	if(isset($_POST['content'])) {
		$TEMPLATE = "blank";
		$CONTENT = $_POST['content'];

		$USERID = md5(time() . ":" . microtime());
		$USERDIR = $COMPILEDIR . $USERID;

		$header = file_get_contents($TEMPLATEDIR . $TEMPLATE . "/header.tex");
		$footer = file_get_contents($TEMPLATEDIR . $TEMPLATE . "/footer.tex");

		$content = $header.$CONTENT.$footer;

		if(preg_match("(\.\.|FLAG|immediate|input|include)", $CONTENT)) {
			echo 'BLACKLISTED commands used';
		} else {
			file_put_contents($USERDIR . ".tex", $content);

			$CMD = "cd $COMPILEDIR && $PDFLATEX --no-shell-escape $USERID.tex";
			$output = shell_exec($CMD);

			if(file_exists($USERDIR . ".pdf")) {
				rename($USERDIR . ".pdf", $OUTPUTDIR . $USERID . ".pdf");
				echo "FILE CREATED: $USERID.pdf\n";
				echo "Download: $DLURL$USERID.pdf\n";
			}

			@unlink($USERDIR . ".tex");
			@unlink($USERDIR . ".log");
			@unlink($USERDIR . ".aux");


			echo "\n\nLOG:\n";
			echo $output;
		}
	} else {
		echo 'Error, wrong data';
	}
?>