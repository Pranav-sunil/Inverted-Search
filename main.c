/******************************************************************************************
   										Inverted Search

	Name of author   :    Pranav S
	Project Name     :    Inverted Search
	Description      :	  The project is to create database of words in the text files 
						  passed via command line using hash tables. The database created 
						  will have information about in how many files the word appears 
						  and how many times it occurs in each file. The data base can also 
						  be saved to a text file, which can be used to re-create the 
						  database.

	Usage			 :    ./a.out <file1.txt> <file2.txt> <file3.txt>
	GitHub Profile   :    www.github.com/Pranav-sunil
	
*******************************************************************************************/


#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<ctype.h>
#include "definitions.h"

int main(int argc, char *argv[])
{
	//Check if the user has passed sufficent number of command line arguments
	if( argc <= 1 )
	{
		printf("ERROR: Please Pass The Files Through Command Line Arguments\n");
		printf("USAGE: ./a.out <file1.txt> <file2.txt>\n");
		return 0;
	}

	//Create a head pointer to store file names and file pointers
	FNnode *file_node_head = NULL;	
	FNnode *update_file_head = NULL;
	//validate the files
	for( int i = 1 ; i < argc ; i++ )
	{
		if( check_file_type( argv[i] ) == e_success )
		{
			//printf("File type okay\n");
			FILE *file_pointer;
			if( check_and_open_file( argv[i] , &file_pointer) == e_success )
			{
				//printf("File exist\n");
				if( check_file_contents( file_pointer ) == e_success )
				{
					//printf("File is not empty\n");
					if( check_duplicate_file( argv[i], file_node_head ) == e_success )
					{
						//printf("File is not duplicate\n");
						//If file has passed all validations, create a node and store details to it
						if( create_node_and_update( &file_node_head, argv[i] ) == e_failure )
						{
							printf("ERROR: Unable to create and add data to node\n");
							continue;
						}
						else
						{
							printf("SUCCESS: Inserted file %s to the file node\n", argv[i] );
							fclose( file_pointer );
						}
					}
					else
					{
						printf("ERROR: %s has already been passed before\n", argv[i]);
						continue;
					}
				}
				else
				{
					printf("ERROR: %s is an empty file\n", argv[i] );
					continue;
				}
			}
			else
			{
				printf("ERROR: %s does not exist\n", argv[i] );
				continue;
			}
		}
		else
		{
			printf("ERROR: %s is not a .txt file\n", argv[i]);
			continue;
		}
	}
	//For storing the data, create an array of pointers
	Mnode **array;
	array = malloc( 26 * sizeof( Mnode * ) );

	//Check if memory has been allcoated
	if( array == NULL )
	{
		printf("ERROR: Unable to allcoate memory for then database\n");
		return 0;
	}

	for( int i = 0 ; i < 26 ; i++ )
	{
		array[i] = NULL;
	} 

	int db_flag = 0; //Flag to know if database has been created and prevent repeated creations
	int updt_flag = 0; //Flag to know if database has been updated

	//Use switch case for calling functions 
	char  opt;
	int choice;
	do
	{
		menu();
		scanf("%d", &choice);
		getchar();
		switch( choice )
		{
			case 1:
				// To create database
				if( db_flag == 0 )
				{
					if( create_data_base( file_node_head, array ) == e_success )
					{
						printf("Database created successfully\n");
						db_flag = 1;
					}
					else
					{
						printf("ERROR: Unable to create database\n");
					}
				}
				else
				{
					printf("Database already created\n");
				}
				break;
			case 2:
				//To display database
				if( db_flag == 1 || updt_flag == 1 )
			   	{
					display_database( array ); 
				}
				else
				{
					printf("ERROR: Database has not been created\n");
				}
				break;
			case 3:
				//To search
				if( db_flag == 1 || updt_flag == 1 ) 
				{
					char key[100];
					printf("Enter the word to be searched : ");
					scanf("%100s", key );
					getchar();

					if( search_db( array, key ) != e_success ) 
					{ 
						printf("%s was not found in the data base\n", key ); 
					}
				}
				else 
				{
					printf("ERROR: database not yet created\n");
				}
					break;
			case 4:
				//Update the database
					if( db_flag == 0 )
					{
						char ud_file[100];
						printf("Enter the file name to update database : ");
						scanf("%s", ud_file );
						getchar();
						
						//Validate file
						if( strstr( ud_file, ".txt" ) != NULL )
						{
							if( updata_database( array, &file_node_head, &update_file_head, ud_file ) == e_success )
							{
								printf("Database has been sucessfully updated\n");
								updt_flag =1;
							}
							else
							{
								printf("ERROR: Database update is not sucessful\n");
							}
						}
						else
						{
							printf("ERROR: File should be a .txt file\n");
						}
					}
					else
					{
						printf("ERROR: Database has already been created\n");
					}
					break;
			case 5:
				//Save the data base
				if( db_flag == 1 )
				{
					char save_file[100];
					printf("Enter the file name : ");
					scanf("%[^\n]", save_file );
					getchar();

					//Validate
					if( strstr( save_file, ".txt") == NULL )
					{
						printf("ERROR: File should be a .txt file\n");
						break;
					}

					if( save_data_base( array, save_file ) == e_success )
					{
						printf("Successfully saved the database\n");
						break;
					}
					else
					{
						printf("ERROR: Database not stored\n");
						break;
					}
				}
				else
				{
					printf("ERROR: Database not yet created\n");
				}
				break;
			default:
				printf("ERROR: Invalid choice\n");
				break;
		}

		printf("\nDo you want to continue [yY/nN] : ");
		scanf("%c", &opt);
		getchar();
		opt = toupper(opt);
	}while( opt != 'N' );

	return 0;
}

void menu( void )
{
	printf("\nSelect option : \n");
	printf("1. Create Database\n");
	printf("2. Display database\n");
	printf("3. Search database\n");
	printf("4. Update database\n");
	printf("5. Save database\n");

	printf("Enter your choice : ");
}
