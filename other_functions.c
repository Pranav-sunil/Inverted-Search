#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<ctype.h>
#include "definitions.h"

Mnode *create_main_node( char *word )
{
	/* Function to create a new main node */

	Mnode *new_main_node = malloc( sizeof( Mnode ));
	//Validating if memory has been allocated properly
	if( new_main_node == NULL )
	{
		printf("ERROR: Unable to allocate memory for main node\n");
		return NULL;
	}

	//Updating values to the new_main_node
	strcpy( new_main_node->word, word );
	new_main_node->file_count = 1;
	new_main_node->main_link = NULL;
	new_main_node->sub_link = NULL;

	return new_main_node;
}

Snode *create_sub_node( char *f_name )
{
	/* Function to create a sub node to the main node */

	Snode *new_sub_node = malloc ( sizeof( Snode ));

	//Validating if memory has been allocated
	if( new_sub_node == NULL )
	{
		printf("ERROR: Unable to allocate memory for sub node\n");
		return NULL;
	}

	//Updating values the new sub node
	new_sub_node->word_count = 1;
	strcpy( new_sub_node->file_name, f_name );
	new_sub_node->sub_flink = NULL;

	return new_sub_node;
}

Status display_database( Mnode **array )
{
	/* Function to display the database */

	table_header();

	printf("\n");
	
	//Using a loop to iterate through each index of the hash table
	for( int i = 0 ; i < 26 ; i++ )
	{
		//Check if the index is empty
		if( array[i] == NULL )
		{
			//If the index is empty skip
			//printf("Index %d for [%c] is empty\n", i, (char)(i+65));
			continue;
		}

		Mnode *temp = array[i];
		//If the index is not empty, print the data
		printf("[%c]\t", (char)(i + 65 ));
		printf("%-20s", temp->word);
		printf("%d\t\t", temp->file_count );

		//Check the sub nodes for filenames and word count in each file
		Snode *stemp = temp->sub_link;
		printf("Appears in %s %d times\t", stemp->file_name, stemp->word_count);
		if( stemp->sub_flink == NULL ) { printf("\n"); }

		//Check for further sub nodes
		if( stemp->sub_flink != NULL )
		{
			//Use a pointer and travers the sub nodes
			Snode *subtemp = stemp->sub_flink;
			while( subtemp != NULL )
			{
				printf("%s %d times\t", subtemp->file_name, subtemp->word_count );
				subtemp = subtemp->sub_flink;
			}
			printf("\n");
		}

		//Check if there are any further main nodes in the same index
		if( temp->main_link != NULL )
		{
			//Use a temporary pointer and print the data in the subsequent main nodes
			Mnode *mtemp = temp->main_link;
			while( mtemp != NULL )
			{
				//printf("        ");
				printf("\t%-20s", mtemp->word);
				printf("%d\t\t", mtemp->file_count );

				//Check its sub nodes
				Snode *subtemp = mtemp->sub_link;
				printf("Appears in %s %d times\t", subtemp->file_name, subtemp->word_count);
				if( subtemp->sub_flink == NULL ) { printf("\n"); }

				//Check for subsequent sub nodes
				if( subtemp->sub_flink != NULL )
				{
					//Use a temp pointer and traverse the sub nodes
					Snode *temps = subtemp->sub_flink;
					while( temps != NULL )
					{
						printf("%s %d times\t", temps->file_name, temps->word_count );
						temps = temps->sub_flink;
					}
					printf("\n");
				}
				//Update mtemp
				mtemp = mtemp->main_link;
			}
		}
	}

	table_foot();

	return e_success;
}

int my_strcmp( char *str1, char *str2 )
{
	/* Function to compare strings irrespective of case */

	char c1, c2;
	while( *str1 && *str2 )
	{
		c1 = tolower(*str1);
		c2 = tolower(*str2);
		if( (c1 - c2)== 0 )
		{
			str1++;
			str2++;
		}
		else
		{
			return ( (c1) - (c2) );
		}
	}
	return 0;
}

Status search_db( Mnode **array, char *key )
{
	/* Function to serach the data base for a word input by the user */

	int index;

	//Obtaining the index
	if( key[0] >= 97 && key[0] <= 122 )
	{
		index = key[0] - 97;
	}
	else if( key[0] >= 65 && key[0] <= 90 )
	{
		index = key[0] - 65;
	}

	//Check if the required index is empty
	if( array[index] == NULL ) {
		return e_failure; }

	//If not empty take a temporary pointer and search the main nodes
	Mnode *main_temp = array[index];

	while( main_temp != NULL )
	{
		printf("Word : %s  Key : %s\n", main_temp->word, key );
		//Compare the word in the node
		if( my_strcmp(main_temp->word, key ) == 0 )
		{
			table_header();
			//If word was found, print details
			printf("[%c]\t", (index+65) );
			printf("[%s]\t", main_temp->word );
			printf("%d\t", main_temp->file_count );

			//Use a temporary pointer and print sub nodes
			Snode *sub_temp = main_temp->sub_link;
			while( sub_temp != NULL )
			{
				printf("%s  %d time(s)\t", sub_temp->file_name, sub_temp->word_count );
				sub_temp = sub_temp->sub_flink;
			}
			printf("\n");
			table_foot();
			return e_success;
		}

		main_temp = main_temp->main_link;
	}

	//If word was not found, return failure
	return e_failure;
}

