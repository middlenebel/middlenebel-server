


## 
Some times the server can't start because the port is in use (sometimes by himself if crashed)

You can destroyed him in windows:
```
> netstat -aon | findstr '8080'
  TCP    127.0.0.1:8080         0.0.0.0:0              LISTENING       8512
  TCP    127.0.0.1:8080         127.0.0.1:51363        ESTABLISHED     8512
  TCP    127.0.0.1:8080         127.0.0.1:51364        ESTABLISHED     8512
  TCP    127.0.0.1:8080         127.0.0.1:51365        ESTABLISHED     8512
  TCP    127.0.0.1:8080         127.0.0.1:51366        ESTABLISHED     8512
[...]

> tasklist | findstr '8512'
wslrelay.exe                  8512 Console                    1     8.856 KB
> taskkill /F /PID 8512
Correcto: se terminó el proceso con PID 8512.
```