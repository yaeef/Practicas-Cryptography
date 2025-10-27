<?php
// Obtener los valores de la URL para rellenar el formulario.
// Usamos htmlspecialchars() por seguridad, para prevenir ataques XSS.
$val_a = isset($_GET['a']) ? htmlspecialchars($_GET['a']) : '';
$val_b = isset($_GET['b']) ? htmlspecialchars($_GET['b']) : '';
$val_n = isset($_GET['n']) ? htmlspecialchars($_GET['n']) : '';

// Variable para el mensaje de error
$error_message = '';
if (isset($_GET['error'])) {
    if ($_GET['error'] == '1') {
        $error_message = '<strong>Error:</strong> Los valores de a y n no son coprimos. Prueba con otro valor para a.';
    } elseif ($_GET['error'] == '2') {
        // Este caso es poco probable con el inverso aditivo, pero lo manejamos.
        $error_message = '<strong>Error:</strong> No se pudo calcular el inverso aditivo de \'B\' ya que no cumple (0 <= B < n)';
    }
}
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Calculadora Cifrado Afín</title>
    <style>
        body { font-family: sans-serif; max-width: 500px; margin: 40px auto; }
        input { width: 100%; padding: 8px; margin-bottom: 10px; box-sizing: border-box; }
        button { padding: 10px 15px; background-color: #007bff; color: white; border: none; cursor: pointer; }
        .notification { margin-bottom: 20px; padding: 15px; border-radius: 4px; }
        .error { background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
    </style>
</head>
<body>
    <h1>Calculadora de Inversos (Cifrado Afín)</h1>
    <p>Ingresa los valores para calcular a⁻¹ y -b (mod n).</p>

    <?php if (!empty($error_message)): ?>
        <div class="notification error">
            <?php echo $error_message; ?>
        </div>
    <?php endif; ?>

    <form action="calcular.php" method="POST">
        <label for="a">Valor de a:</label>
        <input type="number" id="a" name="a" value="<?php echo $val_a; ?>" required>

        <label for="b">Valor de b:</label>
        <input type="number" id="b" name="b" value="<?php echo $val_b; ?>" required>

        <label for="n">Valor de n (módulo):</label>
        <input type="number" id="n" name="n" value="<?php echo $val_n; ?>" required>

        <button type="submit">Calcular</button>
    </form>
</body>
</html>