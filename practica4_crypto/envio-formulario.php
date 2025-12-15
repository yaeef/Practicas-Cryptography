<?php
$nombre_usuario=$_POST['nombre'];
$email_usuario=$_POST['email'];
$consulta_usuario=$_POST['mensaje'];
// Creo 3 variables que me toman lo que se envía por POST de cada valor del formulario

$destino="yaef123@gmail.com";

$asunto="Consulta enviada desde www.siteyaef.com.";

$mensaje="Tu Nombre es: ".$nombre_usuario."\r\n";
$mensaje.="Tu Email es: ".$email_usuario."\r\n";
$mensaje.="Consulta: ".$consulta_usuario."\r\n";

$remitente="From: consulta@yaef.com.";

mail($destino, $asunto, $mensaje, $remitente);


header("Location:contacto.php?i=ok");
// SI EL FORMULARIO SE ENVIO CORRECTAMENTE, PONER EN LA URL DESPUES DEL contacto.PHP UN SIGNO DE PREGUNTA, UNA LETRA I, Y UN IGUAL OK
?>
