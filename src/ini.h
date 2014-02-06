/*
 * ini.h
 *
 *  Created on: 8 oct. 2011
 *      Author: Arnaud
 */

#ifndef INI_H_
#define INI_H_

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


/*
 * PRIVATE
 */
char* 	getSectionName		(const char* original);
char* 	getParameterName	(const char* original);
double 	getValueDouble		(const char* original);
double* getValuesDouble		(const char* original, int nb_values);
char* 	getValueString		(const char* original, char* dest);
int 	goto_section		(FILE* iniFile,  const char* section);
int 	goto_key			(FILE* iniFile,  const char* parameter);
int 	get_nb_sections		(FILE* iniFile);
int 	get_nb_keys 		(FILE* iniFile, const char* section);

/*
 * PUBLIC
 */

char*  	ini_read_string		(FILE* iniFile, char* section, char* key, char* def);
double 	ini_read_double		(FILE* iniFile, char* section, char* key, double def);
double* ini_read_array  	(FILE* iniFile, char* section, char* key, double* def, int * taille);


FILE* 	ini_add_section 	(FILE* iniFile, char* section);
FILE* 	ini_add_value 		(FILE* iniFile, char* ini_name, char* section, char* key, char* value);
FILE* 	ini_replace_value 	(FILE* iniFile, char* ini_name, char* section, char* key, char* replacement);
FILE* 	ini_update			(FILE* iniFile, char* ini_name, char* section, char* key, char* value);

char** 	ini_get_ALL_the_keys	(FILE* iniFile, char* section, int* nb_keys);
char**  ini_get_ALL_the_sections(FILE* iniFile, int* nb_sections);

FILE * 	ini_open		(const char* name);
void 	ini_close		(FILE* ini);
FILE* 	ini_clean_file	(FILE* iniFile, const char* ini_name);
void affiche_fichier	(FILE* fichier);

int same_string(const char* str1, const char* str2);


#endif /* INI_H_ */
