/* csv.h — error contract
 *
 * Every function returns a CsvStatus. CSV_OK means success and
 * output parameters have been written. Any other value means failure,
 * and output parameters are untouched.
 *
 * On CSV_ERRNO the caller may inspect errno for details.
 * No function frees anything the caller passed in.
 * csv_last_message() returns a human-readable description of the
 * most recent failure on this thread.
 */

typedef enum {
    CSV_OK = 0,
    CSV_ERRNO,          /* a system call failed; see errno */
    CSV_BAD_FORMAT,     /* the input was malformed         */
    CSV_TOO_LONG,       /* a line exceeded the limit       */
    CSV_NO_MEMORY
} CsvStatus;

const char *csv_status_name(CsvStatus s);