void table_header( void )
{
	printf("-------------------------------------------------------------------------------------------------------\n");
	printf("Index\tWord\t\t   File Count\tFiles\n\n");
	printf("-------------------------------------------------------------------------------------------------------\n");
}

void table_foot( void )
{
	printf("-------------------------------------------------------------------------------------------------------\n\n");
}

Status save_data_base( Mnode **array, char *f_name )
{
	/* Function to save the data base to a text file */

	//Open the file and validate
	FILE *fptr = fopen( f_name, "w" );
	if( fptr == NULL )
	{
		printf("ERROR: Unable to create and open %s\n", f_name );
		return e_failure;
	}

	int i;

	//Use a loop to iterate through each index of the array
	for( i = 0 ; i < 26 ; i++ )
	{
		//Validate if the current index has some data
		if( array[i] !=  NULL )
		{
			//Use a temporary pointer and traverse the main nodes
			Mnode *mtemp = array[i];

			while( mtemp != NULL )
			{
				fprintf(fptr, "%c", '#');
				fprintf(fptr, "%d", i);
				fprintf(fptr, "%c", ';' );
				fprintf(fptr, "%s", mtemp->word );
				fprintf(fptr, "%c", ';' );
				fprintf(fptr, "%d", mtemp->file_count );
				fprintf(fptr, "%c", ';' );

				//Use a temporary pointer and fetch details of sub nodes
				Snode *stemp = mtemp->sub_link;

				while( stemp != NULL )
				{
					fprintf(fptr, "%s", stemp->file_name );
					fprintf(fptr, "%c", ';' );
					fprintf(fptr, "%d", stemp->word_count);
					fprintf(fptr, "%c", ';' );
					stemp = stemp->sub_flink;
				}

				fprintf(fptr, "%c", '#');
				fprintf(fptr, "%c", '\n');

				mtemp = mtemp->main_link;
			}
		}
	}
	//At the end close the file

	fclose( fptr );

	return e_success;	
}

Status updata_database( Mnode **array, FNnode **head, FNnode **up_head , char *fname)
{
	/* Function to update the database from a saved file */

	//Open the file and validate
	FILE *save_file = fopen( fname, "r");
	if( save_file == NULL )
	{
		printf("ERROR: Uable to open the save file\n");
		return e_failure;
	}

	if( validate_update_file( save_file ) != e_success )
	{
		return e_failure;
	}

	char *word, *file_name; 
	char line[500];
	int fcount, wcount, i, index;
	//Use a loop and read data from file, line by line
	while( fscanf( save_file, "%[^\n]\n", line ) != EOF )
	{

		index = atoi( strtok( line, "#,;"));

	//	printf("Index : %d\t", index);

		word = strtok( NULL, "#,;");

		//Check if the index is empty
		if( array[index] == NULL )
		{
			//Create new main node directly
			array[index] = create_main_node( word );
			if( array[index] == NULL ) 
			{ 
				printf("ERROR: Main Node not created\n");
				return e_failure;
			}

			//Update file count
			fcount = atoi( strtok( NULL, "#,;"));
			array[index]->file_count = fcount;

		//	printf("Word : %s\t", word);
		//	printf("File count : %d\n", fcount);

			for( i = 0 ; i < fcount ; i++ )
			{
				file_name = strtok( NULL, "#,;");
				wcount = atoi( strtok( NULL, "#,;"));

				if( updated_file_list( file_name, up_head ) == e_failure )
				{
					printf("ERROR: File list not created\n");
					return e_failure;
				}
				//Check if the sub node is empty
				if( array[index]->sub_link == NULL )
				{
					array[index]->sub_link = create_sub_node( file_name );
					if( array[index]->sub_link == NULL ) 
					{ 
						printf("ERROR: Sub Node not created\n");
						return e_failure;
					}
					array[index]->sub_link->word_count = wcount;
				}
				else
				{
					//Use a temporary pointer and create a sub node for the file name
					Snode *sub_temp = array[index]->sub_link;
					Snode *prev_sub = array[index]->sub_link;

					while( sub_temp != NULL )
					{
						prev_sub = sub_temp;
						sub_temp = sub_temp->sub_flink;
					}

					//At the end, create new sub node
					prev_sub->sub_flink = create_sub_node( file_name );
					if( prev_sub->sub_flink == NULL ) 
					{ 
						printf("ERROR: Sub Node not created\n");
						return e_failure;
					}
					prev_sub->sub_flink->word_count = wcount;
				}
			}
		}
		else
		{
			//Create a temporary pointer and create main nodes and sub nodes
			Mnode *main_temp = array[index];
			Mnode *prev_main = array[index];

		//	printf("Word : %s\t", word);
		//	printf("File count : %d\n", fcount);

			//Traverse the main nodes and create new node at the end
			while( main_temp != NULL )
			{
				prev_main = main_temp;
				main_temp = main_temp->main_link;
			}

			//Create the main node
			prev_main->main_link = create_main_node( word );
			if( prev_main->main_link == NULL ) //Validate if node was created 
			{ 
				printf("ERROR: Main Node not created\n");
				return e_failure;
			}
			main_temp = prev_main->main_link;

			//Update file count
			fcount = atoi( strtok( NULL, "#,;"));
			main_temp->file_count = fcount;

			for( i = 0 ; i < fcount ; i++ )
			{
				file_name = strtok( NULL, "#,;");
				wcount = atoi( strtok( NULL, "#,;"));
				if( updated_file_list( file_name, up_head ) == e_failure )
				{
					printf("ERROR: File list not created\n");
					return e_failure;
				}

				//Check if the sub node is empty
				if( main_temp->sub_link == NULL )
				{
					main_temp->sub_link = create_sub_node( file_name );
					if( main_temp->sub_link == NULL ) 
					{ 
						printf("ERROR: Sub Node not created\n");
						return e_failure;
					}
					array[index]->sub_link->word_count = wcount;
				}
				else
				{
					//Use a temporary pointer and create a sub node for the file name
					Snode *sub_temp = main_temp->sub_link;
					Snode *prev_sub = main_temp->sub_link;

					while( sub_temp != NULL )
					{
						prev_sub = sub_temp;
						sub_temp = sub_temp->sub_flink;
					}

					//At the end, create new sub node
					prev_sub->sub_flink = create_sub_node( file_name );
					prev_sub->sub_flink->word_count = wcount;
				}
			}
		}
	}

	fclose( save_file );

	if( remove_duplicate( head, up_head) == e_failure )
	{
		printf("ERROR: Unable to remove file name from list\n");
		return e_failure;
	}
	
	return e_success;
}

