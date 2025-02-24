
# Historial de Comandos

Este proyecto en C analiza el historial de comandos de un usuario, proporcionando estadísticas y visualizaciones sobre el uso de comandos en la terminal.


## Instalación

Para compilar y ejecutar el programa, sigue estos pasos:
## Pasos para Configurar

1. **Abrir el archivo de configuración de Zsh**  
   Edita tu archivo `~/.zshrc` con tu editor de texto:
   ```sh
   nano ~/.zshrc
   ```
   o
   ```sh
   vim ~/.zshrc
   ```

2. **Añadir las siguientes líneas al archivo**  
   Copia y pega el siguiente código al final de `~/.zshrc`:
   
   ```sh
   # Configuración del historial de Zsh
   export HISTFILE=~/.zsh_history
   HISTSIZE=50000000
   SAVEHIST=50000000

   # Formato de tiempo para el historial
   export HISTTIMEFORMAT="%Y-%m-%d "

   # Crear un archivo de historial separado por fecha + comando
   export HISTFILE_DATE=~/.zsh_history_date

   # Función para guardar el comando con la fecha
   precmd() {
       echo "$(date "+%Y-%m-%d") $(fc -ln -1)" >> $HISTFILE_DATE
   }
   ```

3. **Aplicar los cambios**  
   Guarda el archivo y cierra el editor. Luego, ejecuta:
   ```sh
   source ~/.zshrc
   ```

4. **Verificación**  
   Para comprobar que la configuración funciona correctamente, ejecuta un comando de prueba y revisa el archivo de historial:
   ```sh
   ls
   cat ~/.zsh_history_date
   ```
   Deberías ver los comandos ejecutados junto con la fecha correspondiente.

## Notas Adicionales
- `HISTSIZE` y `SAVEHIST` están configurados en 50 millones para mantener un historial extenso. Puedes ajustarlo según tus necesidades.
- El archivo `~/.zsh_history_date` almacenará el historial con fecha, separado del historial estándar de Zsh.
- `precmd()` es una función especial en Zsh que se ejecuta antes de cada prompt, lo que permite registrar el comando con la fecha.

¡Listo! Ahora tu historial de Zsh estará mejor organizado con fechas incluidas.

1.Clona el repositorio:

    git clone https://github.com/Tello-P/cli_wrapped
    cd cli_wrapped
2.Compila el programa:

    gcc main.c -o cli_wrapped

3.Ejecuta el programa:

    ./cli_wrapped

## Uso

El programa analiza el historial de comandos almacenado en los archivos ~/.zsh_history y ~/.zsh_history_date. Proporciona estadísticas sobre los comandos más utilizados, combinaciones de comandos y el uso de comandos por mes.

## Funciones Principales
#### longitud_historial

Calcula la longitud del historial de comandos.

#### obtener_comandos

Obtiene todos los comandos del historial.

#### contador_comandos

Cuenta la frecuencia de cada comando.

#### ordenar_cadena

Ordena los comandos por frecuencia de uso.

#### sumar_comandos_principales

Suma la frecuencia de los comandos principales.


## Visualizaciones

El programa incluye varias visualizaciones para mostrar los datos de manera atractiva:

  -Comandos Más Usados

  -Combinaciones de Comandos Más Usadas

  -Uso de Comandos por Mes
  
  -Días con Más Comandos


## Contribuciones

¡Las contribuciones son bienvenidas! Si deseas contribuir, por favor sigue estos pasos:

   Haz un fork del repositorio.
   
   Crea una nueva rama (git checkout -b feature/nueva-funcionalidad).
   
   Realiza tus cambios y haz commit (git commit -am 'Añadir nueva funcionalidad').
   
   Haz push a la rama (git push origin feature/nueva-funcionalidad).
   
   Abre un Pull Request.

## Licencia

Este proyecto está bajo la Licencia GNU3. Consulta el archivo LICENSE para más detalles.




