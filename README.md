# SIMPLEDU

#### SOPE First Project - 10/04/2020

## Description

Simple program that can estimate the size of a *directory* and its *files* in a system. It works like `du` command on **Unix** based systems. However, it is a more simple version of `du` command, so we call it **`simpledu`**. 

![sample image](https://github.com/rodykings/simpledu/blob/master/samples/1.png)

## How it works

Everytime the program finds another directory inside a directory, it creates a new process that is going to handle all the subdirectories and files of that directory. We do recursive calls of the `simpledu` program rearranging the atributes and we do it with `execvp` system call.

**To make conditions and filter information we can use some`flags` that can be combined:**

| Flag | Alternative Flag  | Description                                                               |
|------|-------------------|---------------------------------------------------------------------------|
|-a    | --all             |a informação exibida diz respeito também a ficheiros                       |
|-b    | --bytes           |apresenta o número real de bytesde dados(ficheiros) ou alocados(diretórios)|
|-B    | --block-size=SIZE |define o tamanho (bytes) do bloco para efeitos de representação            |
|-l    | --count-links     |contabiliza múltiplas vezeso mesmo ficheiro                                |
|-L    | --dereference     |segue linkssimbólicos                                                      |
|-S    | --separate-dirs   |a informação exibida não inclui o tamanho dos subdiretórios                |
|      | --max-depth=N     |limita a informação exibida a N(0,1,...)níveis de profundidadede diretórios|


## How to run

```
$ cd simpledu
  
$ make
 
$ ./simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]
```

## Autors

- Deborah Lago
- Ricardo Nunes
- Rodrigo Reis
