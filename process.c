

void new_process(char* filepath, int depthlevel){


    while((dir_entry = readdir(home))!=NULL){

        char *filename = dir_entry->d_name;

        if(strcmp(filename, ".")==0 || strcmp(filename, "..") ==0){
            continue;
        }

        //LOG
        //printf("Processing %s file in process %d\n", filename, getpid());

        char filepath[300];
        if(sprintf(filepath, "%s/%s", argv[pathPos], filename) < 0){
            printf("Error in sprintf\n");
            log_exit(logFile, 5);
            //exit(5);
        }

        //printf("filepath: %s\n",filepath);

        //Reads file
        if(stat(filepath, &filestat)!=0){
            printf("Error in stat\n");
            log_exit(logFile, 3);
            //exit(3);
        }
        
        if(spcFlags.all && S_ISREG(filestat.st_mode)){
            if(spcFlags.bytes)
                printf("%ld\t%s\n", filestat.st_size,filepath);
            else if(spcFlags.block_size)
                printf("%ld\t%s\n", filestat.st_size/spcFlags.nbytes,filepath);
            else
                printf("%ld\t%s\n", filestat.st_size/1024,filepath);

        }else if(S_ISDIR(filestat.st_mode)){          //Verifies if is directory  
            if((spcFlags.max_depth && spcFlags.depth_level != 0) || spcFlags.max_depth == false){

                printf("ENTREI NO IF COM DEPTH LEVEL %d\n", dp_level);

                signal(SIGINT, signalHandler);

                pid_t pid = fork();
                
                

                putpid(pids, MAX_PIDS, pid);

                if(pid == 0){        //Child process
                    
                    char * new_arg[argc];
                    new_arg[argc] = NULL;
                    
                    for(int i = 0; i < argc; i++)
                        new_arg[i] = malloc(100);
                    

                    copy_values(new_arg,argv,argc);
                    
                    if(sprintf(new_arg[pathPos], "%s/%s", argv[pathPos] ,filename) < 0){
                        printf("sprintf\n");
                        log_exit(logFile, 4);
                        //exit(4);
                    }

                    //write(STDOUT_FILENO,"after sprintf\n",14);
                    
                    /*
                    if(strlen(argv[0]) == 10){
                        new_arg[0] = "../simpledu";
                    }else {
                        sprintf(new_arg[0], "../%s", argv[0]);
                    }
                    */

                    //printf("Going to %s using prog %s with flag %s in process %d\n", new_arg[argc-1], new_arg[0], new_arg[1], getpid());  
                    print_args(new_arg, argc);  
                    if(spcFlags.max_depth)
                        decrease_max_depth(new_arg);
                                          
                    execvp(new_arg[0], new_arg);
                    printf("Error in executing recursive simpledu to %s\n", new_arg[argc-1]);
                    log_exit(logFile, 3);
                    //exit(3);
                }else{              //Parent process

                

                /*   
                    if(putpid(pids, MAX_PIDS, pid)!=0){
                        printf("Maximum number of forks exceeded\n");
                        exit(5);
                    }
                    */
                }

                int ret;
                while ((ret = wait(&status)) > 0); 
                /*
                if(spcFlags.bytes)
                    printf("%ld\t%s\n", filestat.st_size,filepath);
                else if(spcFlags.block_size)
                    printf("%ld\t%s\n", filestat.st_size/spcFlags.nbytes,filepath);
                else
                    printf("%ld\t%s\n", filestat.st_size/1024,filepath);
                */
                //sleep(5);
            }
            
        }
       
       

        /*
        int ret;
        while((ret = waitpid(-1, &status, WNOHANG) != 0) && ret != -1);
        */
        //printf("passei o while ma friends!\n");
        
        //Verifica se algum processo filho acabou
        /*
        it_pid = 0;
        while (pids[it_pid] != (pid_t) -1){
            
            if(pids[it_pid] != (pid_t) 0){
                waitpid(pids[it_pid], &status, WNOHANG);
            }
            it_pid++;
                
        }*/
    }
    
    //Waiting for all childs to end (needs to change!)
    //while(wait(&status)!=-1);
    int ret;
    while ((ret = wait(&status)) > 0);

    
    log_exit(logFile, 0);
}
