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

