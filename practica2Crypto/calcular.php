<?php
// Comprobar si se han enviado los datos mediante POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // 1. Obtener los valores del formulario
    $a = $_POST['a'];
    $b = $_POST['b'];
    $n = $_POST['n'];

    // 2. Validar y "sanitizar" la entrada para el comando de shell
    $a_safe = escapeshellarg($a);
    $b_safe = escapeshellarg($b);
    $n_safe = escapeshellarg($n);

    // 3. Construir y ejecutar el comando
    $command = "./euclides $a_safe $b_safe $n_safe";
    //$output = shell_exec($command);
    exec($command, $output, $return_code);
    
    // Si la salida es nula o vacía, hubo un problema con la ejecución
    if (empty(trim($output))) {
        // Manejar un error genérico si el script de C no devuelve nada
        header("Location: index.php?error=3&a=".urlencode($a)."&b=".urlencode($b)."&n=".urlencode($n));
        exit();
    }

    // 4. Procesar la salida
    list($a_inverso, $b_inverso) = explode(' ', trim($output));

    // 5. Lógica de redirección por error
    if ($a_inverso == '-1') {
        // Redirigir a index.php con error=1 y los valores originales
        header("Location: index.php?error=1&a=".urlencode($a)."&b=".urlencode($b)."&n=".urlencode($n));
        exit(); // Detener la ejecución del script
    }
    
    // Suponiendo que tu C también podría devolver -1 para b
    if ($b_inverso == '-1') {
        // Redirigir a index.php con error=2 y los valores originales
        header("Location: index.php?error=2&a=".urlencode($a)."&b=".urlencode($b)."&n=".urlencode($n));
        exit(); // Detener la ejecución del script
    }

    // 6. Si no hay errores, mostrar la página de resultados (código sin cambios)
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Resultado del Cálculo</title>
    <style>
        body { font-family: sans-serif; max-width: 500px; margin: 40px auto; }
        .resultado { margin-top: 20px; padding: 15px; background-color: #f0f0f0; border-left: 5px solid #28a745; }
        a { color: #007bff; }
    </style>
</head>
<body>
    <h1>¡Cálculo Exitoso!</h1>
    <div class='resultado'>
        <strong>Valores ingresados:</strong> a=<?php echo htmlspecialchars($a); ?>, b=<?php echo htmlspecialchars($b); ?>, n=<?php echo htmlspecialchars($n); ?><hr>
        <strong>Comando ejecutado: </strong> <?php echo htmlspecialchars($command);?><br><hr>
        <strong>Inverso Multiplicativo (a⁻¹):</strong> <?php echo htmlspecialchars($a_inverso); ?><br>
        <strong>Inverso Aditivo (-b):</strong> <?php echo htmlspecialchars($b_inverso); ?><br><hr>
        <strong>Función de Cifrado:</strong> <?php echo "Ck = " . htmlspecialchars($a) . "p + " . htmlspecialchars($b) . " mod " . htmlspecialchars($n); ?><br>
        <strong>Función de Decifrado:</strong> <?php echo " p = Dk = " . htmlspecialchars($a_inverso) . "[Ck + " . htmlspecialchars($b_inverso) . "] mod " . htmlspecialchars($n);  ?>
        
            
    </div>
    <p><a href="index.php">← Realizar otro cálculo</a></p>
</body>
</html>
<?php
} else {
    // Si alguien intenta acceder a calcular.php directamente, lo regresamos al inicio.
    header("Location: index.php");
    exit();
}
?>
