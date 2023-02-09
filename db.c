
#include "db.h"

int db_lookup(char*target_key, char*db_value){

    FILE *general;
    int linepos;
    char *status;
    char line[80];
    char db_key[60];

   //  open general knowlege database
    general = fopen("general.txt","r");
    if(general == NULL) return CANT_OPEN_FILE;

    // search for key
    while(1){
	   // get a line
	   status = fgets(line,80,general);
       if (status==0){
    	  fclose(general);
          return NOT_FOUND;
       }

     // get the key
       linepos =  copy_to_delimiter(line, db_key, ':');
       // is it the key we're looking for?
       // if not, get another line

       if(strcmp(target_key, db_key) != 0) continue;
       break;
	} // end of key search

    // get value
    db_copy_word(line, linepos, db_value);

    fclose(general);

    return FOUND;  // (found)

}

int db_add_pair(char*key, char*value){

    FILE *general;

   //open
	  general = fopen("general.txt","a");
	  if(general == NULL) {printf("fopen failed while trying to open general.txt");}

   //add key and value
	  fprintf(general, "%s:%s\n", key, value);

   // close
	  fclose(general);

	  return 1;

}

int db_get_id(char* firstname)
{

   int i;
    char id_string[20];
    char db_name[20];
    char string[20];
    char key[80];
    int result;

    for(i=1; i<100; i++)
    {
        sprintf(clipboard, "#%d", i); // make id string
        // make key
        sprintf(key, "%s > firstname", clipboard);
        result = db_lookup(key, db_name);
          if(result == 0)continue;

        if(strcmp(db_name, firstname) == 0){
            return i;
        }
    }
    return 0; // not in the database
}

int db_change_value(char*key, char*value){
    FILE *general;
    FILE *temp;

    char *status;
    char line[80];
    char db_key[60];

    //open files
   	general = fopen("general.txt","r");
	   if(general == NULL) {
       printf("DB: can't open general.txt");
       return 2;
    }
	temp = fopen("temp.txt","w");
   	if(temp == NULL) {
       printf("DB: can't open temp.txt");
       fclose(general);
       return 2;
    }

    // search for a key, copy each line
    while(1){
	      // get a line
	      status = fgets(line,80,general);
       if (status==0){
          fclose(general);
          fclose(temp);
          return 1;  //(key not found)
	      }

       db_copy_word(line,1, db_key);
       // is it the key we're looking for?
       // if not, get another line
       if(strcmp(key, db_key)==0) {
         fprintf(temp, "%s", line); // copy line to temp file
         break;
       }
    }  // end of subject search

    //add key and value
	   fprintf(temp, "%s: %s\n", key, value);


    // copy remainder of file
    //   fprintf(temp, "%s", line); // copy line to temp file
    while(1){
        // get another line
		     status = fgets(line,80,general);
		     if (status==0) break;
 	        fprintf(temp, "%s", line); // copy line to temp file
       }

    // close
	   fclose(general);
	   fclose(temp);

   	if(remove("general.txt") != 0) {printf("oops, file error ""%s"" \n", strerror(1));}
	   if(rename("temp.txt","general.txt") != 0) {printf("file error ""%s"" \n", strerror(1));}

	   return 0;  // (success)

}

int db_next_available_id(void){

   int i;
   char value[20];
   char string[20];
   int result;
   char key[20];
   char id_string[20];

   for(i=1; i<100; i++){
      // look for id, first_name
    sprintf(clipboard, "#%d", i); // make id string
    strcpy(key, clipboard);
    strcat(key, " > class");
    result = db_lookup(key,value);
      if(result != FOUND){
        return i;
      }
   }

   printf("error in function DNAI");
    return 0;
}
int db_copy_word(char* line, int position, char* value)
{

    int i;

    for(i=0; i<MAX_LETTERS-2; i++)
    {
        //skip over leading spaces and tabs
        if(line[position]==' ') position++;
        if(line[position]==' ') position++;
        if(line[position]==' ') position++;
        if(line[position]=='\t') position++;
        if(line[position]=='\t') position++;
        if(line[position]=='\t') position++;

        if ((isalpha(line[position])==0) && (isdigit(line[position])==0))
        {
            value[i]=0;

            return position;
        }
        else
        {
            value[i]=line[position];
            position++;
        }
    }

    return position;

}

int copy_to_delimiter(char* from, char* to, char delimiter){

   int i;

   for(i=0; i<80; i++){
      if (from[i] == delimiter){
         to[i] = NULL; //
         return i + 1;
      }
      else{
         to[i] = from[i];
         //i++;
      }
   }
return i;
}
int db_root_check(char* startingwith, char* lookingfor){
                      //  animal              creature
   int result;
   int n;
   char subject[60];
   char key[60];
   char value[20];

   strcpy(subject, startingwith);
   for(n=0; n<5; n++){
       strcpy(key, subject); // prepare to lookup
       strcat(key," > class"); // prepare to lookup
       result = db_lookup(key, value); // lookup
       if(result == NOT_FOUND) return NOT_FOUND; // if not in database at all, exit
       if(strcmp(value,lookingfor)==0)return FOUND;  // is it what we're looking for?
       if(strcmp(value,"root")==0)break; // has it reached root?
       strcpy(subject,value); // if no luck so far, the value becomes the subject
   }

  return NOT_FOUND;  // not found

}
