<?php
// 1. Iniciar la sesión
session_start();

// 2. Validar que se llegó aquí por POST y con el botón 'ejecutar'
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['ejecutar'])) {

    // 3. Obtener todos los datos del formulario
    $name = $_POST['archivo'];
    $flag = $_POST['bandera'];
    $type = $_POST['tipo'];
    $key = $_POST['key'];
    $iv = $_POST['iv'];
    
    $input_path = 'archivos/' . $name;
    // Sanitizar la ruta completa
    $input_path_safe = escapeshellarg($input_path);

    // 4. Sanitización de datos para la línea de comandos
    $name_safe = escapeshellarg($name);
    $flag_safe = escapeshellarg($flag);
    $type_safe = escapeshellarg($type);
    $key_safe = escapeshellarg($key);
    $iv_safe = escapeshellarg($iv);

    // 5. Construir el comando
    // Asegúrate de que tu ejecutable 'aes' esté en la misma carpeta y tenga permisos de ejecución (chmod +x aes)
    
    if($type == 0 || $type == 1)
    {
    		 $command = "./bloques $flag_safe $input_path_safe  $type_safe $key_safe $iv_safe";
    }
   if($type == 2 || $type == 3)
    {
    		 $command = "./flujos $flag_safe $input_path_safe  $type_safe $key_safe $iv_safe";
    }
    
    // 6. Ejecutar el comando
    $output = [];
    $return_code = -1;
    exec($command, $output, $return_code);

    // 7. Lógica de éxito o error
    if ($return_code !== 0) {
        // ERROR: El comando falló
        
        // 7.1. Guardar los datos del formulario en la sesión para el "sticky form"
        $_SESSION['form_data'] = $_POST;
        
        // 7.2. Guardar el mensaje de error y la salida del comando
        $_SESSION['error_message'] = "La operación falló (Código de retorno: $return_code).";
        $_SESSION['command_output'] = implode("\n", $output); // Convertir array de salida en string
        
        // 7.3. Redirigir de vuelta a index.php
        header("Location: index.php?status=error");
        exit(); // Detener la ejecución

    } else {
        // ÉXITO: El comando funcionó
        
        // Limpiar cualquier dato de error viejo
        unset($_SESSION['form_data']);
        unset($_SESSION['error_message']);
        unset($_SESSION['command_output']);
        
        // Definir variables para mostrar en la página de éxito
        if($type === '0'){ $tipo = "ECB";}
        if($type === '1'){ $tipo = "CBC";}
        if($type === '2'){ $tipo = "CFB";}
        if($type === '3'){ $tipo = "OFB";}
        
        if($flag === '-e'){ $tipo_op = "Cifrado";}
        if($flag === '-d'){ $tipo_op = "Decifrado";}
        
        // 7.4. Mostrar la página de éxito (tu HTML original)
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Estado de la Operación</title>
    <style>
        body { font-family: sans-serif; background-color: #f4f4f9; color: #333; max-width: 600px; margin: 40px auto; padding: 20px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); border-radius: 8px; background-color: #fff; }
        h1 { color: #28a745; }
        .resultado { margin-top: 20px; padding: 15px; background-color: #f0f0f0; border-left: 5px solid #28a745; word-wrap: break-word; }
        a { color: #007bff; }
        button { background-color: #007bff; color: white; cursor: pointer; border: none; padding: 10px 20px; margin-top: 20px; border-radius: 5px; font-size: 16px; }
        button:hover { background-color: #0056b3; }
        .output { background-color: #007bff; } /* Tu estilo original */
        pre { background-color: #e9ecef; padding: 10px; border-radius: 4px; white-space: pre-wrap; word-wrap: break-word; font-family: monospace; }
    </style>
</head>
<body>
    <h1>¡Operación AES Exitosa!</h1>
    <div class='resultado'>
        <strong>Valores ingresados:</strong><br>
        Operación: <strong><?php echo htmlspecialchars($tipo_op); ?></strong><br>
        Modo: <strong><?php echo htmlspecialchars($tipo); ?></strong><br>
        Key: <strong><?php echo htmlspecialchars($key);?></strong><br>
        IV: <strong><?php echo htmlspecialchars($iv);?></strong>
        <hr>
        <strong>Comando ejecutado: </strong>
        <pre><?php echo htmlspecialchars($command);?></pre>
        <hr>
        <strong>Salida del comando:</strong>
        <pre><?php echo htmlspecialchars(implode("\n", $output)); ?></pre>
    </div>
    
    <form method="get" action="index.php">
        <button type="submit" class="output">← Regresar</button>
    </form>
</body>
</html>
<?php
    } // Fin del 'else' de éxito

} else {
    // 8. Si alguien accede a driver.php directamente, redirigirlo a index.php
    header("Location: index.php");
    exit();
}
?>
