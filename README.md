# SIMPLEDU

#### SOPE First Project - 10/04/2020

`Made in C Programming Language`

It was created for the first part of SOPE’s project, an operative systems subject within MIEIC-FEUP curriculum, for the 2019/20 academic year.

##### Teachers: Jorge Alves da Silva (Main lecturer); Flávio Henrique Ferreira Couto; João Pedro Matos Teixeira Dias

## Description

Simple program that can estimate the size of a *directory* and its *files* in a system. It works like `du` command on **Unix** based systems. However, it is a more simple version of `du` command, so we call it **`simpledu`**. 

![sample image](https://github.com/rodykings/simpledu/blob/master/samples/1.png)

## How it works

Everytime the program finds another directory inside a directory, it creates a new process that is going to handle all the subdirectories and files of that directory. We do recursive calls of the `simpledu` program rearranging the atributes and we do it with `execvp` system call.

**To make conditions and filter information we can use some`flags` that can be combined:**

| Flag | Alternative Flag  | Description                                                               |
|------|-------------------|---------------------------------------------------------------------------|
|help  |                   |shows all the flags that can be used                                       |
|-a    | --all             |write counts for all files, not just directories                           |
|-b    | --bytes           |present real number of bytes: for data (files) or allocated (directories)  |
|-B    | --block-size=SIZE |define block size in bytes for representations purposes                    |
|-l    | --count-links     |count sizes many times if hard linked                                      |
|-L    | --dereference     |dereference all symbolic links                                             |
|-S    | --separate-dirs   |do not include size of subdirectories                                      |
|      | --max-depth=N     |print information limited to N (0,1, ...) levels of directories depth      |

## Implemented features
- Process creation and handling
- Pipes
- Signal Handlers
- Makefile
- LogFile

## How to run

```
$ cd simpledu
  
$ make
 
$ ./simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]
```

## Autors

- Deborah Lago (<a href="https://github.com/deborahlago">@deborahlago</a>)
- Ricardo Nunes (<a href="https://github.com/TitanicThompson1">@TitanicThompson1</a>)
- Rodrigo Reis  (<a href="https://github.com/rodykings">@rodykings</a>)
