<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, user-scalable=no">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="shortcut icon" href="img/icons8-connect-develop-48.png" type="image/x-icon">
    <link rel="stylesheet" href="css/normalize.css">
    <link href="https://fonts.googleapis.com/css?family=Montserrat:300,400&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="css/style.css">
    <title>Alex Franco</title>
</head>
<body>
    <a href="https://api.whatsapp.com/send?phone=+525573705014&text=!Hola¡%20Quiero%20crear%20mi%20pagina%20web!" id="btn-whatsapp" target="_blank">  <img src="img/whatsapp.png" alt="Icono de whatsapp"></a>
    <header class="site-header-g flex-header">
        <div class="barra">
            <div class="contenedor">
                <a href="/"><p class="logo">Alex Franco</p></a>
                <img src="img/menu-resp.png" alt="Burger Menu" class="menu-resp">
                <nav class="nav"> 
                    <ul class="lista-menu">
                        <li><a href="/">inicio</a></li>
                        <li><a href="index.html#ancla-quien-soy">¿quien soy?</a></li>
                        <li><a href="index.html#ancla-estudios">estudios</a></li>
                        <li><a href="contacto.php">contacto</a></li>
                    </ul>
                </nav>
            </div>
        </div>
    </header>

    <main>
        <section>
                <?php
                if(@$_GET['i']=='ok') { // QUIERE DECIR QUE EL FORMULARIO SE ENVIO CORRECTAMENTE ?>
          
                <h3 class="contenedor centrar-texto fw-300 envio">La consulta se envio correctamente. Nos contactaremos a la brevedad.</h3>
          
                <?php
                }
                else {
                    ?>                
            <h2 class="fw-300 centrar-texto">Contacto</h2>
            <div class="contenedor ">
                <form action="envio-formulario.php" method="POST" enctype="multipart/form-data" class="form">
                    <input type="text" name="nombre"  placeholder="Nombre" required>
                    <input type="email" name="email" placeholder="example@dominio.com" required>
                    <textarea name="mensaje"  placeholder="Mensaje" required></textarea>
                    <input type="submit" value="Enviar">
                </form>
                <?php } ?>
            </div>
            
        </section>
        <section class="form-banner">
            <h2></h2>
            <div class="contenedor">
            </div>
        </section>
    </main>

    <footer>
        <div class="contenedor-footer contenedor">
             <div class="contenedor-redes">
                <a href="https://www.facebook.com/AlejandroErreguinFranco" target="_blank" ><img src="img/facebook.png" alt=""></a>
                <a href="https://api.whatsapp.com/send?phone=5573705014&text=!Hola%C2%A1%20Quiero%20crear%20mi%20pagina%20web!" target="_blank"><img src="img/whatsapp-icon.png" alt="Icono de whatsapp"></a>
                <a href="https://www.instagram.com/franc0_yair/" target="_blank"><img src="img/instagram.png" alt=""></a>
                    <a href="contacto.php"><img src="img/mail.png" alt="Icono de mail"></a>
            </div>
            <div class="footer-texto">
                <p>Alejandro Erreguin | diseñador Web |Todos los derechos reservados ©</p>
            </div>
            <div class="footer-texto-responsive">
                <p>Todos los derechos reservados ©</p>
            </div>
        </div>
    </footer>
    <script
    src="https://code.jquery.com/jquery-3.4.1.min.js"
    integrity="sha256-CSXorXvZcTkaix6Yvo6HppcZGetbYMGWSFlBw8HfCJo="
    crossorigin="anonymous">
    </script>
    <script>
        $(document).ready(function(){
            $('.menu-resp').click(function(){
                $('nav').slideToggle();
                
            })  
        }) 
    </script>
</body>
</html>