# Algorithms and Principles of Computer Science Final Project

A.Y. 2019-2020

## Specifications :book:

The project consists in the implementation of a simple text editor.  
The editor interface supports only text commands, terminated by `\n`. Commands can be followed by a portion of text, consisting of one or more lines, terminated by `.`, which appears as the only character on the following line.  
The maximum length of a line of text is **1024** characters.  
Complete specifications can be found [here](specification.pdf) (only in italian).

## Commands :keyboard:

| Command | Example | Effect |
|-------|-------|------|
| `(addr1,addr2)c` | `1,2c` | Changes the text between lines `addr1` and `addr2` (extremes included). The text following this command must contain `addr2-addr1+1` lines of text. `addr1` must be an address effectively present in the text or alternatively the first address after the last line present in the text (1 if the text is empty). |
| `(addr1,addr2)d` | `1,2d` | Deletes the lines between `addr1` and `addr2` (extremes included), lifting up lines following `addr2` (if any). The deletion of a non existing line has no effects. |
| `(addr1,addr2)p` | `1,2p` | Prints lines between `addr1` and `addr2` included. Wherever a line to print isn't present in the text, it's printed a line containing the character `.` followed by `\n`. |
| `(num)u` | `3u` | Performs the undo of the specified number of commands (`c` or `d`). The specified number must be an integer strictly greater than zero. A sequence of consecutive **undo** commands cancels a number of commands equals to the sum of the numbers specified at each step. If the number of commands to undo is greater than the number of the commands executed, all the steps are canceled. The execution of a command which modifies the text (`c` or `d`) after an undo deletes **definitely** the history of undone commands. |
| `(num)r` | `2r` | Cancels the effects of **undo** for the specified number of commands from the current version of the text. `num` must be greater than zero. If the number of redo to perform is greater than the number of commands actually undone, the maximum number of available redo is performed. |
|`q`|`q`| Terminates the execution of the editor. |

## Example of usage

![example_of_usage](/github/example_of_usage.svg)
