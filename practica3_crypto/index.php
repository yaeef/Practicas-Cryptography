<?php
// 1. Iniciar la sesión al principio de todo
session_start();

// 2. Recuperar los valores anteriores (si existen) para el "sticky form"
// Usamos htmlspecialchars para prevenir XSS al imprimir en 'value'
$archivo_prev = htmlspecialchars($_SESSION['form_data']['archivo'] ?? '');
$bandera_prev = htmlspecialchars($_SESSION['form_data']['bandera'] ?? '');
$tipo_prev    = htmlspecialchars($_SESSION['form_data']['tipo'] ?? '');
$key_prev     = htmlspecialchars($_SESSION['form_data']['key'] ?? '');
$iv_prev      = htmlspecialchars($_SESSION['form_data']['iv'] ?? '');

// 3. Recuperar el mensaje de error y la salida del comando (si existen)
$error_msg = htmlspecialchars($_SESSION['error_message'] ?? '');
$command_output = htmlspecialchars($_SESSION['command_output'] ?? '');

?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Práctica 3 - Cifrador AES</title>
    <style>
        body { font-family: sans-serif; background-color: #f4f4f9; color: #333; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; padding: 20px 0; }
        .container { background-color: #fff; padding: 2em; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 90%; max-width: 500px; }
        h1 { color: #444; text-align: center; }
        label { display: block; margin-top: 15px; font-weight: bold; }
        select, input[type="text"], button { width: 100%; padding: 10px; margin-top: 5px; border-radius: 5px; border: 1px solid #ccc; font-size: 16px; box-sizing: border-box; }
        button { background-color: #007bff; color: white; cursor: pointer; border: none; margin-top: 20px; }
        button:hover { background-color: #0056b3; }
        .output { margin-top: 20px; padding: 10px; background-color: #e9ecef; border-radius: 5px; white-space: pre-wrap; font-family: monospace; word-wrap: break-word; }
        
        /* Estilos para notificación de error */
        .notification { margin-bottom: 20px; padding: 15px; border-radius: 4px; }
        .error { background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
        .error strong { color: #721c24; }
        .error pre { background-color: #f5c6cb; color: #721c24; padding: 10px; border-radius: 4px; white-space: pre-wrap; word-wrap: break-word; }
    </style>
</head>
<body>

<div class="container">
    <h1>Cifrador AES</h1>

    <?php
    // 4. Mostrar el bloque de notificación si hay un error
    if (isset($_GET['status']) && $_GET['status'] == 'error' && !empty($error_msg)):
    ?>
        <div class="notification error">
            <strong>¡Error!</strong> <?php echo $error_msg; ?>
            <?php if (!empty($command_output)): ?>
                <p>Salida del comando:</p>
                <pre><?php echo $command_output; ?></pre>
            <?php endif; ?>
        </div>
    <?php
    endif;
    ?>

    <form method="post" action="driver.php">
        <label for="archivo">1. Selecciona una Imagen:</label>
        <select name="archivo" id="archivo" required>
            <option value="" disabled <?php echo empty($archivo_prev) ? 'selected' : ''; ?>>-- Elige un archivo --</option>
            <?php
                $directorio = 'archivos/';
                if (is_dir($directorio)) {
                    $archivos = scandir($directorio);
                    foreach ($archivos as $archivo) {
                        if ($archivo != "." && $archivo != "..") {
                            // 5. Marcar como 'selected' si coincide con el valor previo
                            $selected = ($archivo == $archivo_prev) ? 'selected' : '';
                            echo "<option value=\"$archivo\" $selected>" . htmlspecialchars($archivo) . "</option>";
                        }
                    }
                }
            ?>
        </select>

        <label for="bandera">2. Selecciona una opción:</label>
        <select name="bandera" id="bandera" required>
            <option value="" disabled <?php echo empty($bandera_prev) ? 'selected' : ''; ?>>-- Elige una acción --</option>
            <option value="-d" <?php echo ($bandera_prev == '-d') ? 'selected' : ''; ?>>Decifrar</option>
            <option value="-e" <?php echo ($bandera_prev == '-e') ? 'selected' : ''; ?>>Cifrar</option>
        </select>
        
        <label for="tipo">3. Selecciona un tipo de cifrado:</label>
        <select name="tipo" id="tipo" required>
            <option value="" disabled <?php echo empty($tipo_prev) ? 'selected' : ''; ?>>-- Elige un tipo --</option>
            <option value="0" <?php echo ($tipo_prev == '0') ? 'selected' : ''; ?>>ECB</option>
            <option value="1" <?php echo ($tipo_prev == '1') ? 'selected' : ''; ?>>CBC</option>
            <option value="2" <?php echo ($tipo_prev == '2') ? 'selected' : ''; ?>>CFB</option>
            <option value="3" <?php echo ($tipo_prev == '3') ? 'selected' : ''; ?>>OFB</option>
        </select>
        
        <div id="key-container">
            <label for="key">4. Ingresa una llave de 16 bytes</label>
            <input type="text" name="key" id="key" placeholder="Llave de Cifrado/Decifrado" pattern="\d{16}" maxlength="16" value="<?php echo $key_prev; ?>" required>
        </div>
        
        <div id="iv-container">
            <label for="iv">5. Ingresa un Vector de Inicialización (IV) de 16 bytes</label>
            <input type="text" name="iv" id="iv" placeholder="Vector de Inicialización" pattern="\d{16}" maxlength="16" value="<?php echo $iv_prev; ?>" required>
        </div>

        <button type="submit" name="ejecutar">Ejecutar</button>
    </form>
</div>

</body>
</html>
<?php
// 6. Limpiar la sesión para que el error no se muestre de nuevo
unset($_SESSION['form_data']);
unset($_SESSION['error_message']);
unset($_SESSION['command_output']);
?>
