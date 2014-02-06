

#include "ini.h"
#include "constantes.h"


/**************************************************************************************************
	Equivalent à strcmp
**************************************************************************************************/
int same_string(const char* str1, const char* str2)
{
    int i = 0;
	
    while(		str1[i] != '\0' 
			&& 	str2[i] != '\0')
    {
        if (str1[i] != str2[i])	{ return FALSE;}
		i++;
    }
	
    if(str1[i] == str2[i]) 		{ return TRUE; }
	else						{ return FALSE;}
}



/**************************************************************************************************
    Verifie l'existence puis ouvre le fichier dont le nom est passé en parametre
    Puis le nettoie (voir ini_clean_file)
**************************************************************************************************/
FILE * ini_open(const char* name)
{
	FILE * ini = NULL;
	ini = fopen(name, "r+");

	if(ini == NULL)
	{
		printf("Fichier %s inexistant\n", name);
	}
	else
	{
		printf("Fichier %s ouvert avec succes\n", name);
		ini = ini_clean_file(ini, name);
		printf("Fichier nettoye :\n");
		/** affiche_fichier(ini); */
	}

	return ini;
}



/**************************************************************************************************
	Affiche le contenu d'un fichier
**************************************************************************************************/
void affiche_fichier(FILE* fichier)
{
	char string[LINE_LENGTH];
	rewind(fichier);

	printf("\n\n--------------> *\n");

	while(fgets(string, LINE_LENGTH, fichier) != NULL)
	{
			printf("                * ");
			printf("%s", string);
	}

	printf("<-------------- *\n\n\n");
}



/***************************************************************************************************
Ferme un fichier 
***************************************************************************************************/
void ini_close(FILE* ini)
{
	fclose(ini);
}



/**************************************************************************************************
Trouve le nom de la section de la chaine original (enleve les [ ]) 
**************************************************************************************************/
char* getSectionName(const char* original)
{
	int i = 1;
	char* sectName = calloc(LINE_LENGTH, sizeof(char));

	while (original[i] != ']' && i< LINE_LENGTH-1)
	{
		sectName[i-1] = original[i];
		i++;
	}
	sectName[i] = '\0';

	return sectName;
}



/**************************************************************************************************
    Trouve le nom de la clé dans original
    Gere les espaces / tab avant le =
**************************************************************************************************/
char* getParameterName(const char* original)
{
	int i = 0, j = 0;
	char* param = NULL;
	while(original[j] == ' ' || original[j] == '\t' ) j++;
	for(i = 0; i < strlen(original) && original[i] != '=' ; i++);

	while(original[i - 1] == ' ' || original[i - 1] == '\t' )
	{
		i--;
	}

	param = calloc(strlen(original) + 1 , sizeof(char));

	strncpy(param, &original[j], i);
	param[i] = '\0';
	return param;
}



/**************************************************************************************************
    Retourne la valeur en double de original
    Trouve l'indice de la chaine ou il y a un =
    Appelle atof() sur la case d'apres
**************************************************************************************************/
double getValueDouble(const char* original)
{
	char* tmp = strchr(original, '=');
	return atof(&tmp[1]);
}



/**************************************************************************************************
    Retourne un tableau de double en fonction de original
    Meme principe que précedemment
    Nouveau nombre à chaque caractere SEPARATEUR
        -->ini_clean_file enleve les SEPARATEUR en fin de ligne pour plus de sécurité
**************************************************************************************************/
double* getValuesDouble(const char* original, int nb_values)
{
	double* values = calloc(nb_values, sizeof(double));
	int i = 0;
	int j = 0;

	while(		(i < LINE_LENGTH)
			&& 	(j < nb_values)
		  	&& 	(original[i]!= '\n'))
	{
		values[j] = atof(&original[i]);
		j++;

		if (j == nb_values) {return values;}

		while (  (original[i] != SEPARATEUR)
			  && (original[i] != '\n'))
		{
			i++;
		}
		i++;
	}
	return values;
}



/**************************************************************************************************
    Trouve la chaine après le '=' dans original
    Duplique le résultat dans dest
**************************************************************************************************/
char* getValueString(const char* original, char* dest)
{
	char* tmp = strchr(original, '=');
	int i = 1;
	int j = 0;

	while(  (tmp[i] == ' ' ) 
		 || (tmp[i] == '\t')) { i++; }

	while(	 tmp[i] != '\n'
		  && tmp[i] != EOF
		  && tmp[i] != '\t'
		  && tmp[i] != '\r'
		  && i < LINE_LENGTH-1)
	{
		dest[j] = tmp[i];
		i++;
		j++;
	}
	dest[j] = '\0';

	return dest;
}



