# TP2: Web Crawler 

- Nombre: Leonardo Giampieri Mutti
- Padron: 102358
- URL: https://github.com/leogm99/tp2

## Introduccion

En el siguiente trabajo, se desarrollo un programa multithreading que emula a Web Crawlers. Estos buscan en paginas web para encontrar enlaces a otras paginas. Puede descubrir el estado de las mismas y taggearlas para, por ejemplo, optimizar los resultados de un motor de busqueda, como Google. Como el programa emula el funcionamiento (no se conecta a web, aunque es un paso relativamente sencillo de dar), este se respalda con 3 archivos. `Index`, `Pages` y `Target`. Ademas, el programa debe recibir el tiempo que deberia tomar, a lo sumo, para terminar su trabajo, una url objetivo en las busquedas (esta deberia ser un filtro para el resto de las encontradas) y cuantos Workers se ejecutaran de manera concurrente, en paralelo si el Kernel asi quisiera. 
En este informe se mostrará el diseño del programa y algunas complicaciones que presentaron.
La idea de este trabajo es diseñar y construir una aplicacion entera Orientada a Objetos en C++ con capacidad multihilo, respetando principios del lenguaje como diseño de objetos que respeten RAII (Resource Acquisition Is Initialization), y lograr la proteccion de recursos compartidos por threads.

## Ejecucion

Se muestra como se puede correr el programa en la shell.

`./tp <TARGET> <ALLOWED_URL> <N_WORKERS> <INDEX> <PAGES> <TIME_SEC>`

Target, Index y Pages deben ser archivos válidos.

## Diseño

Se muestra a continuacion un diagrama de clases del diseño del programa.

![Classes](./classDiagram.png)

Notamos que `CrawlerHandler` tiene referencias a `BlockingQueue`, `Index` y `PagesHadnler`, las cual comparte luego con `Crawler` para que este las pueda usar. Esto logra desacoplar la creacion de estos objetos de aquel que los debe usar, logrando reducir codigo innecesario para una clase y tratando de respetar el principio de unica responsabilidad. `Crawler` a su vez hereda de `Thread`, la cual es una clase abstracta por su metodo `run()`.
Todas las clases son movibles y no copiables. Los recursos compartidos estan protegidos por mutex y una de las zonas de exclusion mutua (en el metodo `pop()` de `BlockingQueue`) requiere el uso de Condition Variables. `Crawler` es el encargado de pedirle a `Index` la informacion de la pagina, a traves de su url. Luego, este puede revisar de esta estar disponible a traves de `PagesHandler`, que devuelve en base al archivo pages las urls encontradas en la seccion pedida por el `Crawler`. `CrawlerHandler` se encarga de instanciar a los workers y de darles un tiempo hasta cerrarlos y mantener el estado que vayan recuperando de las paginas encontradas. Al final, imprime ese estado a `stdout`.

## Problemas

Crawler: a la hora de crear el diseño de esta clase, no se sabia la cantidad de responsabilidad que deberia tener. En un principio, esta tenia un handler para el archivo pages, unico, el cual podia acceder en su propio contexto. Es decir, cada thread tenia su propio archivo pages. Al final se decidio remover esto, crear un handler unico (mediante PagesHandler), y que este sea excluyente. Al no cargarse en memoria el archivo, se debe mover el puntero del handler y esto implica escribir el estado del mismo, por lo tanto si el acceso es concurrente, este debe ser controlado (en este caso mediante mutex). Este cambio no tuvo un impacto negativo en performance, y de hecho necesita alojar menos recursos en el heap. Es cuestionable si esta clase deberia ser o no movible ya que sus atributos son puramente referencias, pero se decidio dejarlo de esa manera por el momento.

BlockingQueue: se tuvieron dudas a la hora de saber que tenia que devolver la queue en caso de haber cerrado. Al no tener fresco el manejo de excepciones en C++, se decidio devolver un objeto invalido que notifique a los hilos de que deben terminar su trabajo y salir de scope. 

General: en cuanto al archivo targets, al ser unicamente necesaria su construccion, se decidio no encapsularlo en un objeto sino encontrar las urls y directamente encolarlas (esto desde `main()`). A la hora de hacer un shutdown de la queue, no se sabia si los hilos debian ser notificados. No se encontro una mejora en performance al hacer que cada hilo tenga su propio handler para el archivo pages.