#include <sqlite3ext.h>
#include <math.h>
SQLITE_EXTENSION_INIT1

/* define a sqrt function */
static void sqrtFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	sqlite3_result_double(context, sqrt(sqlite3_value_double(argv[0])));
}

/* SQLite invokes this routine once when it loads the extension.
 * Create new functions, collating sequences, and virtual table
 * modules here.  This is usually the only exported symbol in
 * the shared library.
 */

int sqlite3_extension_init(sqlite3 *db,
		char **pzErrMsg,
		const sqlite3_api_routines *pApi)
{
  SQLITE_EXTENSION_INIT2(pApi);
  sqlite3_create_function(db, "sqrt", 1, SQLITE_ANY, 0, sqrtFunc, 0, 0);
  return 0;
}