/**************************************************************************************************
    Déplace le curseur du fichier à la premiere ligne de la section
**************************************************************************************************/
int goto_section(FILE* iniFile, const char* section)
{
	char string[LINE_LENGTH];
	rewind(iniFile);

	while(fgets(string, LINE_LENGTH, iniFile) != NULL)
	{
		/*tant qu'on est pas à la fin du fichier */
		if (string[0] != ';')                           
		{
			/* Si la ligne n'est pas un commentaire */
			if( string[0] == '[' )
            {
				/* Si la ligne est une section */
                if(same_string((const char*) getSectionName(string), (const char*) section) == TRUE)
                {
					/* si le nom de la section est le bon */
                    return TRUE;
                }
            }
		}
	}
	return FALSE;
}



/**************************************************************************************************
    Utilisée en principe avec goto_section
    Utilisée pour tester l'existence d'une key
**************************************************************************************************/
int goto_key(FILE* iniFile, const char* parameter)
{
	char string[LINE_LENGTH];
	while(fgets(string, LINE_LENGTH, iniFile) != NULL)
	{
		if(string[0] == '[') {return FALSE;}
		else 
		{
			if( string[0] != ';' )
			{
				if( same_string(parameter, getParameterName(string)) == TRUE)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}



/**************************************************************************************************
    Va chercher une valeur simple en double dans un fichier
    Retourne def si la section / key n'existe pas
**************************************************************************************************/
double ini_read_double(FILE* iniFile, char* section, char* key, double def)
{
	if(	goto_section(iniFile, section) == TRUE)
	{
		double value;
		char string[LINE_LENGTH];
		while(		fgets(string, LINE_LENGTH, iniFile) != NULL 
				&& 	string[0] != '[') 				
		{
			/*Tant qu'on est pas à la fin di fichier */
		    if( string[0] != ';' && same_string(getParameterName(string), key) == TRUE)
			{
				/* Si on se trouve à la bonne clé */
				value = getValueDouble(string);
				printf("%s - %s : OK --> %.2f\n", section, key, value);
				/*on retourne la valeur */
				return value;
			}
		}
		printf("%s - %s : Parametre inexistant --> Default : %.2f\n", section, key, def);
	}
	else { printf("%s - %s : Section inexistante\n", section, key); }

	/* La clée n'a pas été trouvée: 
	On retourne la valeur de défault */
	return def;
}



/**************************************************************************************************
    Va chercher une chaine dans un fichier
    Retourne def si la section / key n'existe pas
**************************************************************************************************/
char* ini_read_string(FILE* iniFile, char* section, char* key, char* def)
{
	char* value  = NULL;
	char string[LINE_LENGTH];

	value = calloc(LINE_LENGTH,sizeof(char)); value[0]  = '\0';
	
	if(	goto_section(iniFile, section) == TRUE)
	{
		/* Si la section existe bel et bien */
		while(fgets(string, LINE_LENGTH, iniFile) != NULL && string[0] != '[')
		{
			/* si on est pas à la fin du fichier */
			if( string[0] != ';' && same_string(getParameterName(string), key) == TRUE)
			{
				/* si on est à la bonne key */
				value = getValueString(string,value);
				printf("%s - %s : OK --> %s\n", section, key, value);
				return value;
			}
		}
		printf("%s - %s : Parametre inexistant\n", section, key);
	}
	else
	{
		printf("%s - %s : Section inexistante\n", section, key);
	}
	free(value);
	
	/*Sinon retourne la valeur par défaut*/
	return def;													
	
}



/**************************************************************************************************
    Va chercher un tableau de valeurs en double dans un fichier
    Retourne def si la section / key n'existe pas
    Modifie taille pour mettre dedans le nombre de valeurs relevées
**************************************************************************************************/
double* ini_read_array  (FILE* iniFile, char* section, char* key, double* def, int * taille)
{
	double* values  = NULL;
    char* string    = malloc(LINE_LENGTH * sizeof(char));
	int i           = 0;
	int nb_values   = 1;

	if(	goto_section(iniFile, section) == TRUE)
    {
        while(fgets(string, LINE_LENGTH, iniFile) != NULL && string[0] != '[')
        {
            if((string[0] != ';')
            && (same_string(getParameterName(string), key) == TRUE))
            {
				/* Si c'est la bonne key */
                string = getValueString(string, string);

                for(i = 0; i < strlen(string); i++)
                {
                    if(string[i] == SEPARATEUR)
                        nb_values++;
                    if(string[i] == NIL)
                    {
                        nb_values=0;
                        *taille = 0;
                        printf("%s - %s : Liste Vide\n", section, key);

                        free(string);
                        return NULL;
                    }
                }
                values = getValuesDouble(string, nb_values);
                *taille = nb_values;
                free(string);
                printf("%s - %s : OK --> [%d] ", section, key, nb_values);

                for(i = 0; i < nb_values; i++) {
                    printf("%.2f  ", values[i]);
                }
                printf("\n");
                return values;
            }
        }
        printf("%s - %s : Parametre inexistant\n", section, key);

    }
    else
    {
        printf("%s - %s : Section inexistante\n", section, key);
    }

    /* Sinon retourne la valeur par défaut */
    free(string);
    return def;
}



/**************************************************************************************************
    Ajoute une section à la fin du fichier
    Teste son existence avant
**************************************************************************************************/
FILE* ini_add_section (FILE* iniFile, char* section) 
{
	if(goto_section(iniFile, section) == FALSE) 
	{
		fseek(iniFile, 0, SEEK_END);
		if(ftell(iniFile) == 0)
			fputs("[", iniFile);
		else
			fputs("\n[", iniFile);
		fputs(section, iniFile);
		fputs("]\n", iniFile);
	}
	return iniFile;
}



/**  Les fonctions suivantes ajoutent / remplacent / enlevent des éléments au milieu des fichiers   **/
/**  A chaque fois, les fichiers seront copiés, corrigés, puis les anciens seront détruits          **/
/**  Les nouveaux fichiers auront le meme noms que les anciens                                      **/
/**  Une alternative aurait été de copier tout dans une string puis de le copier simplement...      **/



/**************************************************************************************************
    Ajoute une valeur dans le fichier
    @section et @key
**************************************************************************************************/
FILE* ini_add_value (FILE* iniFile, char* ini_name, char* section, char* key, char* value)
{
	char 	backup_name[LINE_LENGTH];
	char 	string[LINE_LENGTH];
	FILE* 	newFile 		= NULL;
	char*  	actual_section;
	int 	added;
		
	if(goto_section(iniFile, section) == FALSE)
	{
		ini_add_section(iniFile, section);
		fputs(key, iniFile);
		fputc('=',iniFile);
		fputs(value, iniFile);
		fputc('\n', iniFile);
		return iniFile;
	}
	else
	{
		rewind(iniFile);
		strcpy(backup_name,ini_name);
		strcat(backup_name, ".bak");

		fclose(iniFile);
		rename(ini_name, backup_name);
		iniFile = fopen(backup_name, "r");
		/*FILE* */ newFile = fopen(ini_name, "w+");

		/*char* */ actual_section = "SECTION";
		/*int*/ added = 0;

		while(fgets(string, LINE_LENGTH, iniFile)) 
		{
			if(!added) 
			{
				if(string[0] == '[' && string[strlen(string)-2] == ']')
				{
					actual_section = getSectionName(string);
				}

				if(same_string(section, actual_section) == TRUE)
				{
					fputs(string, newFile);
					fputs(key, newFile);
					fputc('=',newFile);
					fputs(value, newFile);
					fputc('\n', newFile);
					added = 1;
				}
				else 
				{
					fputs(string, newFile);
				}
			}
			else 
			{
				fputs(string, newFile);
			}
		}
		fclose(iniFile);
		remove(backup_name);
		return newFile;
	}
}



/**************************************************************************************************
    Remplace une valeur dans un fichier
    @section @key
**************************************************************************************************/
FILE* ini_replace_value (FILE* iniFile, char* ini_name, char* section, char* key, char* replacement) 
{
	char backup_name[LINE_LENGTH];
	char string[LINE_LENGTH];
	
	FILE* newFile = NULL;
	char* actual_section = NULL;
	char*  actual_key = NULL;
	int replaced = 0;

	strcpy(backup_name,ini_name);
	strcat(backup_name, ".bak");

	fclose(iniFile);
	rename(ini_name, backup_name);
	iniFile = fopen(backup_name, "r");
	newFile = fopen(ini_name, "w+");

	actual_section = "SECTION";
	actual_key = "KEY";


	while(fgets(string, LINE_LENGTH, iniFile)) 
	{
		if(!replaced) 
		{
			if(string[0] == '[' && string[strlen(string)-2] == ']') {actual_section = getSectionName(string);}
			else if (string[0] != ';') 								{actual_key = getParameterName(string);}

			if(same_string(key, actual_key) == TRUE && same_string(section, actual_section) == TRUE) 
			{
				fputs(key, newFile);
				fputc('=',newFile);
				fputs(replacement, newFile);
				fputc('\n', newFile);
				replaced = 1;
			}
			else 	{ fputs(string, newFile); }
		}
		else		{ fputs(string, newFile); }
	}

	fclose(iniFile);
	remove(backup_name);
	
	if(!replaced) { printf("Valeur non trouvee, pas de remplacement effectue\n"); }
	return newFile;
}



/**************************************************************************************************
    Décide si il faut utiliser add ou replace
    En fonction de si la section / key existe
**************************************************************************************************/
FILE* ini_update(FILE* iniFile, char* ini_name, char* section, char* key, char* value) {
	if(goto_section(iniFile, section) == TRUE)
		if(goto_key(iniFile, key) == TRUE)
			return ini_replace_value(iniFile,ini_name, section, key, value);

	return ini_add_value(iniFile,ini_name, section, key, value);
}


/**************************************************************************************************
    Enleve TOUS les \r en fin de ligne (pour les fichiers créés sous Windows incompatibles sous linux \o/)
    Enleve tous les caractère SEPARATOR, espaces et tab en fin de ligne
**************************************************************************************************/
FILE* ini_clean_file(FILE* iniFile, const char* ini_name)
{
	char backup_name[LINE_LENGTH];
	char string[LINE_LENGTH];
	char dumpLC = 0;
	int i, id_last_char = 0;
	FILE* newFile = NULL;
	
	strcpy(backup_name,ini_name);
	strcat(backup_name, ".bak");

	fclose(iniFile);
	rename(ini_name, backup_name);
	iniFile = fopen(backup_name, "r");
	newFile = fopen(ini_name, "w+");
	


	while(fgets(string, LINE_LENGTH, iniFile))
	{
		id_last_char = 0;
		for (i = 0; i < LINE_LENGTH-2; i++)
		{
			if (string[i] != ' ' && string[i] != '\n' && string[i] != '\r' && string[i] != SEPARATEUR)
			{
				id_last_char = i;
			}
			if (string[i] == '\r') {printf(".");}
			if (string[i] == '\n')
			{
				break;
			}
		}
		if (i!=0)
		{
			string[id_last_char+1] = '\n';
			string[id_last_char+2] = '\0';
		}
		else
		{
			string[0] = '\n';
			string[1] = '\0';
		}
		dumpLC = string[strlen(string) - 1];
		fputs(string, newFile);
	}

	if (dumpLC != '\n')
	{
		fseek(newFile, 0, SEEK_END);
		fputc('\n', newFile);
	}

	fclose(iniFile);
	remove(backup_name);

	return newFile;
}



/**************************************************************************************************
    Retourne le nombre de sections dans un fichier ini
**************************************************************************************************/
int get_nb_sections(FILE* iniFile) 
{
	int count = 0;
	char string[LINE_LENGTH];
	rewind(iniFile);
	
	while(fgets(string, LINE_LENGTH, iniFile) != NULL) 
	{
		if(string[0] == '[')
		{
			count++;
		}
	}
	return count;
}



/**************************************************************************************************
    Retourne un tableau de string avec le nom de TOUTES les sections du fichier
    Met leur nombre dans nb_sections
**************************************************************************************************/
char** ini_get_ALL_the_sections(FILE* iniFile, int* nb_sections)
{

	int i;
	char string[LINE_LENGTH];
	char** sections = NULL;
	
	*nb_sections = get_nb_sections(iniFile);
	sections = calloc(*nb_sections, sizeof(char*));


	for(i = 0; i < *nb_sections; i++)
		{ sections[i] = calloc(64, sizeof(char));}

	rewind(iniFile);

	i = 0;
	while(fgets(string, LINE_LENGTH, iniFile) != NULL)
	{
		if(string[0] == '[')
		{
			sections[i] = getSectionName(string);
			i++;
		}
	}
	return sections;
}



/**************************************************************************************************
    Retourne le nombre de parametres dans une section
**************************************************************************************************/
int get_nb_keys (FILE* iniFile, const char* section)
{
	int count = 0;
	char string[LINE_LENGTH];

	rewind(iniFile);

	while(fgets(string, LINE_LENGTH, iniFile) != NULL && string[0] != '[')
	{
		if((string[0] != '[')
		&& (string[0] != ';')
		&& (string[0] != '\n'))
		{
			count++;
		}
	}
	return count;
}



/**************************************************************************************************
    Retourne tous les parametres d'une section
    Met leur nombre dans nb_keys
**************************************************************************************************/
char** ini_get_ALL_the_keys(FILE* iniFile, char* section, int* nb_keys) 
{
	int i;
	char string[LINE_LENGTH];
	char** keys = NULL;
	
	*nb_keys = get_nb_keys(iniFile, section);
	keys = calloc(*nb_keys, sizeof(char*));

	for(i = 0; i < *nb_keys; i++)
	{
		keys[i] = calloc(64, sizeof(char));
	}
	
	rewind(iniFile);

	i = 0;
	while(fgets(string, LINE_LENGTH, iniFile) != NULL && string[0] != '[') 
	{
		if(string[0] != '[' && string[0] != ';' && string[0] != '\n') 
		{
			keys[i] = getParameterName(string);
			i++;
		}
	}
	return keys;
}
