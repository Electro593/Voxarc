/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum execution_state {
    EXECUTION_UNINITIALIZED,
    EXECUTION_RUNNING,
    EXECUTION_PAUSED,
    EXECUTION_ENDED,
} execution_state;

typedef enum key_state {
    KEY_RELEASED,
    KEY_PRESSED,
    KEY_HELD,
} key_state;

typedef enum file_mode {
    FILE_READ,
    FILE_WRITE,
    FILE_APPEND,
} file_mode;