<?php
$bin = "http://62.98.147.26/cgi-bin/decrypt.cgi?bin=";
$bin .= $_GET['bin'];
//echo $bin;
echo "<html>
<body>
<center>
<canvas id=\"qr\">

</canvas>
</center>
<script src=\"qrious.js\"></script>
<script>
var qr = new QRious({

	    element: document.getElementById('qr'),

	    value: '".$bin."',

	    background: 'white', // background color

	    foreground: 'black', // foreground color

	    backgroundAlpha: 1,

	    foregroundAlpha: 1,

	    level: 'L', // Error correction level of the QR code (L, M, Q, H)

	    mime: 'image/png', // MIME type used to render the image for the QR code

	    size: 500, // Size of the QR code in pixels.

	    padding: null // padding in pixels
	})
</script>
</body>
</html>";

?>