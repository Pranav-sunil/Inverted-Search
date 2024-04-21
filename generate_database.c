#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<ctype.h>
#include "definitions.h"

Status create_data_base( FNnode *file_node, Mnode **array )
{
	/* Function to create the database */

	/* Use a temporary pointer to traverse through the file name list and open
	   each file and read data from each file */

	FNnode *file_temp = file_node;

	//Use a loop to traverse the file name list
	while( file_temp != NULL )
	{
		//File pointer to open file and store file stream
		FILE *fptr = fopen( file_temp->file_name, "r" );
		if( fptr == NULL ) //Validate if file opened correctly
		{
			printf("ERROR: File not opened\n");
			return e_failure;
		}

		char word[100]; //To store the word obtained from file
		int index; //To store the index
		
		//Use a loop until fscanf encounters EOF to read words from the file
		while( fscanf( fptr, " %s ", word ) != EOF )
		{
			//Obtain the index to which the word is to be stored
			if( word[0] >= 97 && word[0] <= 122 )
			{
				index = word[0] - 97;
			}
			else
			{
				index = word[0] - 65;
			}

			if( array[index] == NULL )
			{
				//If the required index is empty, directly store the word to that index
				array[index] = create_main_node( word );

				array[index]->sub_link = create_sub_node( file_temp->file_name );
			}
			else
			{
				//If required index is not empty, traverse the main nodes using a loop
				Mnode *main_temp = array[index];
				Mnode *prev_main = NULL;

				int sub_node_flag = 0;
				int main_node_flag = 0;

				while( main_temp != NULL )
				{
					//Check if the word contained in main node pointed by main_temp is same
					if( my_strcmp( main_temp->word, word ) == 0 )
					{
						main_node_flag = 1;
						
						/* Check the sub node and find if the word is in a file that exists 
						   in the data base */

						Snode *sub_temp = main_temp->sub_link;
						Snode *prev_sub = NULL;

						while( sub_temp != NULL )
						{
							if( my_strcmp( sub_temp->file_name, file_temp->file_name ) == 0 )
							{
								/* If the word exists in a file already in database,
									increment word count and break the loop */
								(sub_temp->word_count)++;
								sub_node_flag = 1;
								break;
							}
							
							prev_sub = sub_temp;
							sub_temp = sub_temp->sub_flink;
						}
						if( sub_node_flag == 0 && sub_temp == NULL )
						{
							/* If word is in a file that is not in the database,
							   increment file count and then break the outer while loop */
							(main_temp->file_count)++;
							prev_sub->sub_flink = create_sub_node( file_temp->file_name );
							break;
						}	
					}
					
					prev_main = main_temp;
					main_temp = main_temp->main_link;
				}
				if( main_node_flag == 0 && main_temp == NULL )
				{
					/* If the word does not exist in the index, then create a 
					   main node and sub node for the word */
					prev_main->main_link = create_main_node( word );

					prev_main->main_link->sub_link = create_sub_node( file_temp->file_name );
				}
			}
		}

		file_temp = file_temp->link;
	}

	return e_success;
}