Status updated_file_list( char *file_name, FNnode **up_head )
{
	/* Function to check and create linked list of file names
	   obtained while updating database */

	//Check if the list is empty
	if( *up_head == NULL )
	{
		//Create the node directly and validate
		FNnode *new = malloc( sizeof( FNnode ) );
		if( new == e_failure )
		{
			return e_failure;
		}

		//Update file name;
		strcpy( new->file_name, file_name );
		new->link = NULL;

		//Update head with the address of the new node
		*up_head = new;
		return e_success;
	}

	//If the list is not empty, check the nodes and search if file name exists
	FNnode *temp = *up_head;
	FNnode *prev = *up_head;
	while( temp != NULL )
	{
		//Check if the file exists in the nodes
		if( my_strcmp( temp->file_name, file_name ) == 0 )
		{
			return e_success;
		}

		prev = temp;
		temp = temp->link;
	}

	//If the file name was not found, create the node at the end
	FNnode *new = malloc( sizeof( FNnode ) );
	if( new == e_failure )
	{
		return e_failure;
	}

	//Update file name;
	strcpy( new->file_name, file_name );
	new->link = NULL;

	//Link the new node to the list
	prev->link = new;
	return e_success;

}

Status remove_duplicate( FNnode **head, FNnode **up_head )
{
	/* Function to remove file name nodes that are present in
	   the file list created from CLA based on file list from update database */

	//Check if the lists are empty
	if( *head == NULL )
	{
		return e_failure;
	}
	if( *up_head == NULL )
	{
		printf("ERROR: Update list is empty\n");
		return e_failure;
	}

	//Use a temporary pointer to take each element of update head and compare
	FNnode *update = *up_head;

	while( update != NULL )
	{
		FNnode *cla_temp = *head;
		FNnode *cla_prev = *head;

		//Check if the filename is in first node
		if( my_strcmp( update->file_name, cla_temp->file_name ) == 0 )
		{
			*head = cla_temp->link;
			free( cla_temp );
		}

		//If not, traverse the list and compare each nodes
		while( cla_temp != NULL )
		{
			if( my_strcmp( update->file_name, cla_temp->file_name ) == 0 )
			{
				cla_prev->link = cla_temp->link;
				free( cla_temp );
			}
			cla_prev = cla_temp;
			cla_temp = cla_temp->link;
		}

		update = update->link;
	}

	return e_success;
}

Status validate_update_file( FILE *fptr )
{
	/* Function to check if the file given for updating data base
   		is valid and of proper format	*/

	//Check if the file is empty
	fseek( fptr, 0, SEEK_END );
	if( ftell( fptr ) == 0 )
	{
		printf("ERROR: File is empty\n");
		return e_failure;
	}

	rewind( fptr );

	char line[500];
	//Check if each line has # at the beginning and end
	while( fscanf( fptr, "%[^\n]\n", line ) != EOF )
	{
		if( line[0] != '#' || line[strlen( line )-1] != '#' )
		{
			printf("ERROR: File is not of proper format\n");
			return e_failure;
		}
	}

	rewind( fptr );
	return e_success;
}
