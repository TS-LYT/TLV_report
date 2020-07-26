#include "sql.h"



int callback_insert(void *pv, int argc, char **argv, char **col)
{       
	if( !argc)
	{       
		PRINT_ERRNO;
		return -1;
	}
	if( !argv )
	{       
		PRINT_ERRNO;
		return -1;
	}
	if( !col )
	{       
		PRINT_ERRNO;
		return -1;
	}

	int    i =0;

	printf("%s\n", (const char*)pv);
	printf("insert nedata to database::");
	for(i=0; i<argc; i++)
	{       
		printf("%s    |", argv[i]);
	}       
	printf("\n");
	return 0;
}



int callback_delete(void *pv, int argc, char **argv, char **col)
{
	if( !argc)
	{
		PRINT_ERRNO;
		return -1;
	}
	if( !argv )
	{       
		PRINT_ERRNO;
		return -1;
	}
	if( !col )
	{       
		PRINT_ERRNO;
		return -1;
	}

	int    i =0;

	printf("delete nedata to database:");
	for(i=0; i<argc; i++)
	{
		printf("%s    |", argv[i]);
	}
	printf("\n");
	return 0;
}


int creat_table(sqlite3 *db)
{
	if( !db )
	{
		PRINT_ERRNO; 
		return -1;
	}
	char              *sql = NULL;
	char              *errmsg = NULL;
	int               rv = -1;

	sql = " CREATE TABLE TEMPERATURE(\
	       ID INTEGER PRIMARY KEY AUTOINCREMENT NULL,\
	       CLIENT_IP      CHAR(20) NOT NULL,\
	       SERIAL_NUMBER  CHAR(15) NOT NULL,\
	       TEMP           CHAR(15) NOT NULL,\
	       CLIENT_TIME    CHAR(20) NOT NULL,\
	       SERVER_TIME    CHAR(40) NOT NULL\
	       );";

	/*sql = " CREATE TABLE TEMPERATURE(\
	       TEMP           CHAR(5) NOT NULL,\
	       TIME           CHAR(20) NOT NULL,\
	       CLIENT_TIME    CHAR(20) NOT NULL,\
	       CLIENT_IP      CHAR(20) NOT NULL\
	       );";*/
	rv=sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(rv != SQLITE_OK)
	{
		printf("create table error : %s\n",errmsg);
		return -1;
	}
	printf("create table successful\n");
	return 0;
	sqlite3_free(errmsg );
}


int insert_data(sqlite3 *db, INFO *data)
{
	if( !db )
	{       
		PRINT_ERRNO;
		return -1;
	}
	if ( !data )
	{
		PRINT_ERRNO;
		return -1;
	}

	int               rv = -1;
	char              sql[200];
	char              *errmsg = NULL;
	char              *pv = "Callback function called.";

	memset(sql, 0, sizeof(sql));

	//sql = "INSERT INTO 'TEMPERATURE' VALUES('data->temp', 'data->n_time', 'data->c_time', 'data->c_ip');";

	//sql = sqlite3_mprintf("INSERT INTO TEMPERATURE VALUES('%s', '%s', '%s', '%s')", data->temp, data->n_time, data->c_time, data->c_ip);

	rv=snprintf(sql, sizeof(sql), "INSERT INTO 'TEMPERATURE' VALUES(NULL, '%s', '%s', '%s', '%s', '%s');", data->ip, data->name, data->temp, data->client_time, data->server_time);
	if(rv < 0)
	{
		printf("snprintf failure\n");
        return -1;
	}
	
	
	rv=sqlite3_exec(db, sql, callback_insert, (void*)pv, &errmsg);

	if(rv != SQLITE_OK)
	{
		printf("insert data to database failure:%s\n", errmsg);
		return -2;
	}
	else
	{
		printf("insert data to database successful\n");
	}
	sqlite3_free(errmsg );
}

/*int main()
{
	sqlite3  *db;
	INFO data ={"1","2","3","0","4"};
	sqlite3_open("hhh.db", &db);
	insert_data(db, &data);

}*/




