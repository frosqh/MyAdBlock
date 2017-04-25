#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyAdBlock.h"


int parse(char *ENTREE, char *SORTIE, char *SORTIEEX, char *SORTIEEXACTE, char *SORTIEEEX){
	FILE *f_in, *f_out, *f_outex, *f_outexacte, *f_outeex;
	char temp[500000];
	char c, c2;
	int isEx = 0, i = 0;
	int isInSix = 0, isInTwelve = 0;
	int justInSix = 0;
	int isEex = 0;
	int ignore = 0;
	int isExacte = 0;
	int ig = 0;

	if ((f_in = fopen(ENTREE, "r")) == NULL){
		fprintf(stderr, "\nErreur : Impossible de le lire le fichier %s \n", ENTREE);
		exit(EXIT_FAILURE);
	}

	if ((f_out = fopen(SORTIE,"w")) == NULL){
      fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n", SORTIE);
      exit(EXIT_FAILURE);
    }

    if ((f_outex = fopen(SORTIEEX,"w")) == NULL){
      fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n", SORTIEEX);
      exit(EXIT_FAILURE);
    }


    if ((f_outexacte = fopen(SORTIEEXACTE,"w")) == NULL){
      fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n", SORTIEEX);
      exit(EXIT_FAILURE);
    }

    if ((f_outeex = fopen(SORTIEEEX,"w")) == NULL){
      fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n", SORTIEEX);
      exit(EXIT_FAILURE);
    }

	bzero((char*) temp, 500000);

	out:
    while((c = fgetc(f_in)) != EOF){
    	temp[strlen(temp)] = c;
    	//printf("%c", c);
    	if (c == '\n' || c=='|' || c=='^' || c=='$'){
    		if(strlen(temp)>2)
    			temp[strlen(temp)-1]='\n';
    		//printf("%s\n", temp);
    		//sleep(1);
    		if (c == '|'){
    			if (isInSix){
    				if (strlen(temp)==2){
    					goto out;
    				}
    				goto normal;
    			}else{
    				if (strlen(temp) > 2)
    					temp[strlen(temp)-1] = '|';
    					goto out;
    				isInSix = 1;
   					bzero((char*) temp, 500000);
    			}
    		}
    		else{
    			if (c=='^'){
    				if (temp[0] == '|' && temp[1] =='|')
    					goto normal;
    			}
    			else{
    				if (c == '$'){
    					bzero((char*) temp, 500000);
    					goto normal;
    				}
    				else{
    					normal:
						temp[strlen(temp)]=0;
    					if (!(temp[0] == '!' || (temp[0] == '#' && temp[1] == '#') || temp[0] == '[')) {
    						//printf("On est passé :P \n");
    						if (temp[0] == '@' && temp[1] == '@'){
    							/*printf("C'est une exception, bitches :p\n");
    							printf("%s\n", temp);
    							sleep(1);*/
    							isEx = 1;
    							if (temp[2] == '|' && temp[3] =='|'){
    								isEex = 1;
    								isEx = 0;
    							}
    						}
    						else{
    							if (temp[0] == '|' && temp[1] == '|'){
    								isExacte = 1;
    							}
    							isEx = 0;
    						}
	    					while (i < strlen(temp)){
	    						c2 = temp[i];
	    						//printf("%c", c2);
	    						if (!ignore){
	    							if (isEex){
	    								if (i>3)
	    								fputc(c2, f_outeex);
	    							}
	    							else{
			  							if (isEx){
			  								if (i>1){
			  									fputc(c2, f_outex);
			  								}
			  							}
			  							else{
			  								if (isExacte){
			  									if (i>1){
			  										fputc(c2, f_outexacte);
			  									}
			  								}
			  								else{
												putc(c2, f_out);
			  								}
			  							}
			  						}
			  					}
	  							i++;
	    					}
	    				}
	    				else {
	    					//printf("Bat les steaks \n");
	    				}
	    				bzero((char*) temp, 500000);
	    				i = 0;
	    				isEx = 0;
	    				isInTwelve = 0;
	    				isExacte = 0;
	    				if (ig){
	    					ignore = 1;
	    				}
	    				else{
	    					ignore = 0;
	    				}
	    				ig = 0;
	    				isEex = 0;
	    				isInSix = 0;
	    				justInSix = 0;
    				}
    			}
    		}
    	}
    	
    }
    fclose(f_in);
    fclose(f_out);
    fclose(f_outex);
    fclose(f_outexacte);
    return(0);
}