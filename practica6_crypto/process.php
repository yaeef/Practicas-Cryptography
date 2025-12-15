<?php
// Configuración de directorios
$UPLOAD_DIR = './uploads/';
$BIN_KEYGEN = './key_gen_rsa';
$BIN_ENDEC = './endec';

// Crear directorio de subida si no existe
if (!is_dir($UPLOAD_DIR)) {
    mkdir($UPLOAD_DIR, 0777, true);
}

// Inicializar variables de respuesta
$result_status = "Error: Acción no reconocida.";
$result_output = "";
$exit_code = -1;

// --- Funciones Auxiliares ---

/**
 * Mueve un archivo subido a un directorio seguro.
 * @param array $file_info La entrada del array $_FILES.
 * @return string|false La ruta temporal del archivo o false en caso de error.
 */
function handle_upload($file_info) {
    global $UPLOAD_DIR;
    if ($file_info['error'] !== UPLOAD_ERR_OK) {
        return false;
    }
    // Generar un nombre seguro para evitar colisiones
    $temp_filename = uniqid('upload_', true) . '_' . basename($file_info['name']);
    $upload_path = $UPLOAD_DIR . $temp_filename;

    if (move_uploaded_file($file_info['tmp_name'], $upload_path)) {
        return $upload_path;
    }
    return false;
}

// --- Lógica Principal ---

if (isset($_POST['action'])) {
    $action = $_POST['action'];

    if ($action == 'keygen') {
        // 1. Generar Llaves
        $command = escapeshellcmd($BIN_KEYGEN); // Comando: ./key_gen_rsa
        $result_status = "Generación de Llaves";
        $result_output = shell_exec($command);
        $exit_code = 0; // shell_exec no retorna el código de salida directamente, asumimos éxito si hay output
        
        if (empty($result_output)) {
             $result_output = "Comando ejecutado. Revisa si se crearon public_key.pem y private_key.pem.";
        }

    } elseif ($action == 'encrypt' || $action == 'decrypt') {
        
        // 2. Cifrar / 3. Descifrar
        $mode = ($action == 'encrypt') ? '-e' : '-d';
        $result_status = ($action == 'encrypt') ? "Cifrado RSA" : "Descifrado RSA";

        // a. Manejar Subida de Archivos
        $input_path = handle_upload($_FILES['input_file']);
        $key_path = handle_upload($_FILES['key_file']);

        if ($input_path && $key_path) {
            // b. Construir Comando
            // Se usa escapeshellarg para proteger las rutas de archivos de la inyección de comandos
            $cmd_input = escapeshellarg($input_path);
            $cmd_key = escapeshellarg($key_path);
            $command = "$BIN_ENDEC $mode $cmd_input $cmd_key"; // Comando: ./endec -e file_path key_path

            // c. Ejecutar Comando
            $output_array = [];
            $exit_code = -1;
            $result_output = exec($command, $output_array, $exit_code);

            // d. Limpiar Archivos Temporales
            unlink($input_path);
            unlink($key_path);

            // e. Evaluar Resultado
            if ($exit_code === 0) {
                $result_output = "Operación exitosa. El archivo resultante (" . 
                                (($mode == '-e') ? 'ciphertext.enc' : 'ciphertext_decrypted.txt') . 
                                ") debería haberse creado en el directorio del servidor.";
            } else {
                $result_output = "Fallo la ejecución del binario. Código de salida: $exit_code. Revisa los permisos y la salida de error (si fue capturada): " . implode("\n", $output_array);
            }

        } else {
            $result_output = "Error al subir uno o ambos archivos. Verifica el tamaño y los permisos.";
            $exit_code = -1;
        }
    }
}

?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Resultado de la Operación</title>
    <style>
        /* Estilos del HTML principal para consistencia */
        body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f9; }
        .container { max-width: 800px; margin: auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }
        h1 { color: #333; }
        .result { margin-top: 20px; padding: 15px; border-radius: 5px; }
        .success { background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .error { background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
        pre { white-space: pre-wrap; word-wrap: break-word; background-color: #eee; padding: 10px; border-radius: 3px; }
        a { color: #007bff; text-decoration: none; }
    </style>
</head>
<body>

<div class="container">
    <h1>Resultados de la Operación: <?php echo $result_status; ?></h1>

    <div class="result <?php echo ($exit_code === 0 || $action == 'keygen') ? 'success' : 'error'; ?>">
        <h2>Estado de la Ejecución</h2>
        <p><strong>Comando Ejecutado:</strong> <code><?php echo htmlspecialchars($command ?? "N/A"); ?></code></p>
        <p><strong>Código de Retorno:</strong> <code><?php echo $exit_code; ?></code></p>

        <h3>Salida (stdout/stderr):</h3>
        <pre><?php echo htmlspecialchars($result_output); ?></pre>
    </div>

    <p><a href="index.html">← Volver al Menú Principal</a></p>
</div>

</body>
</html>
