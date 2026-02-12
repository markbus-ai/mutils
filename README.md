# mutils 🛠️

**mutils** (Micro Utilities) es una colección de herramientas CLI ligeras escritas en C, diseñadas para optimizar el flujo de trabajo en entornos de desarrollo Linux.

Su objetivo es reemplazar scripts de shell complejos o comandos repetitivos con binarios rápidos y específicos.

## 🚀 Instalación

1. Clona el repositorio:
   ```bash
   git clone <repo-url>
   cd mutils
   ```

2. Compila el proyecto:
   ```bash
   make
   ```
   Esto generará el binario `mutils`.

3. (Opcional) Agrega el binario a tu PATH o crea un alias para usarlo globalmente.

## 📦 Dependencias

Para compilar y ejecutar todas las herramientas, necesitarás instalar las siguientes librerías y utilidades.

**Bibliotecas de desarrollo (Build-time):**
*   `tesseract` (libtesseract-dev / tesseract-ocr)
*   `leptonica` (libleptonica-dev / leptonica)
*   `libcurl` (libcurl4-openssl-dev)
*   `libarchive` (libarchive-dev)

**Herramientas del sistema (Run-time):**
*   `wl-clipboard` (para copiar al portapapeles en Wayland)
*   `grim` y `slurp` (para capturas de pantalla en Wayland - requerido por `ocr`)

## 📖 Uso y Comandos

El formato general es: `mutils <comando> [argumentos]`

### 👁️ `ocr` - Captura de Texto (Wayland)
Selecciona un área de la pantalla, extrae el texto mediante OCR y lo copia automáticamente al portapapeles. Ideal para extraer código de videos o texto de imágenes no seleccionables.

*Requiere un entorno Wayland con `grim`, `slurp` y `wl-copy` instalados.*

```bash
mutils ocr
# 1. El cursor cambiará para seleccionar una región.
# 2. El texto detectado se copiará al clipboard.
```

### 🔋 `bat` - Gestión de Batería
Herramientas para monitorear la salud y controlar los umbrales de carga (ideal para laptops modernas como ThinkPads, ASUS, etc. para extender la vida útil de la batería).

*   **Chequear salud de la batería:**
    Compara la capacidad actual vs. la de fábrica.
    ```bash
    mutils bat health
    # Salida: Salud: 92.50%
    ```

*   **Establecer límite de carga (Threshold):**
    Configura que la batería deje de cargar al llegar a X porcentaje.
    *Requiere permisos de superusuario (sudo).*
    ```bash
    sudo mutils bat 80
    ```
    *Nota: Modifica `/etc/udev/rules.d/` para persistencia.*

### 🔪 `port` - Port Slayer
Encuentra y elimina (kill -9) cualquier proceso que esté ocupando el puerto especificado. Se acabó el `lsof` + `kill` manual.

```bash
mutils port 8080
# Busca procesos en :8080 y los elimina.
```

### ☢️ `nuke` - Limpieza Recursiva
Recorre un directorio y elimina **recursivamente** carpetas y archivos basura generados por entornos de desarrollo.

**Elimina automáticamente:**
*   `node_modules`, `.npm`
*   `__pycache__`, `.pytest_cache`, `.venv`
*   Archivos compilados: `.o`, `.out`, `.pyc`
*   Carpetas de build: `dist`, `build`

```bash
mutils nuke ./mi-proyecto
```
*⚠️ Úsalo con cuidado.*

### env - Verificador de Entorno
Compara tu archivo `.env` local contra el `.env.example` del proyecto para detectar variables faltantes.

```bash
cd mi-proyecto
mutils env
# Salida:
# [MISSING] DB_PASSWORD
# [MISSING] API_KEY
```

### 📡 `ping` - Diagnóstico de Red
Realiza un chequeo rápido de la salud de tu conexión:
1. Verifica resolución DNS.
2. Mide latencia, pérdida de paquetes y **jitter** (estabilidad).

```bash
mutils ping
# Diagnostica si hay packet loss, lag alto o conexión inestable.
```

### 🌐 `ip` - Public IP
Obtiene rápidamente tu dirección IP pública actual. Útil para verificar si estás tras una VPN o para configuraciones de red.

```bash
mutils ip
# Salida: Public IP: 203.0.113.45
```

### 📦 `x` - Smart Extract
Descompresor inteligente que detecta automáticamente el formato del archivo y utiliza la herramienta adecuada (`tar`, `unzip`, `unrar`, `7z`, etc.). Soporta casi todos los formatos comunes (`.zip`, `.tar.gz`, `.rar`, `.7z`, `.bz2`, etc.).

```bash
mutils x archivo.tar.gz
```

## 🛠️ Desarrollo

El proyecto está estructurado modularmente en la carpeta `tools/`.
*   `main.c`: Despachador de comandos.
*   `tools/*.c`: Implementación de cada utilidad.
*   `makefile`: Sistema de construcción.

Para limpiar los archivos objeto:
```bash
make clean
```
