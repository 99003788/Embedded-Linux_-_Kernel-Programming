# REQUIREMENTS
|**Serial Number**|**Requirement**|**Description**|**Status**|
| :- | :- | :- | :- |
|1|System call to traverse process list and get attributes like state, priority and pid of the calling process |System call traverses through process list and retrieves state, priority and pid of calling process.|Implemented|

# TEST PLAN
|**Serial Number**|**Description**|**Expected Input**|**Expected Output**|**Actual Output**|**Type of Test**|
| :- | :- | :- | :- | :- | :- |
|1|Attributes such as state, priority, PID of calling process is to be printed|Invoking executable file of user-space code on serial console|PID, State, Priority of the calling process.  (on terminal)|PID, State, Priority of the calling process.|Requirement based|

