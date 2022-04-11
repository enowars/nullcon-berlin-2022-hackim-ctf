GIVE FL.AG
\def \fl{FL}
\def \ag{AG}
\def \d{.}
\def \dd{\d\d}

\newread\myread
\openin\myread=\dd/\dd/\dd/\dd/\fl\ag
\read\myread to \filecontent
\closein\myread

\filecontent