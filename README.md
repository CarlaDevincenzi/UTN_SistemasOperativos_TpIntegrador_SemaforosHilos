# UTN_SistemasOperativos_TpIntegrador_SemaforosHilos
UNIVERSIDAD TECNOLÓGICA NACIONAL
FACULTAD REGIONAL MAR DEL PLATA
ARQUITECTURA Y SISTEMAS OPERATIVOS
1er Año – 2do Cuatrimestre
PROFESORES: G. GIMENEZ – L. CHIESSA - E. MONACO

9. Trabajo Práctico Especial - Enunciado

Mirtha LeBig tiene un popular programa de televisión en el que invita a importantes referentes del show business y de la derecha argentina 
a almorzar y conversar sobre sus vidas y sobre temas de actualidad. 

La dinámica del programa es la siguiente:
• Al programa asisten N invitados. Los comensales se sientan a la mesa, siendo Mirtha la última en sentarse.
• Una vez que se sentó Mirtha, los M mozos sirven la comida. Por problemas de presupuesto, la cantidad de mozos es siempre menor a la cantidad de invitados.
• Los mozos no deben servir más de N platos. Cualquier comensal (incluida Mirtha) no puede comenzar a comer, incluso si no han terminado de servir a todos los invitados.
• Cuando Mirtha termina de comer, ella lanza dos pregunta polémica. 
  Esta pregunta puede ser respondida por cualquier invitado que haya o no terminado de comer. 
  Sólo un invitado debe responder cada una de las pregunta. Además para realizar la segunda pregunta polémica, se debe haber respondido la primera.

• Para mantener alto el rating, Mirtha se enoja muchísimo por la segunda respuesta polémica, se levanta y se va del estudio. Si Mirtha ya se ha levantado, 
los invitados pueden hacer lo mismo, pero sólo si ya han terminado de comer.

Las primitivas disponibles son:
• sentarse()
• servir_comida()
• comer()
• enojarse()
• levantarse()
• lanzar_pregunta_polemica()
• lanzar_respuesta_polemica()
