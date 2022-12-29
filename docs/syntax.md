Syntax for tahaScript
=====================

list of preprocessor commands:
------------------------------

#Lines that start with '#' will be read by the preprocessor

COMMAND                                               PARAMS                         EXAMPLE

* '#d'    -> Define constant global variable         (identifier, value)             *#c my_identifier 21*
* '#f'    -> If symbol defined                       (identifier)                    *#f my_identifier*
* '#fv'   -> If symbol defined to a value            (identifier, value)             *#fv my_identifier 21*
* '#n'    -> If symbol is not defined                (identifier)                    *#n my_int_identifier*
* '#fv'   -> If symbol not defined to a value        (identifier, value)             *#nv my_identifier 23*
* '#e'    -> Ends the if block                       NONE                            *#e*
* '#i'    -> Include file                            (fileName)                      *#i include.tsc*             __@WIP__
* '<'     -> comment blocks that will not be interpreted



