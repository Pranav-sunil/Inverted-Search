#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "definitions.h"

Status check_file_type( char *file_name )
{
	/* Function to check if the file passed is a .txt file or not */
	if( strstr( file_name, ".txt") == NULL )
	{	
		printf("ERROR: %s is not a .txt file\n", file_name);
		return e_failure;
	}
	return e_success;
}

Status check_and_open_file( char *file_name , FILE **pointer )
{
	/* Function to check if file exists and open the file */

	*pointer = fopen( file_name, "r");
	
	if( *pointer == NULL )
	{
		return e_failure;
	}

	return e_success;
}

Status check_file_contents( FILE *pointer )
{
	/* Function checks if the file is an empty file */
	//printf("Function called\n");
	fseek( pointer, 0, SEEK_END );
	//printf("SEEK working\n");
	if( ftell( pointer ) == 0 )
	{
		return e_failure;
	}

	return e_success;
}

Status check_duplicate_file( char *file_name , FNnode *head )
{
	/* Function checks if the file has already been passed via command line */

	if( head == NULL )
	{
		//If the list is empty, then file is not a duplicate
		return e_success;
	}

	FNnode *temp = head;
	
	while( temp != NULL )
	{
		if( strcmp( file_name, temp->file_name ) == 0 )
		{
			return e_failure;
		}
		temp = temp->link;
	}
	return e_success;
}

Status create_node_and_update( FNnode **head, char *file_name )
{
	/* Function creates a node and adds to the linked list containg file details */

	FNnode *new = malloc( sizeof( FNnode ) );

	//Check if memory has been allocated
	if( new == NULL )
	{
		return e_failure;
	}

	//Update values to the node
	strcpy( new->file_name, file_name );
	new->link = NULL;

	//Check if the list is empty
	if( *head == NULL )
	{
		//If empty, update head
		*head = new;
		return e_success;
	}

	//If list is not empty, add node to the end of the list

	FNnode *temp = *head;

	while( temp->link != NULL )
	{
		temp = temp->link;
	}

	//At the end of the loop add the node
	temp->link = new;

	return e_success;
}
