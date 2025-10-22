<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Interfaz HTML con compilado en backend</title>
    <style>
        body { font-family: sans-serif; background-color: #f4f4f9; color: #333; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; padding: 20px 0; }
        .container { background-color: #fff; padding: 2em; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 90%; max-width: 500px; }
        h1 { color: #444; }
        label { display: block; margin-top: 15px; font-weight: bold; }
        select, input[type="text"], button { width: 100%; padding: 10px; margin-top: 5px; border-radius: 5px; border: 1px solid #ccc; font-size: 16px; box-sizing: border-box; }
        button { background-color: #007bff; color: white; cursor: pointer; border: none; margin-top: 20px; }
        button:hover { background-color: #0056b3; }
        .output { margin-top: 20px; padding: 10px; background-color: #e9ecef; border-radius: 5px; white-space: pre-wrap; font-family: monospace; word-wrap: break-word; }
        /* Estilo para el contenedor del campo extra, oculto por defecto */
        #contenedor-valor-extra { display: none; }
    </style>
</head>
<body>

<div class="container">
    <h1>Affine Cipher 4 txt</h1>
    <form method="post" action="index.php">
        <label for="archivo">1. Selecciona un archivo:</label>
        <select name="archivo" id="archivo" required>
            <option value="" disabled selected>-- Elige un archivo --</option>
            <?php
                $directorio = 'archivos/';
                if (is_dir($directorio)) {
                    $archivos = scandir($directorio);
                    foreach ($archivos as $archivo) {
                        if ($archivo != "." && $archivo != "..") {
                            echo "<option value=\"$archivo\">$archivo</option>";
                        }
                    }
                }
            ?>
        </select>

        <label for="bandera">2. Selecciona una opción:</label>
        <select name="bandera" id="bandera" required>
            <option value="-d">decifrar</option>
            <option value="-e">cifrar</option>
        </select>
        
        <div id="contenedor-valor-alpha">
            <label for="valor_alpha">3. Ingresa el valor de Alpha</label>
            <input type="text" name="valor_alpha" id="valor_alpha" placeholder="Ej: 1,3,5,7,9,11,15,17,19,21,23,25">
        </div>
        <div id="contenedor-valor-beta">
            <label for="valor_beta">4. Ingresa el valor de Beta</label>
            <input type="text" name="valor_beta" id="valor_beta" placeholder="Desplazamiendo del 0 al 26">
        </div>

        <button type="submit" name="ejecutar">Ejecutar Programa</button>
    <?php var_dump($_SESSION)?>
    </form>

    <?php
    session_start();
    if(isset($_POST['ejecutar']))
    {
        $flag = $_POST['bandera'];
        $name = $_POST['archivo'];
        $path = 'archivos/' . $name;
    
        if(!($flag === '-d' || $flag === '-e') || !file_exists($path)) 
        {
            echo "<div class='output'>Error: Datos de entrada no válidos o archivo no encontrado.</div>";
            exit;
        }
        
        if ($flag === '-e') 
        {
          $valor_alpha = $_POST['valor_alpha'];
          $valor_beta = $_POST['valor_beta'];
          $_SESSION['alpha'] = $valor_alpha;
          $_SESSION['beta'] = $valor_beta;
            if(empty($valor_alpha)) 
            {
                 echo "<div class='output'>Error: El campo del valor Alpha para cifrar no puede estar vacío.</div>";
                 exit;
            }
            if(empty($valor_beta))
            {
              echo "<div class='output'>Error: El campo del valor Beta para cifrar no puede estar vacío.</div>";
                exit;
            }
            $comando = './affineTXT ' . $flag . ' ' . $path . ' ' . $valor_alpha . ' ' .$valor_beta;
        } 
        else 
        {
            $comando = './affineTXT ' . $flag . ' ' . $path .' ' . $_SESSION['alpha'] . ' ' . $_SESSION['beta'];
        }
        $salida = shell_exec($comando . ' 2>&1');
        header("Location: index.php");
        exit();
    }
    ?>
</div>

<script>
    // Obtener los elementos del DOM
    const selectorBandera = document.getElementById('bandera');
    const contenedorAlpha = document.getElementById('contenedor-valor-alpha');
    const contenedorBeta  = document.getElementById('contenedor-valor-beta');

    // Función para actualizar la visibilidad del campo
    function toggleValorAlpha() {
        if (selectorBandera.value === '-e') {
          contenedorAlpha.style.display = 'block'; // Mostrar el campo
          contenedorBeta.style.display = 'block';
        } else {
          contenedorAlpha.style.display = 'none'; // Ocultar el campo
          contenedorBeta.style.display = 'none';
        }
    }

    // Añadir un "escuchador" de eventos que llama a la función cuando cambia la selección
    selectorBandera.addEventListener('change', toggleValorAlpha);

    // Llamar a la función una vez al cargar la página por si el valor por defecto necesita acción
    toggleValorAlpha(); 
</script>

</body>
</html>
