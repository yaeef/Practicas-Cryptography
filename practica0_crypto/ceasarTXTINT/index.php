<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Interfaz para mi Programa en C</title>
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
    <h1>Ceasar Cipher 4 txt</h1>
    <form method="post" action="index.php">
        <label for="archivo">1. Selecciona un archivo:</label>
        <select name="archivo" id="archivo" required>
            <option value=""disabled selected>-- Elige un archivo --</option>
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
        
        <div id="contenedor-valor-extra">
            <label for="valor_extra">3. Ingresa el valor para cifrar</label>
            <input type="text" name="valor_extra" id="valor_extra" placeholder="Desplazamiendo del 0 al 25" pattern="^(?:[0-9]|1[0-9]|2[0-5])$">
        </div>

        <button type="submit" name="ejecutar">Ejecutar Programa</button>
    </form>

    <?php
    
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
            $valor_extra = $_POST['valor_extra'];
            if (empty($valor_extra)) 
            {
                 echo "<div class='output'>Error: El campo del valor para encriptar no puede estar vacío.</div>";
                 exit;
            }
            $comando = './ceasarTXT ' . $flag . ' ' . $path . ' ' . $valor_extra;
        } 
        else 
        {
            $comando = './ceasarTXT ' . $flag . ' ' . $path .' ' . '0';
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
    const contenedorExtra = document.getElementById('contenedor-valor-extra');

    // Función para actualizar la visibilidad del campo
    function toggleValorExtra() {
        if (selectorBandera.value === '-e') {
            contenedorExtra.style.display = 'block'; // Mostrar el campo
        } else {
            contenedorExtra.style.display = 'none'; // Ocultar el campo
        }
    }

    // Añadir un "escuchador" de eventos que llama a la función cuando cambia la selección
    selectorBandera.addEventListener('change', toggleValorExtra);

    // Llamar a la función una vez al cargar la página por si el valor por defecto necesita acción
    toggleValorExtra(); 
</script>

</body>
</html>
