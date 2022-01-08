/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef enum execution_state {
    Execution_Stopped,
    Execution_Paused,
    Execution_Running,
} execution_state;

typedef enum key_state {
    Key_Released,
    Key_Pressed,
    Key_Held,
} key_state;