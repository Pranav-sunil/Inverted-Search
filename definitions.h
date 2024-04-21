#ifndef DEFIITIONS_H
#define DEFIITIONS_H

/* Status of function */

typedef enum
{
	e_failure,
	e_success 
} Status;

/* Node structre to store file name */
typedef struct file_name_node
{
	char file_name[100];
	struct file_name_node *link;
}FNnode;

/* Structre for sub-node */
typedef struct sub_node
{
	int word_count;
	char file_name[100];
	struct sub_node *sub_flink;
}Snode;

/* Structre for the main node */
typedef struct main_node
{
	int file_count;
	char word[100];
	struct main_node *main_link;
	struct sub_node *sub_link;
}Mnode;

/* Function to display menu */
void menu( void );

/* Function declerations for Creating database */

/* Function to create a new main node */
Mnode *create_main_node( char *word );

/* Function to create sub node */
Snode *create_sub_node( char *f_name );

/* Function to call other functions and create the database  */
Status create_data_base( FNnode *file_node , Mnode **array );

/* Function Declerationd for displaying database */
Status display_database( Mnode **array );

/* Function declerations for validations */

/* Function to create node */
Status create_node_and_update( FNnode **head, char *file_name );

/* Function to check the type of file passed */
Status check_file_type( char *file_name );

/* Function to check if file exists and open them */
Status check_and_open_file( char *file_name , FILE **pointer );

/* Function to check if the file is empty or not */
Status check_file_contents( FILE *pointer );

/* Function to check if duplicate files has been passed */
Status check_duplicate_file( char *file_name , FNnode *head);

/* String compare function that ignores case */
int my_strcmp( char *str1, char *str2 );

/* Function to search the data base for the word */
Status search_db( Mnode **array, char *key );

/* Function to save the data base */
Status save_data_base( Mnode **array, char *f_name );

/* Function to update the data base */
Status updata_database( Mnode **array, FNnode **head, FNnode **up_head, char *fname);

/* Function to validate the save file */
Status validate_update_file( FILE *fptr );

/* Function to create file list for update condition */
Status updated_file_list( char *file_name, FNnode **up_head );

/* Function to delete duplicate nodes in file list after updating */
Status remove_duplicate( FNnode **head, FNnode **up_head );

void table_header( void );

void table_foot( void );

#endif
