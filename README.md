# pipex
42 - This project will let you discover in detail a UNIX mechanism that you already know by using it in your program.

**Usage :**
```bash
$> make
$> ./pipex file1 cmd1 cmd2 file2
```
It should behave like :
```bash
$> < file1 cmd1 | cmd2 > file2
```
