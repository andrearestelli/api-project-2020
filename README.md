# Algorithms and Principles of Computer Science Final Project

A.Y. 2019-2020

## Specifications

The project consists in the implementation of a simple text editor.  
The editor interface supports only text commands, terminated by `\n`. Commands can be followed by a portion of text, consisting of one or more lines, terminated by ".", which appears as the only character on the following line. The maximum length of a line of text is 1024 characters.

## Commands
|Command|Example|Effect|
|-------|-------|------|
|`(addr1,addr2)c`|`1,2c`|Changes the text between lines `addr1` and `addr2` (extremes included). The text following this command must contain `addr2-addr1+1` lines of text. `addr1` must be an address effectively present in the text or alternatively the first address after the last line present in the text (1 if the text is empty).|
|`(addr1,addr2)d`|`1,2d`|Deletes the lines between `addr1` and `addr2` (extremes included), lifting up lines following `addr2` (if any). The deletion of a non existing line has no effects.|
|`(addr1,addr2)p`|`1,2p`|Prints lines between `addr1` and `addr2` included. Wherever a line to print isn't present in the text, it's printed a line containing the character `.` followed by `\n`.|
